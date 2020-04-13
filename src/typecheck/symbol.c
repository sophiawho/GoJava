#include "symbol.h"
#include "typecheck.h"
#include "../ast/tree.h"
#include "../ast/stmt.h"
#include "../error.h"
#include "../weed.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern int print_sym_table;
int indent_level = 0;

void associateVarWithType(VARSPEC *vs, SymbolTable *scope);

bool isSpecialFunction(char *identifier)
{
    return strcmp((char *) "init", identifier) == 0 || strcmp((char*) "main", identifier) == 0;
}

void makeSymbolTable(PROG *root)
{
    SymbolTable *global_scope = initSymbolTable();

    if (print_sym_table)
    {
        printf("================== Symbol Table =================\n");
    }
    if (root != NULL)
    {
        putBaseTypeSymbols(global_scope);
        symTOPLEVELDECL(root->rootTopLevelDecl, global_scope);
    }
}

/*
 * Put GoLite's default symbols and their respective types into the scope
 * Args:
 *  symTable: the symbol table to add the symbols to
 * returns: void
 */
void putBaseTypeSymbols(SymbolTable *symTable) 
{
    putBaseTypeSpec(symTable, (char *) "int", k_typeInt);
    putBaseTypeSpec(symTable, (char *) "rune", k_typeRune);
    putBaseTypeSpec(symTable, (char *) "bool", k_typeBool);
    putBaseTypeSpec(symTable, (char *) "float64", k_typeFloat);
    putBaseTypeSpec(symTable, (char *) "string", k_typeString);
    putBaseConstants(symTable, (char *) "true", 1);
    putBaseConstants(symTable, (char *) "false", 0);
}

void putBaseConstants(SymbolTable *symTable, char *ident, int boolValue) 
{
    EXP *e = malloc(sizeof(EXP));
    e->lineno = 0;
    e->kind = k_expKindBoolLiteral;
    e->val.boolLiteral = boolValue;
    e->type = makeTYPE(k_typeBool);

    IDENT *i = makeIDENT(ident);
    TYPE *t = makeTYPE(k_typeBool);
    VARSPEC *vs = makeVarSpec(i, e, t);
    SYMBOL *s = putSymbol(symTable, ident, k_symbolKindConstant, 0);
    s->val.varSpec = vs;
    printSymbol(s);
}

void putBaseTypeSpec(SymbolTable *symTable, char *typeName, TypeKind typeKind) 
{
    TYPE *t = makeTYPE_ident(typeName);
    t->kind=typeKind;
    t->parent = NULL; // Base types have NULL as parent pointer
    putSymbol_Type(symTable, typeName, t, 0);
}

void symTOPLEVELDECL(TOPLEVELDECL *tld, SymbolTable *symTable)
{
    if (tld == NULL) return;

    symTOPLEVELDECL(tld->next, symTable);

    switch (tld->kind)
    {
    case k_topLevelDeclFunc:
        symFUNC(tld->val.funcDecl, symTable);
        break;
    case k_topLevelDeclType:
        symTYPESPEC(tld->val.typeDecl, symTable);
        break;
    case k_topLevelDeclVar:
        symVARSPEC(tld->val.varDecl, symTable);
        break;
    }

}

void symFUNC_inputParams_putSymbols(TYPESPEC *ts, SymbolTable *scope)
{
    if (ts == NULL) return;
    symFUNC_inputParams_putSymbols(ts->next, scope);

    VARSPEC *vs = makeVarSpec(ts->ident, NULL, ts->type);
    associateVarWithType(vs, scope);
    ts->type = vs->type;

    for (IDENT *ident = ts->ident; ident; ident = ident->next)
    {
        putSymbol_Var(scope, ident->ident, vs, ts->lineno);
    }
}

void symFUNC_inputParams_findParentTypes(TYPESPEC *ts, SymbolTable *scope)
{
    if (ts == NULL) return;
    symFUNC_inputParams_findParentTypes(ts->next, scope);

    findParentType(scope, ts->type);
}

void symFUNC_inputParams(TYPESPEC *ts, SymbolTable *scope) {
    if (ts == NULL) return;

    // Need to first find the parent types to make sure they are declared within the scope
    symFUNC_inputParams_findParentTypes(ts, scope);

    // Then we iterate over all typespecs again and start putting symbols. We do this because otherwise
    // we might add a type's symbol as a variable into the scope, which leads to an error.
    // For example: `f(a int, int int, b int)`. Once we put `int` as a variable into the scope, trying
    // to get type for b will result in an error.
    // Additionally, we need to traverse from tail to head, therwise function parameters get reversed
    symFUNC_inputParams_putSymbols(ts, scope);
}

void symFUNC(FUNC *f, SymbolTable *scope)
{
    if (f->returnType != NULL) {
        TYPE *t = findParentType(scope, f->returnType);
        if (f->returnType->kind == k_typeInfer) {
            SYMBOL *s = getSymbol(scope, f->returnType->val.identifier, f->lineno);
            f->returnType = s->val.type;
        } else if (f->returnType->kind == k_typeSlice) {
            f->returnType->val.sliceType.type = t;
        } else if (f->returnType->kind == k_typeArray) {
            f->returnType->val.arrayType.type = t;
        }
    }
    putSymbol_Func(scope, f->name, f, f->lineno);

    SymbolTable *innerScope = scopeSymbolTable(scope);
    openScope();

    if (f->inputParams != NULL) {
        TYPESPEC *ts = f->inputParams;
        symFUNC_inputParams(ts, innerScope);
    }
    
    // (Sophia): Call symSTMT on STMT inside blockSTMT, so that
    // symbol table does not scope. The statements inside the 
    // block statement should be the same scope as the input parameters
    symSTMT(f->rootStmt->val.blockStmt, innerScope);
    closeScope();
}

void symSTMT(STMT *s, SymbolTable *scope)
{
    if (s == NULL) return;

    symSTMT(s->next, scope);

    SymbolTable *innerScope;
    SymbolTable *trueBodyScope;
    SymbolTable *falseBodyScope;

    switch (s->kind)
    {
    case k_stmtKindEmpty:
        break;

    case k_stmtKindExpStmt:
        symEXP(s->val.expStmt, scope);
        break;

    case k_stmtKindIncDec:
        symEXP(s->val.incDecStmt.exp, scope);
        break;

    case k_stmtKindAssign:
        symSTMT_assign(s, scope);
        break;

    case k_stmtKindPrint:
        symEXP(s->val.printStmt.expList, scope);
        break;

    case k_stmtKindVarDecl:
        symVARSPEC(s->val.varDecl, scope);
        break;

    case k_stmtKindTypeDecl:
        symTYPESPEC(s->val.typeDecl, scope);
        break;

    case k_stmtKindBlock:
        innerScope = scopeSymbolTable(scope);
        openScope();
        symSTMT(s->val.blockStmt, innerScope);
        closeScope();
        break;

    case k_stmtKindIfStmt:

        /* Create an additional inner scope for the simple statement */
        /* Whatever occurs in this simple statement, needs to be accessible by both the */
        /* true body and false body*/
        innerScope = scopeSymbolTable(scope);
        openScope();

        if (s->val.ifStmt.simpleStmt != NULL) symSTMT(s->val.ifStmt.simpleStmt, innerScope);

        // TODO typecheck expression resolves to bool
        symEXP(s->val.ifStmt.condition, innerScope);

        if (s->val.ifStmt.trueBody != NULL) 
        {
            trueBodyScope = scopeSymbolTable(innerScope);
            openScope();
            symSTMT(s->val.ifStmt.trueBody, trueBodyScope);
            closeScope();
        }
        
        if (s->val.ifStmt.falseBody != NULL) 
        {
            falseBodyScope = scopeSymbolTable(innerScope);
            openScope();
            symSTMT(s->val.ifStmt.falseBody, falseBodyScope);
            closeScope();
        }

        closeScope();
        break;

    case k_stmtKindSwitch:

        /* Create an additional inner scope for the simple statement */
        /* Whatever occurs in this simple statement, needs to be accessible by all clauses */
        innerScope = scopeSymbolTable(scope);
        
        openScope();

        if (s->val.switchStmt.simpleStmt != NULL) symSTMT(s->val.switchStmt.simpleStmt, innerScope);

        symEXP(s->val.switchStmt.exp, innerScope);
        symEXPRCASECLAUSE(s->val.switchStmt.caseClauses, innerScope);

        closeScope();
        break;

    case k_stmtKindFor:
        symSTMT_forLoop(s, scope); // TODO maybe change
        break;

    case k_stmtKindBreak:
    case k_stmtKindContinue:
        break;

    case k_stmtKindReturn:
        symEXP(s->val.returnExp, scope);
        break;

    }
}

bool isIdentifierDeclared(EXP *lhs, SymbolTable *t) {
    char *name = lhs->val.identExp.ident;
    int hash = Hash(name);
    for (SYMBOL *s = t->table[hash]; s; s = s->next) 
    {
        if (strcmp(s->name, name) == 0) return true;
    }
    return false;
}

void checkUndeclared(EXP *lhs, SymbolTable *t) 
{
    bool oneUndeclared = false;
    for (EXP *cur = lhs; cur; cur = cur->next) {
        if (!isIdentifierDeclared(cur, t) && !isBlankId(cur->val.identExp.ident)) oneUndeclared = true;
    }

    if (!oneUndeclared) {
        throwError("At least one variable on the LHS of a short declaration must be undeclared in the current scope.", lhs->lineno);
    }
}

void checkAllUnique(EXP *e)
{
    SymbolTable *scope = initSymbolTable();
    for (EXP *current = e; current; current = current->next)
    {
        putSymbol(scope, current->val.identExp.ident, k_symbolKindVar, current->lineno);
    }
}

void symSTMT_assign_colonAssign(EXP *lhs, EXP *rhs, SymbolTable *scope)
{
    if (lhs == NULL && rhs == NULL) return;
    symSTMT_assign_colonAssign(lhs->next, rhs->next, scope);

    symEXP(rhs, scope);

    if (!isIdentifierDeclared(lhs, scope)) {
        IDENT *i = makeIDENT(lhs->val.identExp.ident);
        TYPE *t = makeTYPE(k_typeInfer);
        VARSPEC *vs = makeVarSpec(i, rhs, t);
        putSymbol_Var(scope, lhs->val.identExp.ident, vs, lhs->lineno);
    }
    else
    {
        // Symbol already exists so we need to overwrite its VARSPEC by changing it manually, otherwise putting the symbol
        // throws a redeclared error
        SYMBOL *s = getSymbol(scope, lhs->val.identExp.ident, lhs->lineno);
        s->val.varSpec->rhs = rhs;
        printSymbol(s);
    }

    symEXP(lhs, scope);
}

void symSTMT_assign(STMT *s, SymbolTable *scope)
{
    switch (s->val.assignStmt.kind)
    {
    case k_stmtColonAssign:
        // Need to recursively call on each identifier and expression on LHS and RHS
        checkUndeclared(s->val.assignStmt.lhs, scope);
        checkAllUnique(s->val.assignStmt.lhs);
        symSTMT_assign_colonAssign(s->val.assignStmt.lhs, s->val.assignStmt.rhs, scope);
        break;

    default:
        symEXP(s->val.assignStmt.lhs, scope);
        symEXP(s->val.assignStmt.rhs, scope);
        break;
    }
}

void symEXPRCASECLAUSE(EXPRCASECLAUSE *cc, SymbolTable *scope)
{
    if (cc == NULL) return;
    symEXPRCASECLAUSE(cc->next, scope);
    
    openScope();
    SymbolTable *innerScope = scopeSymbolTable(scope);
    closeScope();

    switch (cc->kind)
    {
    case k_caseClause:

        symEXP(cc->expList, scope);
        symSTMT(cc->stmtList, innerScope);

        break;
    
    case k_defaultClause:

        symSTMT(cc->stmtList, innerScope);
        break;
    }

}

void symSTMT_forLoop(STMT *s, SymbolTable *scope)
{
    SymbolTable *innerScope = scopeSymbolTable(scope);

    openScope();
    
    switch (s->val.forLoop.kind)
    {
    case k_loopKindInfinite:
        symSTMT(s->val.forLoop.body, innerScope);
        break;

    case k_loopKindWhile:
        symEXP(s->val.forLoop.condition, innerScope);
        symSTMT(s->val.forLoop.body, innerScope);
        break;

    case k_loopKindThreePart:
        symSTMT(s->val.forLoop.initStmt, innerScope);
        symEXP(s->val.forLoop.condition, innerScope);
        symSTMT(s->val.forLoop.postStmt, innerScope);
        symSTMT(s->val.forLoop.body, innerScope);
        break;
    }
    
    closeScope();
}

void symSTRUCTSPEC(STRUCTSPEC *ss, char *structIdentifier, SymbolTable *scope, SymbolTable *structScope)
{
    if (ss == NULL) return;
    symSTRUCTSPEC(ss->next, structIdentifier, scope, structScope);

    // Check if the STRUCTSPEC type is defined in the scope
    // Associate ss->type with parent type
    if (isRecursive(ss->type)) {
        SYMBOL *s = putSymbol(scope, structIdentifier, k_symbolKindType, ss->lineno);
        SymbolTable *recursiveScope = scopeSymbolTable(scope);
        TYPE *t = findFieldTypeForStruct(recursiveScope, ss->type);
        free(s);
        ss->type=t;
    } else {
        TYPE *t = findFieldTypeForStruct(scope, ss->type);
        ss->type = t;
    }

    // Will help check to see if attribute has already been declared or not
    for (IDENT *i = ss->attribute; i; i = i->next) {
        if (!isBlankId(ss->attribute->ident)) putSymbol(structScope, i->ident, 
            k_symbolKindVar, ss->lineno);
    }
}

// A type is allowed to be recursive if, along the path of recursion, there is a slice. All other cases are disallowed
bool isRecursive(TYPE *type) {
    if (type->kind == k_typeSlice || type->kind == k_typeStruct) {
        return true;
    } else if (type->kind == k_typeArray) {
        return isRecursive(type->val.arrayType.type);
    }
    return false;
}

void symTYPESPEC(TYPESPEC *ts, SymbolTable *symTable)
{
    if (ts == NULL) return;
    symTYPESPEC(ts->next, symTable);

    char *ident = ts->ident->ident;

    // This scope is only used to keep track of identifiers declared in a struct specification
    // It's used to validate whether some identifier has been previously declared or not
    // Don't use it as an actual new scope of code
    SymbolTable *structScope;
    
    switch (ts->kind)
    {
        case k_typeSpecKindTypeDeclaration: ;
            if (isBlankId(ident)) return;
            TYPE *t = ts->type;
            t->typeName = ident;
            if (t->kind == k_typeInfer) {
                TYPE *parentType = findParentType(symTable, t);
                t->parent = parentType;
                putSymbol_Type(symTable, ident, t, ts->lineno);
                return;
            }
            if (t->kind == k_typeStruct) {
                structScope = initSymbolTable();
                symSTRUCTSPEC(t->val.structType, ident, symTable, structScope);
                free(structScope);
                putSymbol_Type(symTable, ident, t, ts->lineno);
            } else if (isRecursive(t)) { // If type is recursive, use inner scope
                SYMBOL *s = putSymbol(symTable, ident, k_symbolKindType, ts->lineno);
                s->val.type = t;
                SymbolTable *innerScope = scopeSymbolTable(symTable);
                findParentType(innerScope, t);
                putSymbol_Type(innerScope, ident, t, ts->lineno);
            } else {
                findParentType(symTable, t);
                putSymbol_Type(symTable, ident, t, ts->lineno);
            }
            break;
        default:
            break;
    }
}

TYPE *findParentType(SymbolTable *symTable, TYPE *t) {
    if (t->kind == k_typeSlice) {
        return findParentType(symTable, t->val.sliceType.type);
    } else if (t->kind == k_typeArray) {
        return findParentType(symTable, t->val.arrayType.type);
    } else if (t->kind == k_typeStruct) {
        STRUCTSPEC *cur = t->val.structType;
        while (cur != NULL) {
            findParentType(symTable, cur->type);
            cur = cur->next;
        }
        return NULL; // a struct has no parent type
    }
    SYMBOL *s = getSymbol(symTable, t->val.identifier, t->lineno);
    if (s == NULL ) {
        throwErrorUndefinedId(t->lineno, t->val.identifier);
    }
    if (s->kind != k_symbolKindType) 
    {
        fprintf(stderr, "Error: (line %d) %s is not a type", t->lineno, s->name);
        exit(EXIT_FAILURE);
    }
    return s->val.type;
}

TYPE *findFieldTypeForStruct(SymbolTable *symTable, TYPE *t) {
    if (t->kind == k_typeSlice) {
        TYPE *parent = findParentType(symTable, t->val.sliceType.type);
        TYPE *sliceType = makeTYPE_slice(parent);
        sliceType->typeName = parent->typeName;
        return sliceType;
    } else if (t->kind == k_typeArray) {
        TYPE *parent = findParentType(symTable, t->val.arrayType.type);
        TYPE *arrayType = makeTYPE_array(t->val.arrayType.size, parent);
        arrayType->typeName = parent->typeName;
        return arrayType;
    } else if (t->kind == k_typeStruct) {
        return findFieldTypeForStruct(symTable, t->val.structType->type);
    }
    SYMBOL *s = getSymbol(symTable, t->val.identifier, t->lineno);
    if (s == NULL ) {
        throwErrorUndefinedId(t->lineno, t->val.identifier);
    }
    return s->val.type;
}

/*
* Function: Associates a variable with its type information given a scope. Queries for the type's symbol from the
* symbol table and assigns the vs->type with the queried symbol's TYPE;
*
* Args:
*   VARSPEC *vs: Variable Specification AST node to wire symbol TYPE with
*   SymbolTable *scope: The symbol table to query for a Variable Specification type's symbol from
*
* Returns: void
*/
void associateVarWithType(VARSPEC *vs, SymbolTable *scope) {
    if (vs->type->kind == k_typeInfer) {
        SYMBOL *s = getSymbol(scope, vs->type->val.identifier, vs->lineno);
        vs->type = s->val.type;
    }
    if (vs->type->kind == k_typeSlice) {
        TYPE *t = findParentType(scope, vs->type);
        if (vs->type->val.sliceType.type->kind == k_typeInfer) {
            vs->type->val.sliceType.type = t;
        } else if (vs->type->val.sliceType.type->kind == k_typeArray) {
            vs->type->val.sliceType.type->val.arrayType.type = t;
        } else if (vs->type->val.sliceType.type->kind == k_typeSlice) {
            vs->type->val.sliceType.type->val.sliceType.type = t;
        }
    } else if (vs->type->kind == k_typeArray) {
        TYPE *t = findParentType(scope, vs->type);
        if (vs->type->val.arrayType.type->kind == k_typeInfer) {
            vs->type->val.arrayType.type = t;
        } else if (vs->type->val.arrayType.type->kind == k_typeArray) {
            vs->type->val.arrayType.type->val.arrayType.type = t;
        } else if (vs->type->val.arrayType.type->kind == k_typeSlice) {
            vs->type->val.arrayType.type->val.sliceType.type = t;
        }
    } else if (vs->type->kind == k_typeStruct) {
        for (STRUCTSPEC *ss = vs->type->val.structType; ss; ss = ss->next) {
            TYPE *t = findFieldTypeForStruct(scope, ss->type);
            ss->type = t;
        }
    }
}

void symVARSPEC(VARSPEC *vs, SymbolTable *scope)
{
    if (vs == NULL) return;
    symVARSPEC(vs->next, scope);

    if (vs->type != NULL) {
        findParentType(scope, vs->type);
        // fprintf(stdout, "type: %s\n", typeToString(vs->type));
        // fprintf(stdout, "type: %s\n", typeToString(vs->type->val.arrayType.type));
        associateVarWithType(vs, scope);
        // SLICE TYPE DISAPPEARS!!
        // fprintf(stdout, "type: %s\n", typeToString(vs->type->val.arrayType.type));
    }
    if (vs->rhs != NULL) {
        symEXP(vs->rhs, scope);
        SYMBOL *s = vs->rhs->val.identExp.symbol;
        if (s != NULL)
        {
            if (s->kind == k_symbolKindType) throwError("Cannot assign a symbol of kind `type` to a variable", vs->lineno);
        }
    }

    IDENT *ident = vs->ident;
    while (ident != NULL)
    {
        putSymbol_Var(scope, ident->ident, vs, vs->lineno);
        ident = ident->next;
    }
}

void symTYPECAST(EXP *e, EXP *typeExpr, EXP *exprToCast) {
    SYMBOL *s = getSymbolFromExp(typeExpr);
    if (s->kind != k_symbolKindType) return;
    e->kind = k_expKindCast;
    e->val.cast.type = s->val.type;
    e->val.cast.exp = exprToCast;
    if (!exprToCast) return;
    s = getSymbolFromExp(exprToCast);
    if (s != NULL && s->kind == k_symbolKindConstant) {
        e->val.cast.exp->type = s->val.varSpec->type;
    }
}

void symEXP(EXP *exp, SymbolTable *scope)
{
    if (exp == NULL) return;
    symEXP(exp->next, scope);

    SYMBOL *s;
    switch (exp->kind)
    {
    case k_expKindIdentifier:
        s = getSymbol(scope, exp->val.identExp.ident, exp->lineno);
        exp->val.identExp.symbol = s;
        break;

    case k_expKindAnd:
    case k_expKindOr:
    case k_expKindEq:
    case k_expKindNotEq:
    case k_expKindLess:
    case k_expKindLessEq:
    case k_expKindGrtr:
    case k_expKindGrtrEq:
    case k_expKindAdd:
    case k_expKindMinus:
    case k_expKindBitOr:
    case k_expKindBitXOR:
    case k_expKindMult:
    case k_expKindDiv:
    case k_expKindMod:
    case k_expKindLeftShift:
    case k_expKindRightShift:
    case k_expKindBitAnd:
    case k_expKindBitClear:
        symEXP(exp->val.binary.lhs, scope);
        symEXP(exp->val.binary.rhs, scope);
        break;

    case k_expKindUPlus:
    case k_expKindUMinus:
    case k_expKindBang:
    case k_expKindUBitXOR: // ATTN: This is similarly named to binary op k_expKindBitXOR
    case k_expKindUParenthesized:
        symEXP(exp->val.unary.rhs, scope);
        break;

    case k_expKindFuncCall:
        symEXP(exp->val.funcCall.primaryExpr, scope);
        symEXP(exp->val.funcCall.expList, scope);
        symTYPECAST(exp, exp->val.funcCall.primaryExpr, exp->val.funcCall.expList);
        break;

    case k_expKindArrayAccess:
        symEXP(exp->val.arrayAccess.arrayReference, scope);
        symEXP(exp->val.arrayAccess.indexExp, scope);
        break;

    case k_expKindFieldAccess:
        symEXP(exp->val.fieldAccess.object, scope);
        break;

    case k_expKindAppend:
        symEXP(exp->val.append.addend, scope);
        symEXP(exp->val.append.slice, scope);
        break;
        
    case k_expKindLen:
        symEXP(exp->val.lenExp, scope);
        break;
        
    case k_expKindCap:
        symEXP(exp->val.capExp, scope);
        break;

    case k_expKindIntLiteral:
    case k_expKindFloatLiteral:
    case k_expKindBoolLiteral:
    case k_expKindRuneLiteral:
    case k_expKindStringLiteral:
        break;
        
    default:
        break;
    }
}

SymbolTable *initSymbolTable()
{
    SymbolTable *t = malloc(sizeof(SymbolTable));

    for (int i = 0; i < HASH_SIZE; i++)
    {   
        t->table[i] = NULL;
    }
    t->parent = NULL;
    return t;
}

SymbolTable *scopeSymbolTable(SymbolTable *parent)
{
    SymbolTable *scope = initSymbolTable();
    scope->parent = parent;
    return scope;
}

int Hash(char *str)
{
    unsigned int hash = 0;
    while (*str) hash = (hash << 1) + *str++;
    return hash % HASH_SIZE;
}

SYMBOL *getSymbol(SymbolTable *t, char *name, int lineno)
{    
    if (name == NULL) throwInternalError("null identifier in 'getSymbol'", lineno);
    if (isBlankId(name)) return NULL;
    int hash = Hash(name);

    for (SYMBOL *s = t->table[hash]; s; s = s->next) 
    {
        if (strcmp(s->name, name) == 0) return s;
    }

    if (t->parent == NULL) throwErrorUndefinedId(lineno, name);
    return getSymbol(t->parent, name, lineno);
}

SYMBOL *getSymbolOnlyFromScope(SymbolTable *t, char *name)
{
    int hash = Hash(name);

    for (SYMBOL *s = t->table[hash]; s; s = s->next) 
    {
        if (strcmp(s->name, name) == 0) return s;
    }
    return NULL;
}

SYMBOL *putSymbol(SymbolTable *t, char *name, SymbolKind kind, int lineno)
{    
    if (isBlankId(name)) return NULL;
    int hash = Hash(name);
    for (SYMBOL *s = t->table[hash]; s; s = s->next)
    {   
        if (strcmp(s->name, name) == 0) throwErrorRedeclaredId(lineno, s->name);
    }

    SYMBOL *s = malloc(sizeof(SYMBOL));
    s->name = name;
    s->kind = kind;
    s->next = t->table[hash];
    t->table[hash] = s;
    return s;
}

SYMBOL *putSymbol_Func(SymbolTable *t, char *name, FUNC *funcSpec, int lineno)
{
    if (isBlankId(name)) return NULL;
    if (isSpecialFunction(name)) {
        if (funcSpec->returnType != NULL || funcSpec->inputParams != NULL) {
            throwSpecialFunctionParameterError(name, lineno);
        } 
        if (strcmp(name, (char *) "init") == 0) {
            if (print_sym_table) printf("init [function] = <unmapped>\n");
            return NULL;
        }
    }
    SYMBOL *s = putSymbol(t, name, k_symbolKindFunc, lineno);
    s->val.funcSpec = funcSpec;
    printSymbol(s);
    return s;
}

SYMBOL *putSymbol_Type(SymbolTable *t, char *name, TYPE *type, int lineno) 
{
    if (isSpecialFunction(name)) throwSpecialFunctionDeclarationError(name, lineno);
    if (isBlankId(name)) return NULL;
    SYMBOL *s = putSymbol(t, name, k_symbolKindType, lineno);
    s->val.type = type;
    printSymbol(s);
    return s;
}

SYMBOL *putSymbol_Var(SymbolTable *t, char *name, VARSPEC *varSpec, int lineno) 
{
    if (isSpecialFunction(name)) throwSpecialFunctionDeclarationError(name, lineno);
    if (isBlankId(name)) return NULL;
    SYMBOL *s = putSymbol(t, name, k_symbolKindVar, lineno);
    s->val.varSpec = varSpec;
    printSymbol(s);
    return s;
}

void printIndent() {
  for (int i = 0; i < indent_level; i++) {
    printf("\t");
  }
}

void openScope() {
  if (print_sym_table) {
    printIndent();
    printf("{\n");
  }
  indent_level++;
}

void closeScope() {
  indent_level--;
  if (print_sym_table) {
    printIndent();
    printf("}\n");
  }
}

// sym_name [sym_kind] = [type -> parenttype]
void printSymbol(SYMBOL *s) {
    if (!print_sym_table) return;
    printIndent();
    printf("%s", s->name);
    switch (s->kind) {
        case k_symbolKindConstant:
            printf(" [constant] = bool");
            break;
        case k_symbolKindVar:
            printf(" [variable] = ");
            if (s->val.varSpec->type != NULL) {
                printType(s->val.varSpec->type);
            } else {
                printf("<infer>");
            }
            break;
        case k_symbolKindType:
            printf(" [type] = ");
            printType(s->val.type);
            break;
        case k_symbolKindFunc:
            printf(" [function] = (");
            TYPESPEC *cur_ts = s->val.funcSpec->inputParams;
            // print input params
            while (cur_ts != NULL) {
                printType(cur_ts->type);
                if (cur_ts->next != NULL) {
                    printf(", ");
                }
                cur_ts = cur_ts->next;
            }
            printf(") -> ");
            // print return type
            if (s->val.funcSpec->returnType == NULL) {
                printf("void");
            } else {
                printType(s->val.funcSpec->returnType);
            }
            break;
    }
    printf("\n"); 
}

void printStructSpec(STRUCTSPEC *ss) {
    if (ss == NULL) return;
    printStructSpec(ss->next);
    for (IDENT *ident = ss->attribute; ident; ident = ident->next) {
        printf(" %s ", ident->ident);
        printType(ss->type);
        printf(";");
    }
}

void printType(TYPE *t) {
    switch (t->kind) {
        case k_typeInfer:
            if (t->typeName != NULL) {
                printf("%s", t->typeName);
            } else if (t->val.identifier != NULL){
                printf("%s", t->val.identifier);
            } else {
                printf("%s", "<infer>");
            }
            break;
        case k_typeSlice:
            if (t->typeName != NULL) {
                printf("%s -> ", t->typeName);
            } 
            printf("[]"); 
            if (t->val.sliceType.type->kind == k_typeStruct) {
                printf("%s", t->val.sliceType.type->typeName);
            } else {
                printType(t->val.sliceType.type);
            }
            break;
        case k_typeArray:
            if (t->typeName != NULL) {
                printf("%s -> ", t->typeName);
            } 
            printf("[%d]", t->val.arrayType.size);
            if (t->val.sliceType.type->kind == k_typeStruct) { // TODO: Fix symbol table segfault
                printf("%s", t->val.arrayType.type->typeName);
            } else {
                printType(t->val.arrayType.type);
            }
            break;
        case k_typeStruct:
            printf("struct { ");
            printStructSpec(t->val.structType);
            printf(" } ");
            break;
        case k_typeInt:
            printf("int");
            break;
        case k_typeFloat:
            printf("float");
            break;
        case k_typeBool:
            printf("bool");
            break;
        case k_typeRune:
            printf("rune");
            break;
        case k_typeString:
            printf("string");
            break;
        default:
            break;
    }
    if (t->parent != NULL) {
        printf(" -> ");
        printType(t->parent);
    }
}
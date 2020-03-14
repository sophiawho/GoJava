#include "symbol.h"
#include "typecheck.h"
#include "../ast/tree.h"
#include "../ast/stmt.h"
#include "../error.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern int print_sym_table;
int indent_level = 0;

SYMBOL *putSymbol(SymbolTable *t, char *name, SymbolKind kind, int lineno);
SYMBOL *getSymbol(SymbolTable *t, char *name);
SYMBOL *getSymbolOnlyFromScope(SymbolTable *t, char *name);
SymbolTable *initSymbolTable();
SymbolTable *scopeSymbolTable(SymbolTable *parent);

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
    // TODO print symbols if print flag enabled
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

    IDENT *i = makeIDENT(ident);
    TYPE *t = makeTYPE(k_typeBool);
    VARSPEC *vs = makeVarSpec(i, e, t);
    SYMBOL *s = putSymbol_Var(symTable, ident, vs, 0);
    s->kind = k_symbolKindConstant;
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

    SymbolTable *innerScope = scopeSymbolTable(symTable);

    openScope();

    switch (tld->kind)
    {
    case k_topLevelDeclFunc:
        symFUNC(tld->val.funcDecl, innerScope);
        break;
    case k_topLevelDeclType:
        symTYPESPEC(tld->val.typeDecl, innerScope);
        break;
    case k_topLevelDeclVar:
        symVARSPEC(tld->val.varDecl, innerScope);
        break;
    }

    closeScope();
}

void symFUNC(FUNC *f, SymbolTable *symTable)
{
    // TODO finish
    if (f->returnType != NULL) {
        // TODO typecheck the return type exists in the given scope
    }
    if (f->inputParams != NULL) {
        // TODO typecheck the types for each param exists in the given scope
    }
    putSymbol_Func(symTable, f->name, f, f->lineno);
    symSTMT(f->rootStmt, symTable);
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
        // TODO typecheck expression
        break;

    case k_stmtKindIncDec:
        // TODO typecheck expression == int | float64 | rune
        break;

    case k_stmtKindAssign:
        symSTMT_assign(s, scope);
        break;

    case k_stmtKindPrint:
        // TODO typecheck all types are well typed
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

void symSTMT_assign(STMT *s, SymbolTable *scope)
{
    switch (s->val.assignStmt.kind)
    {
    case k_stmtAssign:
    case k_stmtColonAssign:
        // TODO typecheck type(LHS) >= type(RHS)
        break;
    case k_opAssignKindPlusEq:
    case k_opAssignKindMinusEq:
    case k_opAssignKindMultEq:
    case k_opAssignKindDivEq:
    case k_opAssignKindModEq:
        // TODO check if numerical on LHS ? I'm not sure since golite specs don't detail
        break;
    case k_opAssignKindBitAndEq:
    case k_opAssignKindBitOrEq:
    case k_opAssignKindBitXorEq:
    case k_opAssignKindLeftShiftEq:
    case k_opAssignKindRightShiftEq:
    case k_opAssignKindBitClearEq:
        // TODO typecheck 
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
        symSTMT(s->val.forLoop.body, innerScope);
        break;
    }
    
    closeScope();
}


void symTYPESPEC(TYPESPEC *ts, SymbolTable *symTable)
{
    if (ts == NULL) return;
    symTYPESPEC(ts->next, symTable);

    IDENT *ident = ts->ident;
    TYPE *newType;
    switch (ts->kind)
    {
        case k_typeSpecKindTypeDeclaration:
            // TYPE t = ts->type
            /*
            type a int  -> makeTypeSpec(kind, a, int)
            // TODO make sure that type int is in the symbol table
            (TYPE int)
            (TYPE a) -> (TYPE int)
            */
            newType = makeTYPE(ts->type->kind);
            newType->parent = ts->type;
            putSymbol_Type(symTable, ident->ident, newType, ts->lineno);
            break;
        default:
            break;
    }
}

void symVARSPEC(VARSPEC *vs, SymbolTable *scope)
{
    if (vs == NULL) return;
    symVARSPEC(vs->next, scope);

    TYPE *type = vs->type;
    if (type != NULL) {
        // TODO check that type exists
    }
    if (type != NULL && vs->rhs != NULL) {
        // TODO check type(expressions) == type(type)
    }

    IDENT *ident = vs->ident;
    while (ident != NULL)
    {
        putSymbol_Var(scope, ident->ident, vs, vs->lineno);
        ident = ident->next;
    }
}

void symEXP(EXP *exp, SymbolTable *scope)
{
    if (exp == NULL) return;
    symEXP(exp->next, scope);

    switch (exp->kind)
    {
    case k_expKindIdentifier:
        getSymbol(scope, exp->val.identExp.ident);
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
        break;

    case k_expKindArrayAccess:
        symEXP(exp->val.arrayAccess.arrayReference, scope);
        symEXP(exp->val.arrayAccess.indexExp, scope);
        break;

    case k_expKindFieldAccess:
        symEXP(exp->val.fieldAccess.object, scope);
        // TODO nothing with field right?
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

SYMBOL *getSymbol(SymbolTable *t, char *name)
{
    int hash = Hash(name);

    for (SYMBOL *s = t->table[hash]; s; s = s->next) 
    {
        if (strcmp(s->name, name) == 0) return s;
    }

    if (t->parent == NULL) return NULL;
    return getSymbol(t->parent, name);
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
    SYMBOL *s = putSymbol(t, name, k_symbolKindFunc, lineno);
    s->val.funcSpec = funcSpec;
    printSymbol(s);
    return s;
}

SYMBOL *putSymbol_Type(SymbolTable *t, char *name, TYPE *type, int lineno) 
{
    SYMBOL *s = putSymbol(t, name, k_symbolKindType, lineno);
    s->val.type = type;
    printSymbol(s);
    return s;
}

SYMBOL *putSymbol_Var(SymbolTable *t, char *name, VARSPEC *varSpec, int lineno) 
{
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
    // print symbol name
    printIndent();
    printf("%s", s->name);
    // print symbol kind
    switch (s->kind) {
        case k_symbolKindConstant:
            printf(" [constant] = bool");
            break;
        case k_symbolKindVar:
            printf(" [variable] = ");
            printType(s->val.varSpec->type);
            break;
        case k_symbolKindType:
            printf(" [type] = ");
            printType(s->val.type);
            break;
        case k_symbolKindFunc:
            printf(" [function] = ");
            // print param list
            // print void if no return type
            // else print return type

            // main [function] = () -> void
            // main [function] = (param_list) -> void
            break;
    }
    // print type
    printf("\n"); // new line
}

void printType(TYPE *t) {
    switch (t->kind) {
        case k_typeSlice:
            printf("[]%s", t->val.identifier);
            // printf("[]%s"); 
            // printType(t->val.sliceType->type);
            break;
        case k_typeArray:
            printf("[%d]%s", t->val.arrayType.size, t->val.identifier);
            // printf("[%d]", t->val.arrayType.size)
            // printType(t->val.arrayType->type);
            break;
        case k_typeStruct:
            printf("struct { ");
            STRUCTSPEC *ss = t->val.structType;
            while (ss != NULL) {
                // TODO print identifiers list
                printf("%s", ss->attribute->ident);
                // TODO print types
                printType(ss->type);
                printf(";");
                ss = ss->next;
            }
            printf(" } ");
            break;
        case k_typeInfer:
            printf("%s", t->val.identifier);
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
    }
    if (t->parent != NULL) {
        printf(" -> ");
        printType(t->parent);
    }
}
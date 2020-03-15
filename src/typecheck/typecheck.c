#include "typecheck.h"
#include "../ast/tree.h"
#include "../ast/stmt.h"
#include "../error.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// ====================== HELPER FUNCTIONS =====================================
// 1.2.1 Resolve Type: Find the underlying type a defined type T given the set of types in scope T
TYPE *resolveType(TYPE *t) {
    if (t->kind == k_typeInt ||
        t->kind == k_typeBool ||
        t->kind == k_typeFloat ||
        t->kind == k_typeRune || 
        t->kind == k_typeString || 
        t->kind == k_typeSlice ||
        t->kind == k_typeArray || 
        t->kind == k_typeStruct ) return t;
    return resolveType(t->parent);
}

bool isEqualType(TYPE *t1, TYPE *t2) {
    if (t1 == NULL || t2 == NULL) return false;
    if (t1->kind != t2->kind) return false;
    STRUCTSPEC *ss1;
    STRUCTSPEC *ss2;
    switch(t1->kind) {
        case k_typeSlice:
            return isEqualType(t1->val.sliceType.type, t2->val.sliceType.type);
        case k_typeArray:
            if (t1->val.arrayType.size != t2->val.arrayType.size) return false;
            return isEqualType(t1->val.arrayType.type, t2->val.arrayType.type);
        case k_typeStruct:
            ss1 = t1->val.structType;
            ss2 = t2->val.structType;
            while (ss1 != NULL && ss2 != NULL) {
                if (strcmp(ss1->attribute->ident, ss2->attribute->ident) != 0) return false;
                if (!isEqualType(ss1->type, ss2->type)) return false;
                ss1 = ss1->next;
                ss2 = ss2->next;
            }
            if (ss1 != NULL || ss2 != NULL) return false;
            return true;
        case k_typeInfer:
            return strcmp(t1->val.identifier, t2->val.identifier) == 0;
        default:
            return true;
    }
    return false;
}

/*
* Function: Determines whether a type is comparable
* 
* Args: 
*   TYPE *t: The type to determine whether it's comparable
*
* Returns: bool
*/
bool isComparable(TYPE *t) {
    if (t->kind == k_typeInt ||
        t->kind == k_typeBool ||
        t->kind == k_typeFloat ||
        t->kind == k_typeRune || 
        t->kind == k_typeString ) return true;

    // Array type is comparable if values of array element are comparable
    if (t->kind == k_typeArray) {
        return isComparable(t->val.arrayType.type);
    }

    // Struct values are comparable if all their fields are comparable
    if (t->kind == k_typeStruct) {

        bool isComp = isComparable(t->val.structType->type);
        if (t->val.structType->next != NULL) {
            isComp = isComp && isComparable(t->val.structType->next->type);
        }
        return isComp;
    }
    return false;
}

bool isOrdered(TYPE *t) {
    if (t->kind == k_typeInt ||
        t->kind == k_typeBool ||
        t->kind == k_typeFloat ||
        t->kind == k_typeRune || 
        t->kind == k_typeString ) return true;
    return false;
}

bool resolveToBaseType(TYPE *t){
    TYPE* rt = resolveType(t);
    if (rt->kind == k_typeInt ||
        rt->kind == k_typeFloat ||
        rt->kind == k_typeRune ||
        rt->kind == k_typeString ||
        rt->kind == k_typeBool) return true;
    return false;
}

bool resolveToNumbericBaseType(TYPE *t){
    TYPE* rt = resolveType(t);
    if (rt->kind == k_typeInt ||
        rt->kind == k_typeFloat ||
        rt->kind == k_typeRune) return true;
    return false;
}

bool resolveToBoolBaseType(TYPE *t){
    TYPE* rt = resolveType(t);
    if (rt->kind == k_typeBool) return true;
    return false;
}

bool resolveToIntegerBaseType(TYPE* t){
    TYPE* rt = resolveType(t);
    if (rt->kind == k_typeInt ||
        rt->kind == k_typeRune) return true;
    return false;
}

/*
* Function: Determines whether a type resolves to string
* 
* Args: 
*   TYPE *t: The type to determine whether it resolves to string
*
* Returns: bool
*/
bool resolveToStringBaseType(TYPE *t) {
    TYPE *rt = resolveType(t);
    if (rt->kind == k_typeString) return true;
    return false;
}

/*
* Function: Get string representation of a type
* 
* Args: 
*   TYPE *t: The type to get a string representation of
*
* Returns: char *
*/
char *typeToString(TYPE *t)
{
    if (t == NULL) throwInternalError("Null type in 'typeToString'");
    switch (t->kind)
    {
    case k_typeSlice:
        return "slice";
    case k_typeArray:
        return "array";
    case k_typeStruct:
        return "struct";
    case k_typeInfer:
        return "<infer>";
    case k_typeInt:
        return "int";
    case k_typeFloat:
        return "float64";
    case k_typeBool:
        return "bool";
    case k_typeRune:
        return "rune";
    case k_typeString:
        return "string";
    }
    exit(EXIT_FAILURE);
}

// =============================== TYPECHECK ===================================

void typePROG(PROG *root) {
	if (root == NULL) { return; }
	typeTOPLEVELDECL(root->rootTopLevelDecl);
}

void typeTOPLEVELDECL(TOPLEVELDECL *tld) {
    if (tld == NULL) return;
    typeTOPLEVELDECL(tld->next);

    switch (tld->kind) {
    case k_topLevelDeclFunc:
        typeFUNC(tld->val.funcDecl);
        break;
    case k_topLevelDeclType:
        // 2.2 Well-typed as per symbol table
        break;
    case k_topLevelDeclVar:
        typeVARSPEC(tld->val.varDecl);
        break;
    }
}

/* 2.3 A function declaration typechecks if the statements of its body type check
*/
void typeFUNC(FUNC *f) {
    typeSTMT(f->rootStmt, f->returnType);
}

/* 2.1
Case 1: var x T = expr
If expr is well-typed and its type is T1 and T1=T, the mapping x:T is 
added to the symbol table

Case 2: var x = expr
If expr is well-typed and its type is T, the mapping x:T is added to the 
symbol table.
*/
void typeVARSPEC(VARSPEC *varspec) {
    return;
}

// =============================== 3. STATEMENTS ===================================
void typeSTMT(STMT *s, TYPE *returnType) {
    if (s == NULL) return;
    typeSTMT(s->next, returnType);
    switch (s->kind) {
        // Trivially well-typed statements
        case k_stmtKindEmpty:
            break;
        case k_stmtKindBreak:
            break;
        case k_stmtKindContinue:
            break;
        // 3.3 Expression statement is well-typed if its expression child is well-typed
        // In GoLite, only function call expressions are allowed to be used as statements
        // ie: foo(x,y) but not x-1
        case k_stmtKindExpStmt:
            typeEXP(s->val.expStmt);
            break;
        // 3.4 return is well typed if the enclosing function has no return type
        // return expr is well-typed if its expression is well-typed
        // and the type of this expression is the same as the return type of the enclosing function
        case k_stmtKindReturn:
            if (s->val.expStmt == NULL) {
                if (returnType == NULL) return;
                throwError("Invalid return. Function has non-void return type.", s->lineno);
            }
            if (returnType == NULL) throwError("Invalid return. Function has void return type.", s->lineno);
            typeEXP(s->val.expStmt);
            if (!isEqualType(s->val.expStmt->type, returnType)) throwError("Invalid return statement. Type does not match function return type.", s->lineno);
            break;
        // An inc/dec statement type checks if its expression is well-typed
        // and resolves to a numeric base type (int, float64, rune)
        case k_stmtKindIncDec:
            typeEXP(s->val.incDecStmt.exp);
            if (!resolveToNumbericBaseType(s->val.incDecStmt.exp->type)) {
                throwError("Illegal increment/decerement statement. Operand must resolve to a numeric type.\n", s->lineno);
            }
            break;
        case k_stmtKindAssign:
            // TODO 3.5 Short declaration
            typeEXP(s->val.assignStmt.lhs);
            typeEXP(s->val.assignStmt.rhs);
            break;
        case k_stmtKindPrint:
            typeEXP(s->val.printStmt.expList);
            break;
        case k_stmtKindVarDecl:
            typeVARSPEC(s->val.varDecl);
            break;
        case k_stmtKindTypeDecl:
            break;
        case k_stmtKindBlock:
            typeSTMT(s->val.blockStmt, returnType);
            break;
        case k_stmtKindIfStmt:
            typeSTMT(s->val.ifStmt.simpleStmt, returnType);
            typeEXP(s->val.ifStmt.condition);
            typeSTMT(s->val.ifStmt.trueBody, returnType);
            typeSTMT(s->val.ifStmt.falseBody, returnType);
            break;
        case k_stmtKindSwitch:
            typeSTMT(s->val.switchStmt.simpleStmt, returnType);
            typeEXPRCASECLAUSE(s->val.switchStmt.caseClauses);
            break;
        case k_stmtKindFor:
            typeSTMT(s->val.forLoop.body, returnType);
            typeEXP(s->val.forLoop.condition);
            typeSTMT(s->val.forLoop.initStmt, returnType);
            typeSTMT(s->val.forLoop.postStmt, returnType);
            break;
    }
}

void typeEXPRCASECLAUSE(EXPRCASECLAUSE *caseClause) {
    return;
}

/*
* Function: Typecheck Expressions
* 
* Args: 
*   EXP *e: The expression to type check
*
* Returns: void
*/
void typeEXP(EXP *e) {
    if (e == NULL) return;
    typeEXP(e->next);

    TYPE *t;
    switch (e->kind) {

        // ============= LITERAL EXPRESSIONS ================
        case k_expKindIntLiteral:
        case k_expKindFloatLiteral:
        case k_expKindRuneLiteral:
        case k_expKindStringLiteral:
            break;

        // ============= IDENTIFIER EXPRESSION ================
        case k_expKindIdentifier:
            // Associate identifier with TYPE using SYMBOL
            if (e->val.identExp.symbol != NULL && e->val.identExp.symbol->val.varSpec->type != NULL) {
                e->type = e->val.identExp.symbol->val.varSpec->type;
            }
            break;

        // ============= UNARY EXPRESSIONS ================
        case k_expKindUPlus:
        case k_expKindUMinus:
            typeEXP(e->val.unary.rhs);
            if (!resolveToNumbericBaseType(e->val.unary.rhs->type)) {
                throwError("Illegal unary plus or negation expression. Operand must resolve to a numeric type.\n", e->lineno);
            }
            e->type = e->val.unary.rhs->type;
            break;

        case k_expKindBang:
            typeEXP(e->val.unary.rhs);
            if (!resolveToBoolBaseType(e->val.unary.rhs->type)) {
                throwError("Illegal logical negation expression. Operand must resolve to a bool type.\n", e->lineno);
            }
            e->type = e->val.unary.rhs->type;
            break;

        case k_expKindUBitXOR:
            typeEXP(e->val.unary.rhs);
            if (!resolveToIntegerBaseType(e->val.unary.rhs->type)) {
                throwError("Illegal bitwise negation expression. Operand must resolve to an integer type.\n", e->lineno);
            }
            e->type = e->val.unary.rhs->type;
            break;

        // ============= BINARY EXPRESSIONS ================
        case k_expKindAnd:      // &&
        case k_expKindOr:       // ||
            typeEXP(e->val.binary.lhs);
            typeEXP(e->val.binary.rhs);
            if (!resolveToBoolBaseType(e->val.binary.lhs->type) || !resolveToBoolBaseType(e->val.binary.rhs->type)) {
                throwError("Illegal binary expression. Operands must resolve to a bool type.\n", e->lineno);
            }
            e->type = makeTYPE(k_typeBool);
            break;

        // TODO: As per 4.4, what do expressions being "comparable/ordered" mean?

        case k_expKindEq:       // ==
        case k_expKindNotEq:    // !=
            typeEXP(e->val.binary.lhs);
            typeEXP(e->val.binary.rhs);

            // Must be comparable
            if (!isComparable(e->val.binary.lhs->type) && !isComparable(e->val.binary.rhs->type)) {
                throwError("Illegal binary comparison. Operands must resolve to comparable types.\n", 
                e->lineno);
            }

            // Must be equal types
            if (!isEqualType(e->val.binary.lhs->type, e->val.binary.rhs->type)) {
                throwError("Illegal binary comparison. Operands must resolve to same type.\n",
                e->lineno);
            }

            e->type = makeTYPE(k_typeBool);
            break;

        case k_expKindLess:     // <
        case k_expKindLessEq:   // <=    
        case k_expKindGrtr:     // >
        case k_expKindGrtrEq:   // >=
            // ordered op ordered -> bool
            typeEXP(e->val.binary.lhs);
            typeEXP(e->val.binary.rhs);

            // Must be ordered
            if (!isOrdered(e->val.binary.lhs->type) && !isOrdered(e->val.binary.rhs->type)) {
                throwError("Illegal binary comparison. Operands must resolve to ordered types.\n",
                e->lineno);
            }

            // Must be equal types
            if (!isEqualType(e->val.binary.lhs->type, e->val.binary.rhs->type)) {
                throwError("Illegal binary comparison. Operands must resolve to same type.\n",
                e->lineno);
            }

            e->type = makeTYPE(k_typeBool);
            break;

        case k_expKindAdd:      // +
            // (numeric or string + numeric or string) -> (numeric or string)
            typeEXP(e->val.binary.lhs);
            typeEXP(e->val.binary.rhs);

            // Both types must both either be numeric or string
            if (
                (!resolveToNumbericBaseType(e->val.binary.lhs->type) && 
                !resolveToNumbericBaseType(e->val.binary.rhs->type)) ||

                (!resolveToStringBaseType(e->val.binary.lhs->type) && 
                !resolveToStringBaseType(e->val.binary.rhs->type))) {
                    throwError("Illegal addition. Operands must both either resolve to numeric or string types", e->lineno);
                }

            // Both types must be the same
            if (!isEqualType(e->val.binary.lhs->type, e->val.binary.rhs->type)) {
                throwError("Illegal addition. Operands must resolve to same type.\n",
                e->lineno);
            }
            t = resolveType(e->val.binary.lhs->type);
            e->type = makeTYPE(t->kind);
            break;

        case k_expKindMinus:    // -
        case k_expKindMult:     // *
        case k_expKindDiv:      // /
            // numeric op numeric -> numeric
            typeEXP(e->val.binary.lhs);
            typeEXP(e->val.binary.rhs);

            // TODO finish
            break;

        case k_expKindMod:      // %

        case k_expKindBitOr:    // |
        case k_expKindBitAnd:   // &

        case k_expKindLeftShift:    // <<
        case k_expKindRightShift:   // >>

        case k_expKindBitClear:     // &^
        case k_expKindBitXOR:       // ^

        default:
            break;
    }
}
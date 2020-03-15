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

// ============================== 4. EXPRESSIONS ================================
void typeEXP(EXP *e) {
    if (e == NULL) return;
    typeEXP(e->next);
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
            if (!resolveToNumbericBaseType(e->type)) {
                throwError("Illegal unary plus or negation expression. Operand must resolve to a numeric type.\n", e->lineno);
            }
        case k_expKindBang:
            typeEXP(e->val.unary.rhs);
            if (!resolveToBoolBaseType(e->type)) {
                throwError("Illegal logical negation expression. Operand must resolve to a bool type.\n", e->lineno);
            }
        case k_expKindUBitXOR:
            typeEXP(e->val.unary.rhs);
            if (!resolveToIntegerBaseType(e->type)) {
                throwError("Illegal bitwise negation expression. Operand must resolve to an integer type.\n", e->lineno);
            }
        // ============= BINARY EXPRESSIONS ================
        case k_expKindAnd: // &&
        case k_expKindOr: // ||
            typeEXP(e->val.binary.lhs);
            typeEXP(e->val.binary.rhs);
            if (!resolveToBoolBaseType(e->val.binary.lhs->type) || !resolveToBoolBaseType(e->val.binary.rhs->type)) {
                throwError("Illegal binary expression. Operands must resolve to a bool type.\n", e->lineno);
            }
            e->type = makeTYPE(k_typeBool);
        // TODO: As per 4.4, what do expressions being "comparable/ordered" mean?
        case k_expKindEq: // ==
        case k_expKindNotEq: // !=
        default:
            break;
    }
}
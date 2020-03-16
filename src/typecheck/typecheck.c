#include "typecheck.h"
#include "../ast/tree.h"
#include "../ast/stmt.h"
#include "../error.h"
#include "symbol.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// ====================== HELPER FUNCTIONS =====================================

/*
* Function: Find the underlying type a defined type T given the set of types in scope T
* 
* Args: 
*   TYPE *t: The type to find the underlying type of
*
* Returns: bool
*/
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

/*
* Function: Determines whether two types are equal. Two types can only be equal if 
* they are exactly the same. If two separate types have the same parent base type, 
* they will still evaluate to different types.
* 
* Args: 
*   TYPE *t1: First type to compare with the second
*   TYPE *t2: Second type to compare with the first
*
* Returns: bool
*/
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
* Function: Determines whether a type is comparable. Comparable means it can be
* compared with the '==' and '!=' operators
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

/*
* Function: Determines whether a type is ordered. Ordered means it can be
* compared with the '> ,'>=', '<' and '<=' operators
* 
* Args: 
*   TYPE *t: The type to determine whether it's ordered
*
* Returns: bool
*/
bool isOrdered(TYPE *t) {
    if (t->kind == k_typeInt ||
        t->kind == k_typeBool ||
        t->kind == k_typeFloat ||
        t->kind == k_typeRune || 
        t->kind == k_typeString ) return true;
    return false;
}

/* Function: TODO FINISH. Specs: 3.7
*/
bool isAddressable(EXP *exp) {
    // TODO finish addressable
    switch (exp->type->kind)
    {
    case k_expKindIdentifier:
    case k_expKindArrayAccess:
    case k_expKindFieldAccess:
        return true;
    default:
        return false;
    }
    return false;
}

/*
* Function: Determines whether a type resolves to a base type. A type can resolve 
* to a base type of either int, float64, rune, string, or bool
* 
* Args: 
*   TYPE *t: The type to determine whether it resolves to a base type
*
* Returns: bool
*/
bool resolveToBaseType(TYPE *t){
    TYPE* rt = resolveType(t);
    if (rt->kind == k_typeInt ||
        rt->kind == k_typeFloat ||
        rt->kind == k_typeRune ||
        rt->kind == k_typeString ||
        rt->kind == k_typeBool) return true;
    return false;
}

/*
* Function: Determines whether a type resolves to a numeric type. A numeric type
* can resolve to a base type of either int, float64, or rune
* 
* Args: 
*   TYPE *t: The type to determine whether it resolves to a numeric type
*
* Returns: bool
*/
bool resolveToNumbericBaseType(TYPE *t){
    TYPE* rt = resolveType(t);
    if (rt->kind == k_typeInt ||
        rt->kind == k_typeFloat ||
        rt->kind == k_typeRune) return true;
    return false;
}

/*
* Function: Determines whether a type resolves to bool
* 
* Args: 
*   TYPE *t: The type to determine whether it resolves to bool
*
* Returns: bool
*/
bool resolveToBoolBaseType(TYPE *t){
    TYPE* rt = resolveType(t);
    if (rt->kind == k_typeBool) return true;
    return false;
}

/*
* Function: Determines whether a type resolves to int
* 
* Args: 
*   TYPE *t: The type to determine whether it resolves to int
*
* Returns: bool
*/
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

/*
* Function: Typecheck a variable declaration (denoted as a VARSPEC abstract-syntax-tree node)
* Follows the specifications from 2.1
* Case 1: var x T = expr
* If expr is well-typed and its type is T1 and T1=T, the mapping x:T is added to the symbol table
* 
* Case 2: var x = expr
* If expr is well-typed and its type is T, the mapping x:T is added to the symbol table.
* 
* Args: 
*   VARSPEC *vs: The variable declaration node to typecheck
*
* Returns: void
*/
void typeVARSPEC(VARSPEC *vs) {
    if (vs == NULL) return;
    typeVARSPEC(vs->next);

    if (vs->rhs != NULL) {
        typeEXP(vs->rhs);

        if (vs->type != NULL) {

            if (!isEqualType(vs->type, vs->rhs->type)) {

                throwError("Illegal variable declaration. Lhs and rhs types don't match.\n",
                vs->lineno);
            }
        }
        vs->type = vs->rhs->type;
    }
}


// =============================== 3. STATEMENTS ===================================

void typeSTMT_colonAssign(EXP *lhs, EXP *rhs)
{
    if (lhs == NULL && rhs == NULL) return;
    typeSTMT_colonAssign(lhs->next, rhs->next);
    typeEXP(rhs);
    lhs->type = rhs->type;
    if (lhs->kind == k_expKindIdentifier) {
        if (lhs->val.identExp.symbol != NULL && lhs->val.identExp.symbol->kind == k_symbolKindVar) {
            lhs->val.identExp.symbol->val.varSpec->type = rhs->type;
        }
    }
}

void typeSTMT_Assign(EXP *lhs, EXP *rhs, int lineno) {
    if (lhs == NULL && rhs == NULL) return;
    typeSTMT_Assign(lhs->next, rhs->next, lineno);
    typeEXP(lhs);
    typeEXP(rhs);
    // TODO (As per 3.7): Ensure expressions on LHS are lvalues (addressable):
    // Variables (non-constants), Slice indexing, Array indexing, Field selection
    if (!isEqualType(lhs->type, rhs->type)) {
        throwError("Illegal assignment. LHS and RHS types don't match.\n", lineno);
    }
}

void typeSTMT_opAssign(AssignKind op, EXP *v, EXP *expr) {
    // TODO 3.8 OP ASSIGNMENT v op expr
    typeEXP(v);
    typeEXP(expr);
    // op accepts two arguments of types typeof(v) and typeof(expr) 
    // and returns a value of typeof(v)
    // The expressions on the LHS must also be lvalues.
}

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
            if (s->val.assignStmt.kind == k_stmtColonAssign) {
                typeSTMT_colonAssign(s->val.assignStmt.lhs, s->val.assignStmt.rhs);
            } else if (s->val.assignStmt.kind == k_stmtAssign) {
                typeSTMT_Assign(s->val.assignStmt.lhs, s->val.assignStmt.rhs, s->lineno);
            } else {
                typeSTMT_opAssign(s->val.assignStmt.kind, s->val.assignStmt.lhs, s->val.assignStmt.rhs);
            }
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
            typeSTMT(s->val.forLoop.initStmt, returnType);
            typeEXP(s->val.forLoop.condition);
            typeSTMT(s->val.forLoop.body, returnType);
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
    SYMBOL *s;
    switch (e->kind) {

        // ============= LITERAL EXPRESSIONS ================
        case k_expKindIntLiteral:
        case k_expKindFloatLiteral:
        case k_expKindRuneLiteral:
        case k_expKindStringLiteral:
            break;

        // ============= IDENTIFIER EXPRESSION ================
        case k_expKindIdentifier:
            s = getSymbolFromExp(e);
            e->type = s->val.varSpec->type;
            break;

        // ============= UNARY EXPRESSIONS ================
        case k_expKindUParenthesized:
            typeEXP(e->val.unary.rhs);
            e->type = e->val.unary.rhs->type;
            break;
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
            if (!isComparable(e->val.binary.lhs->type) || !isComparable(e->val.binary.rhs->type)) {
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
            if (!isOrdered(e->val.binary.lhs->type) || !isOrdered(e->val.binary.rhs->type)) {
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
            if ( (!resolveToNumbericBaseType(e->val.binary.lhs->type) && !resolveToStringBaseType(e->val.binary.lhs->type)) 
                || (!resolveToNumbericBaseType(e->val.binary.rhs->type) && !resolveToStringBaseType(e->val.binary.rhs->type))) {
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

            // Both types must be numeric
            if (!resolveToNumbericBaseType(e->val.binary.lhs->type) || !resolveToNumbericBaseType(e->val.binary.rhs->type)) {
                throwError("Illegal arithmetic operation. Operands must both resolve to numeric types", e->lineno);
            }

            // Both types must be the same
            if (!isEqualType(e->val.binary.lhs->type, e->val.binary.rhs->type)) {
                throwError("Illegal addition. Operands must resolve to same type.\n",
                e->lineno);
            }

            e->type = e->val.binary.lhs->type;
            break;

        case k_expKindMod:          // %
        case k_expKindBitOr:        // |
        case k_expKindBitAnd:       // &
        case k_expKindBitClear:     // &^
        case k_expKindBitXOR:       // ^
        case k_expKindLeftShift:    // << *
        case k_expKindRightShift:   // >> * With shift operations. Go requires unsigned integers
            //                              on the left-hand side. GoLite simply allows signed int
            // int op int -> int
            typeEXP(e->val.binary.lhs);
            typeEXP(e->val.binary.rhs);

            // Both types must be int
            if (!resolveToNumbericBaseType(e->val.binary.lhs->type) || !resolveToNumbericBaseType(e->val.binary.rhs->type)) {
                throwError("Illegal arithmetic operation. Operands must both resolve to int", e->lineno);
            }

            // Both types must be the same
            if (!isEqualType(e->val.binary.lhs->type, e->val.binary.rhs->type)) {
                throwError("Illegal arithmetic operation. Operands must resolve to same type.",
                e->lineno);
            }

            e->type = e->val.binary.lhs->type;
            break;
        case k_expKindFuncCall:
            typeEXP(e->val.funcCall.primaryExpr);
            s = getSymbolFromExp(e->val.funcCall.primaryExpr);
            TYPESPEC *inputParams = s->val.funcSpec->inputParams;
            typeFUNCCALL(inputParams, e->val.funcCall.expList);
            e->type = s->val.funcSpec->returnType;
            break;
        case k_expKindArrayAccess:
            break;
        case k_expKindFieldAccess:
            break;
        case k_expKindAppend:
            break;
        case k_expKindLen:
            break;
        case k_expKindCap:
            break;
        case k_expKindCast:
            if (!resolveToBaseType(e->val.cast.type)) {
                throwError("Illegal typecast operation. Type must resolve to a base type", e->lineno);
            }
            typeEXP(e->val.cast.exp);
            if (resolveType(e->val.cast.type) == resolveType(e->val.cast.exp->type)) {
                e->type = e->val.cast.type;
            } else if (resolveToNumbericBaseType(e->val.cast.type) && resolveToNumbericBaseType(e->val.cast.exp->type)) {
                e->type = e->val.cast.type;
            } else if (resolveToStringBaseType(e->val.cast.type) && resolveToIntegerBaseType(e->val.cast.exp->type)) {
                e->type = e->val.cast.type;
            } else {
                throwError("Illegal typecast operation. The expr is not compatible with the type.", e->lineno);
            }
            break;
        default:
            break;
    }
}

SYMBOL *getSymbolFromExp(EXP *e) {
    switch (e->kind) {
        case k_expKindIdentifier:
            return e->val.identExp.symbol;
        case k_expKindUParenthesized:
            return getSymbolFromExp(e->val.unary.rhs);
        default:
            return NULL;
    }
}

void typeFUNCCALL(TYPESPEC *inputParams, EXP *args) {
    if (inputParams == NULL && args == NULL) return;
    if (inputParams == NULL || args == NULL) {
        throwError("Function called with incorrect number of arguments.", inputParams == NULL ? args->lineno : inputParams->lineno);
    }
    typeFUNCCALL(inputParams->next, args->next);
    typeEXP(args);
    if (!isEqualType(args->type, inputParams->type)) {
        throwError("Function called with incompatible argument types.", args->lineno);
    }
}
#include "typecheck.h"
#include "../ast/tree.h"
#include "../ast/stmt.h"
#include "../error.h"
#include "../weed.h"
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
    if (t== NULL) throwInternalError("Type is NULL in `resolveType`", 23);
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

    // Sometimes typeName is null, so we need to check if it isn't first
    if (t1->typeName != NULL && t2->typeName != NULL) 
    {
        if (strcmp(t1->typeName, t2->typeName) != 0)
        {
            return false;
        }
    }

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

            // Obviously, if two struct types don't have any fields, they're equal
            if (ss1 == NULL && ss2 == NULL) return true;

            IDENT *attribute1 = ss1->attribute;
            IDENT *attribute2 = ss2->attribute;

            // We need to iterate over all attributes in every struct spec
            // However, some structspecs contain multiple attributes whereas another equivalent one may
            // contain less. So we might need to iterate over one list of attributes in one struct type,
            // but iterate to the next structspec in the other struct type. Basically, equal attributes may
            // be in different struct specs, so we need to accomodate for that.
            while (attribute1 != NULL && attribute2 != NULL)
            {
                if (strcmp(attribute1->ident, attribute2->ident) != 0) return false;

                if (attribute1->next != NULL && attribute2->next != NULL)
                {
                    attribute1 = attribute1->next;
                    attribute2 = attribute2->next;
                }
                else if (attribute1->next != NULL && attribute2->next == NULL)
                {
                    attribute1 = attribute1->next;

                    ss2 = ss2->next;
                    if (ss2 != NULL) attribute2 = ss2->attribute;
                    else return false;
                }
                else if (attribute1->next == NULL && attribute2->next != NULL)
                {
                    ss1 = ss1->next;
                    if (ss1 != NULL) attribute1 = ss1->attribute;
                    else return false;

                    attribute2 = attribute2->next;
                }
                else
                {
                    ss1 = ss1->next;
                    ss2 = ss2->next;
                    if ((ss1 != NULL && ss2 == NULL) || (ss1 == NULL && ss2 != NULL))
                    {
                        return false;
                    }
                    else if (ss1 != NULL && ss2 != NULL)
                    {
                        if (!isEqualType(ss1->type, ss2->type)) return false;
                        attribute1 = ss1->attribute;
                        attribute2 = ss2->attribute;
                    }
                    else 
                    {   
                        // This means we've checked all attributes and struct specs, in which no combination   
                        // returned false, so these two struct types must be equal
                        return true;
                    }
                }
            }
            break;
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

    switch (t->kind)
    {
    case k_typeInt:
    case k_typeBool:
    case k_typeFloat:
    case k_typeRune:
    case k_typeString:
        return true;

    // Array type is comparable if values of array element are comparable
    case k_typeArray:
        // fprintf(stdout, "HERE\n");
        return isComparable(t->val.arrayType.type);

    // Struct values are comparable if all their fields are comparable
    case k_typeStruct:
        for (STRUCTSPEC *ss = t->val.structType; ss; ss = ss->next) {
            if (!isComparable(ss->type)) return false;
        }
        return true;
        
    case k_typeSlice:
        return false;
    case k_typeInfer:
        return isComparable(resolveType(t));
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
        t->kind == k_typeFloat ||
        t->kind == k_typeRune || 
        t->kind == k_typeString ) return true;
    else if (t->kind == k_typeInfer) {
        return isOrdered(resolveType(t));
    }
    return false;
}

/* Function: TODO FINISH. Specs: 3.7
*/
bool isAddressable(EXP *exp) {
    // TODO finish addressable
    switch (exp->kind)
    {
    case k_expKindIdentifier:
        if ((strcmp(exp->val.identExp.ident, (char *) "false") == 0)  ||
            (strcmp(exp->val.identExp.ident, (char *) "true") == 0)) {
                return false;
            }
        return true;
        break;
    case k_expKindArrayAccess:
        return isAddressable(exp->val.arrayAccess.arrayReference);
    case k_expKindFieldAccess:
        return isAddressable(exp->val.fieldAccess.object);
    case k_expKindUParenthesized:
        return isAddressable(exp->val.unary.rhs);
    case k_expKindFuncCall: ;
        FUNC *funcSpec = exp->val.funcCall.primaryExpr->val.identExp.symbol->val.funcSpec;
        if (funcSpec->returnType == NULL) return false;
        switch (funcSpec->returnType->kind)
        {
        case k_typeSlice:
            // The only function call expression that is addressable is one that returns a slice
            return true;
        
        default:
            return false;
        }
        //break
        
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

bool resolvesToSliceOrArray(TYPE *t) {
    TYPE *rt = resolveType(t);
    if (rt->kind == k_typeSlice || 
        rt->kind == k_typeArray) return true;
    return false;
}

bool resolvesToSlice(TYPE *t) {
    TYPE *rt = resolveType(t);
    if (rt->kind == k_typeSlice) return true;
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
* Function: Determines whether a type resolves to struct
* 
* Args: 
*   TYPE *t: The type to determine whether it resolves to struct
*
* Returns: bool
*/
bool resolveToStructBaseType(TYPE *t)
{
    TYPE *rt = resolveType(t);
    if (rt->kind == k_typeStruct) return true;
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
    if (t == NULL) throwInternalError("Null type in 'typeToString'", 0);
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
        if (vs->rhs->type == NULL) {
            throwError("Void cannot be used as a value in variable declaration.", vs->lineno);
        }
        if (vs->type != NULL) {
            if (!isEqualType(vs->type, vs->rhs->type)) {
                throwError("Illegal variable declaration. LHS and RHS types don't match.\n",
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
    if (rhs->type == NULL) 
    {
        throwError("Void cannot be used as a value in short declaration.", lhs->lineno);
    }
    if (!isBlankId(lhs->val.identExp.ident)) 
    {
        typeEXP(lhs);

        if (lhs->type->kind != k_typeInfer && !isEqualType(lhs->type, rhs->type)) 
        {
            throwError("Illegal variable declaration. LHS and RHS types don't match.\n",
            lhs->lineno);
        }

        if (lhs->val.identExp.symbol->kind != k_symbolKindVar) 
        {
            fprintf(stderr, "Error: (line %d) type %s is not an expression", lhs->lineno, lhs->val.identExp.ident);
            exit(EXIT_FAILURE);
        }
    }

    lhs->type = rhs->type;
    if (lhs->kind == k_expKindIdentifier) 
    {
        if (lhs->val.identExp.symbol != NULL && lhs->val.identExp.symbol->kind == k_symbolKindVar) 
        {
            lhs->val.identExp.symbol->val.varSpec->type = rhs->type;
        }
        
        if (rhs->kind == k_expKindIdentifier)
        {
            if (rhs->val.identExp.symbol->kind == k_symbolKindType)
            {
                fprintf(stderr, "Error: (line %d) type %s is not an expression", rhs->lineno, rhs->val.identExp.ident);
                exit(EXIT_FAILURE);
            }
        }
    }
}

void typeSTMT_Assign(EXP *lhs, EXP *rhs, int lineno) {
    if (lhs == NULL && rhs == NULL) return;
    typeSTMT_Assign(lhs->next, rhs->next, lineno);

    typeEXP(lhs);
    typeEXP(rhs);
    if (rhs->type == NULL) {
        throwError("Void cannot be used as a value in an assignment declaration.", lhs->lineno);
    }

    if (lhs->kind == k_expKindIdentifier)
    {
        // We do nothing if it's a blank identifier
        if (isBlankId(lhs->val.identExp.ident)) return;
    }

    // TODO (As per 3.7): Ensure expressions on LHS are lvalues (addressable):
    // Variables (non-constants), Slice indexing, Array indexing, Field selection
    if (!isAddressable(lhs)) {
        throwError("Illegal assignment. LHS must be addressable.\n", lineno);
    }

    if ((lhs->type->typeName == NULL && rhs->type->typeName != NULL) ||
        (lhs->type->typeName != NULL && rhs->type->typeName == NULL)) {
            throwError("Illegal assignment. LHS and RHS types don't match.\n", lineno);
        }

    if (!isEqualType(lhs->type, rhs->type)) {
            // NOTE: This is a total hacky workaround for the struct alias bug 
            // as demonstrated in solutions/2-typecheck~parse/valid/4-7_select_expr.go
            // It may cause errors in code generation!!
            if (!(lhs->type->kind == k_typeStruct && rhs->type->kind == k_typeInfer))
            {
                throwError("Illegal assignment. LHS and RHS types don't match.\n", lineno);
            }
    }
}

void typeSTMT_opAssign(AssignKind op, EXP *v, EXP *expr) {
    // TODO 3.8 OP ASSIGNMENT v op expr
    typeEXP(v);
    typeEXP(expr);
    if (expr->type == NULL) {
        throwError("Void cannot be used as a value in operator assignment declaration.", expr->lineno);
    }
    // op accepts two arguments of types typeof(v) and typeof(expr) 
    // and returns a value of typeof(v)
    // The expressions on the LHS must also be lvalues.
    
    // This never occur, since non-addressable expressions on the LHS are
    // caught by the weeder!
    if (!isAddressable(v)) {
        throwError("Illegal assignment. LHS must be addressable.", v->lineno);
    }

    if ((v->type->typeName == NULL && expr->type->typeName != NULL) ||
        (v->type->typeName != NULL && expr->type->typeName == NULL)) {
            throwError("Illegal assignment. LHS and RHS types don't match.\n", expr->lineno);
        }

    if (!isEqualType(v->type, expr->type)) {
        throwError("Illegal assignment. LHS and RHS types don't match.", expr->lineno);
    }

    switch(op) {
        case k_opAssignKindPlusEq:
            if (!resolveToStringBaseType(v->type) && !resolveToNumbericBaseType(v->type)) {
                throwError("Incompatible type in assign op += [expected string or numeric]", expr->lineno);
            }
            break;
        case k_opAssignKindMinusEq:
            if (!resolveToNumbericBaseType(v->type)) {
                throwError("Incompatible type in assign op -= [expected numeric]", expr->lineno);
            }
            break;
        case k_opAssignKindMultEq:
            if (!resolveToNumbericBaseType(v->type)) {
                throwError("Incompatible type in assign op *= [expected numeric]", expr->lineno);
            }
            break;
        case k_opAssignKindDivEq:
            if (!resolveToNumbericBaseType(v->type)) {
                throwError("Incompatible type in assign op /= [expected numeric]", expr->lineno);
            }
            break;
        case k_opAssignKindModEq:       
            if (!resolveToIntegerBaseType(v->type)) {
                throwError("Incompatible type in assign op %= [expected integer or rune]", expr->lineno);
            }
            break;
        case k_opAssignKindBitAndEq:
            if (!resolveToIntegerBaseType(v->type)) {
                throwError("Incompatible type in assign op &= [expected integer or rune]", expr->lineno);
            }
            break;
        case k_opAssignKindBitOrEq:
            if (!resolveToIntegerBaseType(v->type)) {
                throwError("Incompatible type in assign op |= [expected integer or rune]", expr->lineno);
            }
            break;
        case k_opAssignKindBitXorEq:
            if (!resolveToIntegerBaseType(v->type)) {
                throwError("Incompatible type in assign op ^= [expected integer or rune]", expr->lineno);
            }
            break;
        case k_opAssignKindLeftShiftEq:
            if (!resolveToIntegerBaseType(v->type)) {
                throwError("Incompatible type in assign op <<= [expected integer or rune]", expr->lineno);
            }
            break;
        case k_opAssignKindRightShiftEq:
            if (!resolveToIntegerBaseType(v->type)) {
                throwError("Incompatible type in assign op >>= [expected integer or rune]", expr->lineno);
            }
            break;
        case k_opAssignKindBitClearEq:
            if (!resolveToIntegerBaseType(v->type)) {
                throwError("Incompatible type in assign op &^= [expected integer or rune]", expr->lineno);
            }
            break;
        default:
            break;
    }
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
            if (s->val.expStmt->kind == k_expKindCast) throwError("Type conversion evaluated but not used", s->lineno);
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
                throwError("Illegal increment/decrement statement. Operand must resolve to a numeric type.\n", s->lineno);
            }
            if (!isAddressable(s->val.incDecStmt.exp)) throwError("Expected an assignable expression in increment/decrement statement", s->lineno);
            break;
        case k_stmtKindAssign:
            if (s->val.assignStmt.kind == k_stmtColonAssign) 
            {
                typeSTMT_colonAssign(s->val.assignStmt.lhs, s->val.assignStmt.rhs);
            } 
            else if (s->val.assignStmt.kind == k_stmtAssign) 
            {
                typeSTMT_Assign(s->val.assignStmt.lhs, s->val.assignStmt.rhs, s->lineno);
            } 
            else 
            {
                typeSTMT_opAssign(s->val.assignStmt.kind, s->val.assignStmt.lhs, s->val.assignStmt.rhs);
            }
            break;
        case k_stmtKindPrint:
            typeEXP(s->val.printStmt.expList);
            EXP *printExp = s->val.printStmt.expList;
            while (printExp != NULL) {

                if (printExp->type == NULL) throwError("Cannot print out symbol of type void", s->lineno);
                if (printExp->kind == k_expKindIdentifier)
                {
                    if (printExp->val.identExp.symbol->kind == k_symbolKindType) throwError("Expecting expression that evalutes to a value, not a type", s->lineno);
                }
                if (!resolveToBaseType(printExp->type)) {
                    throwError("Expressions in a print statement must resolve to a base type.", s->lineno);
                }
                printExp = printExp->next;
            }
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
            if (s->val.ifStmt.condition != NULL) {
                if (!resolveToBoolBaseType(s->val.ifStmt.condition->type)) {
                    throwError("The condition expression of a for loop must resolve to type bool.", s->lineno);
                }
            }
            break;
        case k_stmtKindSwitch:
            // init simpleStmt type checks
            typeSTMT(s->val.switchStmt.simpleStmt, returnType);
            typeEXP(s->val.switchStmt.exp);
            TYPE *switchExprType;
            if (s->val.switchStmt.exp != NULL) {
                if (s->val.switchStmt.exp->kind == k_expKindFuncCall)
                {
                    if (s->val.switchStmt.exp->val.funcCall.primaryExpr->val.identExp.symbol->val.funcSpec->returnType == NULL) throwError("Function returning void used as value", s->lineno);
                }
                switchExprType = s->val.switchStmt.exp->type;
            } else {
                switchExprType = makeTYPE(k_typeBool);
            }
            // switch WITH an expression type
            // expr is well-typed and is a comparable type
            // the expressions e1, e2,..., en are well-typed
            // and have the same type as expr
            // the statements under the different alternatives type check
            typeEXPRCASECLAUSE(s->val.switchStmt.caseClauses, returnType, switchExprType);
            // switch WITHOUT an expression type
            // e1, e2, .., en are well typed and have type bool
            // all statements type check
            break;
        case k_stmtKindFor:
            typeSTMT(s->val.forLoop.initStmt, returnType);
            typeEXP(s->val.forLoop.condition);
            typeSTMT(s->val.forLoop.body, returnType);
            typeSTMT(s->val.forLoop.postStmt, returnType);
            if (s->val.forLoop.condition != NULL) {
                if (!resolveToBoolBaseType(s->val.forLoop.condition->type)) {
                    throwError("The condition expression of a for loop must resolve to type bool.", s->lineno);
                }
            }
            break;
    }
}

void typeEXPRCASECLAUSE(EXPRCASECLAUSE *caseClause, TYPE *returnType, TYPE *expType) {
    if (caseClause == NULL) return;
    typeEXPRCASECLAUSE(caseClause->next, returnType, expType);
    typeSTMT(caseClause->stmtList, returnType);
    typeEXP(caseClause->expList);
    if (caseClause->kind == k_defaultClause) return;
    EXP *currentExp = caseClause->expList;
    while (currentExp != NULL) {
        if (!isEqualType(currentExp->type, expType)) 
        {
            throwError("Switch statement clauses must match switch statement expression.", caseClause->lineno);
        }
        if (currentExp->type->kind == k_typeSlice) 
        {
            throwError("Invalid case in switch (can only compare slice to nil)", currentExp->lineno);
        }
        currentExp = currentExp->next;
    }
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

    SYMBOL *s;
    switch (e->kind) {

        // ============= LITERAL EXPRESSIONS ================
        case k_expKindIntLiteral:
            e->type = makeTYPE(k_typeInt);
            break;
        case k_expKindFloatLiteral:
            e->type = makeTYPE(k_typeFloat);
            break;
        case k_expKindRuneLiteral:
            e->type = makeTYPE(k_typeRune);
            break;
        case k_expKindStringLiteral:
            e->type = makeTYPE(k_typeString);
            break;

        // ============= IDENTIFIER EXPRESSION ================
        case k_expKindIdentifier:
            s = getSymbolFromExp(e);
            if (s == NULL) break;
            switch (s->kind)
            {
            case k_symbolKindVar:
            case k_symbolKindConstant:
                e->type = s->val.varSpec->type;
                break;
            case k_symbolKindType:
                e->type = s->val.type;
                break;

            case k_symbolKindFunc:
                e->type = s->val.funcSpec->returnType;
                break;
            }
            // e->type = s->val.varSpec->type;
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

            if (!isEqualType(e->val.binary.lhs->type, e->val.binary.rhs->type)) throwError("Illegal binary expression. Operands must be of the same type", e->lineno);

            if (!resolveToBoolBaseType(e->val.binary.lhs->type) || !resolveToBoolBaseType(e->val.binary.rhs->type)) {
                throwError("Illegal binary expression. Operands must resolve to a bool type.\n", e->lineno);
            }
            e->type = e->val.binary.lhs->type;
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

            e->type = e->val.binary.lhs->type;
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

        case k_expKindArrayAccess: ;
            EXP *arrayReference = e->val.arrayAccess.arrayReference;
            typeEXP(arrayReference);
            typeEXP(e->val.arrayAccess.indexExp);
            TYPE* rtIndex = resolveType(e->val.arrayAccess.indexExp->type);
            TYPE* rtExpr = resolveType(arrayReference->type);
            if (rtIndex->kind != k_typeInt) {
                throwError("The index type of expression indexing must be of type int.", e->lineno);
            }
            if (rtExpr->kind != k_typeSlice && rtExpr->kind != k_typeArray) {
                throwError("The expression being indexed to must be a slice or array.", e->lineno);
            }
            if (rtExpr->kind == k_typeSlice) {
                TYPE *sliceType = resolveType(arrayReference->type);
                e->type = sliceType->val.sliceType.type;
            } else if (rtExpr->kind == k_typeArray) {
                TYPE *arrayType = resolveType(arrayReference->type);
                e->type = arrayType->val.arrayType.type;
            }
            if (e->type->kind == k_typeInfer && e->type->symbol) {
                e->type = e->type->symbol->val.type;
            }
            break;

        case k_expKindFieldAccess:
            typeEXP(e->val.fieldAccess.object);

            SYMBOL *s = getSymbolFromExp(e->val.fieldAccess.object);
            if (s != NULL)
            {
                if (s->kind != k_symbolKindVar) throwError("Illegal field access. Object must be of a variable of struct type", e->lineno);
            }

            if (!resolveToStructBaseType(e->val.fieldAccess.object->type)) {
                throwError("Illegal field access. Expecting a struct type", e->lineno);
            }
            
            for (STRUCTSPEC *ss = e->val.fieldAccess.object->type->val.structType; ss; ss = ss->next) {

                TYPE *parentType = e->val.fieldAccess.object->type;
                if (parentType->kind == k_typeInfer)
                {
                    // The structspec may be an alias for another one, so we need to fetch it
                    while(parentType->kind == k_typeInfer) parentType = parentType->parent;
                    ss = parentType->val.structType;
                }

                for (IDENT *ident = ss->attribute; ident; ident = ident->next) {
                    if (strcmp(ident->ident, e->val.fieldAccess.field) == 0) {
                        e->type = ss->type;
                        return;
                    }
                }
            }

            // Otherwise no field-identifier match
            throwError("Illegal field access. No such field exists", e->lineno);
            break;

        case k_expKindAppend:
            typeEXP(e->val.append.slice);
            typeEXP(e->val.append.addend);
            TYPE *sliceRt = resolveType(e->val.append.slice->type);
            if (sliceRt->kind != k_typeSlice) {
                throwError("The first expression in an append call must resolve to a slice type.", e->lineno);
            }
            if (!isEqualType(sliceRt->val.sliceType.type, e->val.append.addend->type)) {
                throwError("The expression types in this append call do not match.", e->lineno);
            }
            e->type = e->val.append.slice->type;
            break;
        case k_expKindLen:
            typeEXP(e->val.lenExp);
            if (!resolvesToSliceOrArray(e->val.lenExp->type) && !resolveToStringBaseType(e->val.lenExp->type)) {
                throwError("The expression in a len call must be a string, slice, or array.", e->lineno);
            }
            e->type->kind = k_typeInt;
            break;
        case k_expKindCap:
            typeEXP(e->val.capExp);
            if (!resolvesToSliceOrArray(e->val.lenExp->type)) {
                throwError("The expression in a cap call must be a slice or array.", e->lineno);
            }
            e->type->kind = k_typeInt;
            break;
        case k_expKindCast:
            if (e->val.cast.exp == NULL) throwError("Missing argument in type cast", e->lineno);
            if (e->val.cast.exp->next != NULL) throwError("Too many arguments in type cast", e->lineno);
            if (!resolveToBaseType(e->val.cast.type)) {
                throwError("Illegal typecast operation. Type must resolve to a base type", e->lineno);
            }
            typeEXP(e->val.cast.exp);
            if (isEqualType(resolveType(e->val.cast.type), resolveType(e->val.cast.exp->type))) {
                e->type = e->val.cast.type;
            } else if (resolveToBoolBaseType(e->val.cast.type) && resolveToBoolBaseType(e->val.cast.exp->type)) {
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

        case k_expKindUPlus:
        case k_expKindUMinus:
        case k_expKindBang:
        case k_expKindUBitXOR:
        case k_expKindUParenthesized:
            return getSymbolFromExp(e->val.unary.rhs);

        case k_expKindArrayAccess:
            return getSymbolFromExp(e->val.arrayAccess.arrayReference);

        case k_expKindFuncCall:
            return getSymbolFromExp(e->val.funcCall.primaryExpr);

        default:
            return NULL;
    }
}

void typeFUNCCALL(TYPESPEC *inputParams, EXP *args) {
    if (inputParams == NULL && args == NULL) return;

    int paramCount = 0;
    int argsCount = 0;

    TYPESPEC *currParams = inputParams;
    EXP *currArgs = args;

    while (currParams != NULL && currArgs != NULL)
    {
        IDENT *id = currParams->ident;
        while (id != NULL && currArgs != NULL) 
        {
            typeEXP(currArgs);
            if (!isEqualType(currParams->type, currArgs->type)) {
                throwError("Function called with incompatible argument types.", args->lineno);
            }

            paramCount++;
            id = id->next;
            argsCount++;
            currArgs = currArgs->next;
        }
        currParams = currParams->next;
    }
    while (currArgs != NULL)
    {
        argsCount++;
        currArgs = currArgs->next;
    }
    while(currParams != NULL)
    {
        paramCount++;
        currParams = currParams->next;
    }
    
    if (paramCount != argsCount) throwError("Function called with incorrect number of arguments.", inputParams == NULL ? args->lineno : inputParams->lineno);
}

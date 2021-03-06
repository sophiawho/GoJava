#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "error.h"
#include "weed.h"

void weedPROG(PROG *p)
{
    if (p != NULL)
    {
        char *name = p->package;
        if (isBlankId(name)) throwError("cannot declare _ as a package identifier", 1);
        weedTOPLEVELDECL(p->rootTopLevelDecl);
    }
    return;
}

void weedTOPLEVELDECL(TOPLEVELDECL *d)
{
    int countLhs = 0;
    int countRhs = 0;
    if (d != NULL)
    {
        weedTOPLEVELDECL(d->next);

        switch (d->kind)
        {
        case k_topLevelDeclVar:
            if (d->val.varDecl != NULL && d->val.varDecl->rhs != NULL) 
            {
                countLhs = countIDENT(d->val.varDecl->ident);
                countRhs = countEXP(d->val.varDecl->rhs);
                if (countLhs != countRhs) throwError("expecting same number of identifiers as expressions", d->lineno);
                weedEXP_eval(d->val.varDecl->rhs);
            }
            break;
        
        case k_topLevelDeclType:
            weedTYPESPEC(d->val.typeDecl);
            break;
        
        case k_topLevelDeclFunc:
            weedFUNC(d->val.funcDecl);
            break;
        }
    }
    return;
}

bool containsBreak(STMT *s) 
{
    while (s != NULL) 
    {
        if (s->kind == k_stmtKindBreak) {
            printf("break\n");
            return true;
        }
        if (s->kind == k_stmtKindIfStmt) {
            if (containsBreak(s->val.ifStmt.trueBody->val.blockStmt) || containsBreak(s->val.ifStmt.falseBody->val.blockStmt)) {
                return true;
            }
        }
        s = s->next;
    }
    return false;
}

bool isTerminatingStmt(STMT *s)
{
    while(s != NULL) 
    {
        if (s->kind == k_stmtKindBlock) return isTerminatingStmt(s->val.blockStmt);
        if (s->kind == k_stmtKindReturn) return true;
        if (s->kind == k_stmtKindIfStmt && s->val.ifStmt.falseBody != NULL) {
            if (isTerminatingStmt(s->val.ifStmt.trueBody) && isTerminatingStmt(s->val.ifStmt.falseBody)) {	
                return true;
            }
        }
        if (s->kind == k_stmtKindFor && s->val.forLoop.condition == NULL) {
            if (!containsBreak(s->val.forLoop.body->val.blockStmt)) {
                return true;
            } else {
                throwError("Function does not have a terminating statement [loop cannot break]", s->lineno);
            }
        }
        if (s->kind == k_stmtKindSwitch) {
            bool containsDefault = false;
            for (EXPRCASECLAUSE *ecc = s->val.switchStmt.caseClauses; ecc; ecc=ecc->next) {
                if (ecc->kind == k_defaultClause) containsDefault = true;
                if (containsBreak(ecc->stmtList)) {
                    throwError("Function does not have a terimnating statement [switch clause cannot break", s->lineno);
                } else if (!isTerminatingStmt(ecc->stmtList)) {
                    throwError("Function does not have a terminating function [switch clause statements must each end in a terminating statement", s->lineno);
                }
            }
            if (containsDefault) return true;
        }
        s = s->next;
    }
    return false;
}

void weedFUNC(FUNC *f)
{
    if (f != NULL) {
        if (f->returnType != NULL) {
            if (!isTerminatingStmt(f->rootStmt->val.blockStmt)) {
                throwError("Function does not have a terminating statement", f->lineno);
            }
        }
        weedSTMT(f->rootStmt);
    }
}

int forLoopDepth = 0;
int switchDepth = 0;

void weedSTMT(STMT *s)
{
    int countLhs = 0;
    int countRhs = 0;
    if (s != NULL) 
    {
        // Remember: The AST is built from tail to head! So the first statement being weeded is actually 
        // the last statement within the given block of code. So if the next statement is a return statement
        // that means there's at least one unreachable line of code
        if (s->next != NULL)
        {
            if (s->next->kind == k_stmtKindReturn) throwError("Unreachable code", s->lineno);
        }

        switch (s->kind)
        {
        case k_stmtKindExpStmt:
            if (!(s->val.expStmt->kind == k_expKindFuncCall)) throwError("expression statements can only be function calls", s->lineno);
            weedEXP_eval(s->val.expStmt);
            break;
            
        case k_stmtKindIncDec:
            weedEXP_eval(s->val.incDecStmt.exp);
            break;

        case k_stmtKindAssign:
            weedSTMT_assign(s);
            break;

        case k_stmtKindPrint:
            weedEXP_eval(s->val.printStmt.expList);
            break;

        case k_stmtKindVarDecl:
            if (s->val.varDecl != NULL && s->val.varDecl->rhs != NULL) 
            {
                countLhs = countIDENT(s->val.varDecl->ident);
                countRhs = countEXP(s->val.varDecl->rhs);
                if (countLhs != countRhs) throwError("expecting same number of identifiers as expressions", s->lineno);
                weedEXP_eval(s->val.varDecl->rhs);
            }
            break;

        case k_stmtKindTypeDecl:
            weedTYPESPEC(s->val.typeDecl);
            break;

        case k_stmtKindBlock:
            weedSTMT(s->val.blockStmt);
            break;

        case k_stmtKindIfStmt:
            weedSTMT(s->val.ifStmt.simpleStmt);
            weedEXP_eval(s->val.ifStmt.condition);
            weedSTMT(s->val.ifStmt.trueBody);
            weedSTMT(s->val.ifStmt.falseBody);
            break;

        case k_stmtKindSwitch:
            switchDepth++;
            weedEXP_eval(s->val.switchStmt.exp);
            weedSTMT(s->val.switchStmt.simpleStmt);
            weedEXPRCASECLAUSE_findDefaultCase(s->val.switchStmt.caseClauses, false);
            weedEXPRCASECLAUSE(s->val.switchStmt.caseClauses);
            switchDepth--;
            break;

        case k_stmtKindFor:
            forLoopDepth++;
            weedSTMT_forLoop(s);
            forLoopDepth--;
            break;

        case k_stmtKindBreak:
            if (forLoopDepth <= 0 && switchDepth <= 0) throwError("expecting for-loop or switch enclosing around break statement", s->lineno);
            break;

        case k_stmtKindContinue:
            if (forLoopDepth <= 0) throwError("expecting for-loop enclosing around continue statement", s->lineno);
            break;

        case k_stmtKindReturn:
            weedEXP_eval(s->val.returnExp);
            break;

        case k_stmtKindEmpty:
            break;

        }
        weedSTMT(s->next);
    }
    return;
}

void weedTYPESPEC(TYPESPEC *tsRoot)
{
    TYPESPEC *ts = tsRoot;
    while (ts != NULL)
    {
        weedTYPE(ts->type);
        ts = ts->next;
    }
    return;
}

void weedEXP_colonAssign(EXP *e)
{
    switch (e->kind)
    {
    case k_expKindUParenthesized:
    case k_expKindArrayAccess:
    case k_expKindFieldAccess:
        throwError("non-name on left side of :=", e->lineno);
        break;
    case k_expKindIdentifier:
        break;
    default:
        throwError("only identifiers allowed on left side of :=", e->lineno);
        break;
    }
}

void weedSTMT_assign(STMT *s)
{
    bool allowBlankId;
    int countLhs = 0;
    int countRhs = 0;
    switch (s->val.assignStmt.kind)
    {
    // In assignment statements, both the left- and right-hand expression lists must be of 
    // the same number of tokens
    case k_stmtColonAssign:
        allowBlankId = true;
        weedEXP_colonAssign(s->val.assignStmt.lhs);
        weedEXP_nonEval(s->val.assignStmt.lhs, allowBlankId);
        weedEXP_eval(s->val.assignStmt.rhs);
        countLhs = countEXP(s->val.assignStmt.lhs);
        countRhs = countEXP(s->val.assignStmt.rhs);
        if (countLhs != countRhs) throwError("expecting same number of identifiers as expressions", s->lineno);
        break;
    case k_stmtAssign:
        allowBlankId = true;
        weedEXP_nonEval(s->val.assignStmt.lhs, allowBlankId);
        weedEXP_eval(s->val.assignStmt.rhs);
        countLhs = countEXP(s->val.assignStmt.lhs);
        countRhs = countEXP(s->val.assignStmt.rhs);
        if (countLhs != countRhs) throwError("expecting same number of identifiers as expressions", s->lineno);
        break;

    // In assignment operations, both the left- and right-hand expression lists must contain 
    // exactly one single-valued expression, and the left-hand expression must not be the 
    // blank identifier. 
    case k_opAssignKindPlusEq:
    case k_opAssignKindMinusEq:
    case k_opAssignKindMultEq:
    case k_opAssignKindDivEq:
    case k_opAssignKindModEq:
    case k_opAssignKindBitAndEq:
    case k_opAssignKindBitOrEq:
    case k_opAssignKindBitXorEq:
    case k_opAssignKindLeftShiftEq:
    case k_opAssignKindRightShiftEq:
    case k_opAssignKindBitClearEq:
        allowBlankId = false;
        weedEXP_nonEval(s->val.assignStmt.lhs, allowBlankId);
        weedEXP_eval(s->val.assignStmt.rhs);
        countLhs = countEXP(s->val.assignStmt.lhs);
        countRhs = countEXP(s->val.assignStmt.rhs);
        if (countLhs != 1 || countRhs != 1) throwError("expecting exactly one identifier and one expression", s->lineno);
        break;

    }
    return;
}

void weedSTMT_forLoop(STMT *s) 
{
    switch (s->val.forLoop.kind)
    {
    case k_loopKindInfinite:
        weedSTMT(s->val.forLoop.body);
        break;

    case k_loopKindWhile:
        weedSTMT(s->val.forLoop.body);
        weedEXP_eval(s->val.forLoop.condition);
        break;

    case k_loopKindThreePart:
        weedSTMT(s->val.forLoop.initStmt);
        weedEXP_eval(s->val.forLoop.condition);
        if (s->val.forLoop.postStmt != NULL) weedSTMT_forLoop_postStmt(s->val.forLoop.postStmt);
        weedSTMT(s->val.forLoop.postStmt);    
        weedSTMT(s->val.forLoop.body);
        break;
    }
    return;
}

void weedSTMT_forLoop_postStmt(STMT *s)
{
    switch (s->kind)
    {
    case k_stmtKindAssign:
        if (s->val.assignStmt.kind == k_stmtColonAssign) throwError("cannot declare in post statement of for loop", s->lineno);
        break;

    case k_stmtKindVarDecl:
    case k_stmtKindTypeDecl:
    case k_stmtKindBlock:
    case k_stmtKindIfStmt:
    case k_stmtKindSwitch:
    case k_stmtKindFor:
    case k_stmtKindBreak:
    case k_stmtKindContinue:
    case k_stmtKindReturn:
        throwError("expecting expression in post statement of for loop", s->lineno);
        break;

    default:
        break;
    }
}

void weedEXPRCASECLAUSE(EXPRCASECLAUSE *cc)
{
    if (cc != NULL)
    {
        switch (cc->kind)
        {
        case k_caseClause:
            weedEXP_eval(cc->expList);
            weedSTMT(cc->stmtList);
            break;
        
        case k_defaultClause:
            weedSTMT(cc->stmtList);
            break;
        }
        weedEXPRCASECLAUSE(cc->next);
    }
    return;
}

void weedEXPRCASECLAUSE_findDefaultCase(EXPRCASECLAUSE *cc, bool foundPreviousDefaultCase)
{
    bool foundDefaultCase = false;
    if (cc != NULL)
    {
        if (cc->kind == k_defaultClause) foundDefaultCase = true;
        if (foundPreviousDefaultCase && foundDefaultCase) throwError("expected up to one default case in switch statement", cc->lineno);

        weedEXPRCASECLAUSE_findDefaultCase(cc->next, foundDefaultCase);
    }
    return;
}

void weedEXP_eval(EXP *e)
{
    if (e != NULL) 
    {
        switch (e->kind)
        {
        case k_expKindIdentifier:
            if (isBlankId(e->val.identExp.ident)) throwError("cannot use _ as a value", e->lineno);
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
            weedEXP_eval(e->val.binary.lhs);
            weedEXP_eval(e->val.binary.rhs);
            break;

        case k_expKindUPlus:
        case k_expKindUMinus:
        case k_expKindBang:
        case k_expKindUBitXOR:
            weedEXP_eval(e->val.unary.rhs);
            break;

        case k_expKindFuncCall:
            weedEXP_eval(e->val.funcCall.primaryExpr);
            break;

        case k_expKindArrayAccess:
            weedEXP_eval(e->val.arrayAccess.arrayReference);
            weedEXP_eval(e->val.arrayAccess.indexExp);
            break;

        case k_expKindFieldAccess:
            weedEXP_eval(e->val.fieldAccess.object);
            if (isBlankId(e->val.fieldAccess.field)) throwError("cannot refer to blank field", e->lineno);
            break;

        case k_expKindAppend:
            weedEXP_eval(e->val.append.slice);
            weedEXP_eval(e->val.append.addend);
            break;

        case k_expKindLen:
            weedEXP_eval(e->val.lenExp);
            break;

        case k_expKindCap:
            weedEXP_eval(e->val.capExp);
            break;

        default:
            break;
        }

        weedEXP_eval(e->next);
    }
    return;
}

void weedTYPE(TYPE *t)
{
    if (t != NULL)
    {
        switch (t->kind)
        {
        case k_typeSlice:
        case k_typeArray:
        case k_typeStruct:
            break;
        case k_typeInfer:
            if (isBlankId(t->val.identifier)) throwError("cannot use _ as a value", t->lineno);
            break;
        }
    }
    return;
}

void weedEXP_nonEval(EXP *e, bool allowBlankId)
{
    if (e != NULL)
    {
        switch (e->kind)
        {
        case k_expKindIdentifier:
            if (!allowBlankId && isBlankId(e->val.identExp.ident)) throwError("cannot use _ as a value", e->lineno);
            break;

        case k_expKindIntLiteral:
        case k_expKindFloatLiteral:
        case k_expKindBoolLiteral:
        case k_expKindRuneLiteral:
        case k_expKindStringLiteral:
            throwError("cannot use a literal in an expression that doesn't evaluate to a value", e->lineno);
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
            throwError("cannot perform binary operation in an expression that doesn't evaluate to a value", e->lineno);
            break;

        case k_expKindUPlus:
        case k_expKindUMinus:
        case k_expKindBang:
        case k_expKindUBitXOR:
            throwError("cannot perform unary operation in an expression that doesn't evaluate to a value", e->lineno);
            break;

        case k_expKindFuncCall:
            // throwError("cannot use function call in an expression that doesn't evaluate to a value", e->lineno);
            break;

        case k_expKindArrayAccess:
            weedEXP_eval(e->val.arrayAccess.arrayReference);
            weedEXP_eval(e->val.arrayAccess.indexExp);
            break;

        case k_expKindFieldAccess:
            weedEXP_eval(e->val.fieldAccess.object);
            if (isBlankId(e->val.fieldAccess.field)) throwError("cannot refer to blank field", e->lineno);
            break;

        case k_expKindAppend:
        case k_expKindLen:
        case k_expKindCap:
            throwError("cannot use a builtin in an expression that doesn't evaluate to a value", e->lineno);
            break;

        // case k_expKindUParenthesized:
        //     throwError("non-name on left side of :=", e->lineno);
        //     break;
        }
    }
    return;
}

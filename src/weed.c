#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "error.h"
#include "weed.h"

#define BLANK_IDENTIFIER "_"

bool isBlankId(char *identifier);
void weedTOPLEVELDECL_varSpec(VARSPEC *vs);
void weedFUNC(FUNC *f);
void weedFUNC_checkForReturnStmt(FUNC *f);
void weedFUNC_inputParams(TYPESPEC *ts);
void weedFUNC_inputParams_id(IDENT *id);
void weedSTMT_assign(STMT *s);
void weedEXPRCASECLAUSE(EXPRCASECLAUSE *cc);
void weedEXPRCASECLAUSE_findDefaultCase(EXPRCASECLAUSE *cc, bool foundPreviousDefaultCase);
void weedEXP_nonEval(EXP *e);
void weedEXP_eval(EXP *e);
void weedTYPE(TYPE *t);

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
    if (d != NULL)
    {
        weedTOPLEVELDECL(d->next);

        switch (d->kind)
        {
        case k_topLevelDeclVar:
            weedTOPLEVELDECL_varSpec(d->val.varDecl);
            break;
        
        case k_topLevelDeclType:
            break;
        
        case k_topLevelDeclFunc:
            weedFUNC(d->val.funcDecl);
            break;
        }
    }
    return;
}

void weedTOPLEVELDECL_varSpec(VARSPEC *vs)
{
    if (vs != NULL)
    {
        if (vs->rhs != NULL) throwError("top level variable declarations cannot be assigned an expression", 0); // TODO add lineno in VARSPEC
        weedTOPLEVELDECL_varSpec(vs->next);
    }
    return;
}

void weedFUNC(FUNC *f)
{
    if (f != NULL) {
        // if (f->returnType != NULL) weedFUNC_checkForReturnStmt(f);
        // if (f->returnType != NULL && f->returnStmt == NULL) throwError("expected return statement", f->lineno);
        // printf("stmts: %p\n", f->rootStmt);
        // printf("return: %p\n", f->returnStmt);
        // weedFUNC_inputParams(f->inputParams);
        weedSTMT(f->rootStmt);
    }
}

void weedFUNC_checkForReturnStmt(FUNC *f)
{
    if (f->returnStmt != NULL) return;
    STMT *s = f->rootStmt;
    while(s != NULL) 
    {
        if (s->kind == k_stmtKindReturn) return;
        s = s->next;
    }

    throwError("expected return statement", f->lineno);
}

void weedFUNC_inputParams(TYPESPEC *ts)
{
    if (ts != NULL)
    {
        weedFUNC_inputParams_id(ts->ident);
        weedFUNC_inputParams(ts->next);
    }
    return;
}

void weedFUNC_inputParams_id(IDENT *id)
{
    if (id != NULL)
    {
        // if (isBlankId(id->ident)) throwError("cannot declare _ as a function parameter", 0); // TODO add lineno to IDENT
        weedFUNC_inputParams_id(id->next);
    }
    return;
}

void weedSTMT(STMT *s)
{
    if (s != NULL) 
    {
        switch (s->kind)
        {
        case k_stmtKindExpStmt:
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
            if (s->val.varDecl->rhs != NULL) weedEXP_eval(s->val.varDecl->rhs);
            break;

        case k_stmtKindTypeDecl:
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
            weedEXP_eval(s->val.switchStmt.exp);
            weedSTMT(s->val.switchStmt.simpleStmt);
            weedEXPRCASECLAUSE_findDefaultCase(s->val.switchStmt.caseClauses, false);
            weedEXPRCASECLAUSE(s->val.switchStmt.caseClauses);
            break;

        case k_stmtKindFor:
            weedSTMT(s->val.forLoop.initStmt);
            weedEXP_eval(s->val.forLoop.condition);
            weedSTMT(s->val.forLoop.body);
            weedSTMT(s->val.forLoop.postStmt);
            break;

        case k_stmtKindBreak:
            break;

        case k_stmtKindContinue:
            break;

        case k_stmtKindReturn:
            weedEXP_eval(s->val.returnExp);
            break;

        }
        weedSTMT(s->next);
    }
    return;
}

void weedSTMT_assign(STMT *s)
{
    switch (s->val.assignStmt.kind)
    {
    case k_stmtAssign:
    case k_stmtColonAssign:
        weedEXP_nonEval(s->val.assignStmt.lhs);
        weedEXP_eval(s->val.assignStmt.rhs);
        break;

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
        weedEXP_eval(s->val.assignStmt.lhs);
        weedEXP_eval(s->val.assignStmt.rhs);
        break;

    }
    return;
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

        case k_expKindIntLiteral:
        case k_expKindFloatLiteral:
        case k_expKindBoolLiteral:
        case k_expKindRuneLiteral:
        case k_expKindStringLiteral:
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
        case k_expKindParentheses:
            weedEXP_eval(e->val.unary.rhs);
            break;

        case k_expKindFuncCall:
            weedTYPE(e->val.funcCall.type);
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
        }

        weedEXP_eval(e->next);
    }
    return;
}

// TODO finish
void weedTYPE(TYPE *t)
{
    // t->
}

void weedEXP_nonEval(EXP *e)
{
    if (e != NULL)
    {
        switch (e->kind)
        {
        case k_expKindIdentifier:
        case k_expKindIntLiteral:
        case k_expKindFloatLiteral:
        case k_expKindBoolLiteral:
        case k_expKindRuneLiteral:
        case k_expKindStringLiteral:
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
            weedEXP_nonEval(e->val.binary.lhs);
            weedEXP_nonEval(e->val.binary.rhs);
            break;

        case k_expKindUPlus:
        case k_expKindUMinus:
        case k_expKindBang:
        case k_expKindUBitXOR:
        case k_expKindParentheses:
            weedEXP_nonEval(e->val.unary.rhs);
            break;

        case k_expKindFuncCall:
            throwError("cannot use function call in an expression that doesn't evaluate to a value", e->lineno);
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
        }
    }
    return;
}

bool isBlankId(char *identifier)
{
    return strcmp(BLANK_IDENTIFIER, identifier) == 0;
}

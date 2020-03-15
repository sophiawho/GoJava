#include "pretty.h"
#include <stdio.h>
#include <stdlib.h>

int indentLevel = 0;

void pretty_INDENT()
{

    for (int i = 0; i < indentLevel; i++) printf("    ");
}

void pretty_PROG(PROG *root)
{
    printf("package %s\n\n", root->package);

    pretty_TOPLEVELDECL(root->rootTopLevelDecl);
}

void pretty_TOPLEVELDECL(TOPLEVELDECL *topLevelDecl)
{
    if (topLevelDecl == NULL)
        return;

    // head recursion
    pretty_TOPLEVELDECL(topLevelDecl->next);

    switch (topLevelDecl->kind)
    {
        case k_topLevelDeclVar:
            pretty_VARSPEC(topLevelDecl->val.varDecl);
            break;
        case k_topLevelDeclType:
            pretty_TYPESPEC(topLevelDecl->val.typeDecl, 0);
            break;
        case k_topLevelDeclFunc:
            pretty_FUNC(topLevelDecl->val.funcDecl);
            break;
    }
}

void pretty_VARSPEC(VARSPEC *varspec)
{
    if (varspec == NULL)
        return;

    // head recursion
    pretty_VARSPEC(varspec->next);

    printf("var ");
    pretty_IDENT(varspec->ident, 0);
    printf(" ");
    if (varspec->type != NULL)
    {
        pretty_TYPE(varspec->type);
        printf(" ");
    }
    if (varspec->rhs != NULL)
    {
        printf("= ");
        pretty_EXP(varspec->rhs, 0);
    }
    printf("\n");
}

void pretty_TYPESPEC(TYPESPEC *ts, int cur_i)
{
    if (ts == NULL)
        return;

    // head recursion
    pretty_TYPESPEC(ts->next, cur_i + 1);

    switch (ts->kind)
    {
        case k_typeSpecKindTypeDeclaration:
            printf("type %s ", ts->ident->ident);
            pretty_TYPE(ts->type);
            printf("\n");
            break;
        case k_typeSpecKindParameterList:
            pretty_IDENT(ts->ident, 0);
            printf(" ");
            pretty_TYPE(ts->type);
            if (cur_i != 0) printf(", ");
            break;
        default:
            printf("Error: invalid type specifier kind");
            exit(1);
    }
}

void pretty_STRUCTSPEC(STRUCTSPEC *ss)
{
    if (ss == NULL)
        return;

    //head recursion
    pretty_STRUCTSPEC(ss->next);

    pretty_INDENT(1);
    pretty_IDENT(ss->attribute, 0);
    printf(" ");
    pretty_TYPE(ss->type);
    printf("\n");
}

void pretty_IDENT(IDENT *ident, int cur_i)
{
    if (ident == NULL)
        return;

    // head recursion
    pretty_IDENT(ident->next, cur_i + 1);

    printf("%s", ident->ident);
    // first identifier in list is the last to be printed so no comma
    if (cur_i != 0)
        printf(", ");
}

void pretty_EXP(EXP *e, int cur_i)
{
    if (e == NULL)
        return;

    // head recursion
    pretty_EXP(e->next, cur_i + 1);

    if (e->kind != k_expKindIdentifier
        && e->kind != k_expKindIntLiteral
        && e->kind != k_expKindFloatLiteral
        && e->kind != k_expKindBoolLiteral
        && e->kind != k_expKindRuneLiteral
        && e->kind != k_expKindStringLiteral)
    {
        printf("(");
    }
    switch (e->kind)
    {
        case k_expKindIdentifier:
            printf("%s", e->val.identExp.ident);
            break;
        case k_expKindIntLiteral:
            printf("%d", e->val.intLiteral);
            break;
        case k_expKindFloatLiteral:
            printf("%f", e->val.floatLiteral);
            break;
        case k_expKindBoolLiteral:
            printf("%s", e->val.boolLiteral ? "true" : "false");
            break;
        case k_expKindRuneLiteral:
            printf("%c", e->val.runeLiteral);
            break;
        case k_expKindStringLiteral:
            printf("%s", e->val.stringLiteral);
            break;
        case k_expKindAnd:
            pretty_EXP(e->val.binary.lhs, 0);
            printf(" && ");
            pretty_EXP(e->val.binary.rhs, 0);
            break;
        case k_expKindOr:
            pretty_EXP(e->val.binary.lhs, 0);
            printf(" || ");
            pretty_EXP(e->val.binary.rhs, 0);
            break;
        case k_expKindEq:
            pretty_EXP(e->val.binary.lhs, 0);
            printf(" == ");
            pretty_EXP(e->val.binary.rhs, 0);
            break;
        case k_expKindNotEq:
            pretty_EXP(e->val.binary.lhs, 0);
            printf(" != ");
            pretty_EXP(e->val.binary.rhs, 0);
            break;
        case k_expKindLess:
            pretty_EXP(e->val.binary.lhs, 0);
            printf(" < ");
            pretty_EXP(e->val.binary.rhs, 0);
            break;
        case k_expKindLessEq:
            pretty_EXP(e->val.binary.lhs, 0);
            printf(" <= ");
            pretty_EXP(e->val.binary.rhs, 0);
            break;
        case k_expKindGrtr:
            pretty_EXP(e->val.binary.lhs, 0);
            printf(" > ");
            pretty_EXP(e->val.binary.rhs, 0);
            break;
        case k_expKindGrtrEq:
            pretty_EXP(e->val.binary.lhs, 0);
            printf(" >= ");
            pretty_EXP(e->val.binary.rhs, 0);
            break;
        case k_expKindAdd:
            pretty_EXP(e->val.binary.lhs, 0);
            printf(" + ");
            pretty_EXP(e->val.binary.rhs, 0);
            break;
        case k_expKindMinus:
            pretty_EXP(e->val.binary.lhs, 0);
            printf(" - ");
            pretty_EXP(e->val.binary.rhs, 0);
            break;
        case k_expKindBitOr:
            pretty_EXP(e->val.binary.lhs, 0);
            printf(" | ");
            pretty_EXP(e->val.binary.rhs, 0);
            break;
        case k_expKindBitXOR:
            pretty_EXP(e->val.binary.lhs, 0);
            printf(" ^ ");
            pretty_EXP(e->val.binary.rhs, 0);
            break;
        case k_expKindMult:
            pretty_EXP(e->val.binary.lhs, 0);
            printf(" * ");
            pretty_EXP(e->val.binary.rhs, 0);
            break;
        case k_expKindDiv:
            pretty_EXP(e->val.binary.lhs, 0);
            printf(" / ");
            pretty_EXP(e->val.binary.rhs, 0);
            break;
        case k_expKindMod:
            pretty_EXP(e->val.binary.lhs, 0);
            printf(" %% ");
            pretty_EXP(e->val.binary.rhs, 0);
            break;
        case k_expKindLeftShift:
            pretty_EXP(e->val.binary.lhs, 0);
            printf(" << ");
            pretty_EXP(e->val.binary.rhs, 0);
            break;
        case k_expKindRightShift:
            pretty_EXP(e->val.binary.lhs, 0);
            printf(" >> ");
            pretty_EXP(e->val.binary.rhs, 0);
            break;
        case k_expKindBitAnd:
            pretty_EXP(e->val.binary.lhs, 0);
            printf(" & ");
            pretty_EXP(e->val.binary.rhs, 0);
            break;
        case k_expKindBitClear:
            pretty_EXP(e->val.binary.lhs, 0);
            printf(" &^ ");
            pretty_EXP(e->val.binary.rhs, 0);
            break;
        case k_expKindUPlus:
            printf("+");
            pretty_EXP(e->val.unary.rhs, 0);
            break;
        case k_expKindUMinus:
            printf("-");
            pretty_EXP(e->val.unary.rhs, 0);
            break;
        case k_expKindBang:
            printf("!");
            pretty_EXP(e->val.unary.rhs, 0);
            break;
        case k_expKindUBitXOR:
            printf("^");
            pretty_EXP(e->val.unary.rhs, 0);
            break;
        case k_expKindArrayAccess:
            pretty_EXP(e->val.arrayAccess.arrayReference, 0);
            printf("[");
            pretty_EXP(e->val.arrayAccess.indexExp, 0);
            printf("]");
            break;
        case k_expKindFieldAccess:
            pretty_EXP(e->val.fieldAccess.object, 0);
            printf(".");
            printf("%s", e->val.fieldAccess.field);
            break;
        case k_expKindAppend:
            printf("append(");
            pretty_EXP(e->val.append.slice, 0);
            printf(", ");
            pretty_EXP(e->val.append.addend, 0);
            printf(")");
            break;
        case k_expKindLen:
            printf("len(");
            pretty_EXP(e->val.lenExp, 0);
            printf(")");
            break;
        case k_expKindCap:
            printf("cap(");
            pretty_EXP(e->val.capExp, 0);
            printf(")");
        case k_expKindFuncCall:
            pretty_EXP(e->val.funcCall.primaryExpr, 0);
            printf("(");
            pretty_EXP(e->val.funcCall.expList, 0);
            printf(")");
            break;
        default:
            printf("Error: invalid expression kind\n");
            exit(1);
    }
    if (e->kind != k_expKindIdentifier
        && e->kind != k_expKindIntLiteral
        && e->kind != k_expKindFloatLiteral
        && e->kind != k_expKindBoolLiteral
        && e->kind != k_expKindRuneLiteral
        && e->kind != k_expKindStringLiteral)
    {
        printf(")");
    }

    // first expression in list is the last to be printed so no comma
    if (cur_i != 0)
        printf(", ");
}

void pretty_TYPE(TYPE *t)
{
    switch (t->kind)
    {
        case k_typeInfer:
            printf("%s", t->val.identifier);
            break;
        case k_typeSlice:
            printf("[]");
            pretty_TYPE(t->val.sliceType.type);
            break;
        case k_typeArray:
            printf("[%d]", t->val.arrayType.size);
            pretty_TYPE(t->val.arrayType.type);
            break;
        case k_typeStruct:
            printf("struct {\n");
            indentLevel++;
            pretty_STRUCTSPEC(t->val.structType);
            indentLevel--;
            pretty_INDENT(1);
            printf("}");
            break;
        default:
            printf("Error: invalid type kind\n");
            exit(1);
    }
}

void pretty_FUNC(FUNC *f)
{
    printf("func ");
    printf("%s", f->name);
    printf("(");
    pretty_TYPESPEC(f->inputParams, 0);
    printf(") ");
    if (f->returnType) 
    {
        pretty_TYPE(f->returnType);
        printf(" ");
    }
    pretty_STMT(f->rootStmt, NULL);
}

void pretty_STMT(STMT *s, STMT *parent)
{
    if (s == NULL)
        return;

    // head recursion
    pretty_STMT(s->next, NULL);

    pretty_indentFirstLineOfStmt(s, parent);

    switch (s->kind)
    {
        case k_stmtKindBlock:
            printf("{\n");
            indentLevel++;
            pretty_STMT(s->val.blockStmt, s);
            indentLevel--;
            pretty_INDENT(1);
            printf("}");
            break;
        case k_stmtKindExpStmt:
            pretty_EXP(s->val.expStmt, 0);
            break;
        case k_stmtKindIncDec:
            pretty_EXP(s->val.incDecStmt.exp, 0);
            if (s->val.incDecStmt.amount)
                printf("++");
            else
                printf("--");
            break;
        case k_stmtKindAssign:
            pretty_EXP(s->val.assignStmt.lhs, 0);
            pretty_assign(s->val.assignStmt.kind);
            pretty_EXP(s->val.assignStmt.rhs, 0);
            break;
        case k_stmtKindPrint:
            if (s->val.printStmt.newLine)
                printf("println(");
            else
                printf("print(");
            pretty_EXP(s->val.printStmt.expList, 0);
            printf(")");
            break;
        case k_stmtKindVarDecl:
            pretty_VARSPEC(s->val.varDecl);
            break;
        case k_stmtKindTypeDecl:
            pretty_TYPESPEC(s->val.typeDecl, 0);
            break;
        case k_stmtKindIfStmt:
            printf("if ");
            if (s->val.ifStmt.simpleStmt)
            {
                pretty_STMT(s->val.ifStmt.simpleStmt, s);
                printf("; ");
            }
            pretty_EXP(s->val.ifStmt.condition, 0);
            printf(" ");
            pretty_STMT(s->val.ifStmt.trueBody, s);
            if (s->val.ifStmt.falseBody)
            {
                printf(" else ");
                pretty_STMT(s->val.ifStmt.falseBody, s);
            }
            break;
        case k_stmtKindSwitch:
            printf("switch ");
            pretty_STMT(s->val.switchStmt.simpleStmt, s);
            printf("; ");
            pretty_EXP(s->val.switchStmt.exp, 0);
            printf(" {\n");
            indentLevel++;
            pretty_EXPRCASECLAUSE(s->val.switchStmt.caseClauses);
            indentLevel--;
            pretty_INDENT();
            printf("}");
            break;
        case k_stmtKindFor:
            printf("for ");
            if (s->val.forLoop.kind == k_loopKindWhile)
            {
                pretty_EXP(s->val.forLoop.condition, 0);
                printf(" ");
            }
            else if (s->val.forLoop.kind == k_loopKindThreePart)
            {
                pretty_STMT(s->val.forLoop.initStmt, s);
                printf("; ");
                pretty_EXP(s->val.forLoop.condition, 0);
                printf("; ");
                pretty_STMT(s->val.forLoop.postStmt, s);
                printf(" ");
            }
            pretty_STMT(s->val.forLoop.body, s);
            break;
        case k_stmtKindBreak:
            printf("break");
            break;
        case k_stmtKindContinue:
            printf("continue");
            break;
        case k_stmtKindReturn:
            printf("return ");
            pretty_EXP(s->val.returnExp, 0);
            break;
        case k_stmtKindEmpty:
            break;
        default:
            printf("Error: invalid statement kind\n");
            exit(1);
    }

    pretty_endLastLineOfStmt(s, parent);
}

void pretty_assign(AssignKind kind)
{
    switch (kind)
    {
        case k_stmtAssign:
            printf(" = ");
            break;
        case k_stmtColonAssign:
            printf(" := ");
            break;
        case k_opAssignKindPlusEq:
            printf(" += ");
            break;
        case k_opAssignKindMinusEq:
            printf(" -= ");
            break;
        case k_opAssignKindMultEq:
            printf(" *= ");
            break;
        case k_opAssignKindDivEq:
            printf(" /= ");
            break;
        case k_opAssignKindModEq:
            printf(" %%= ");
            break;
        case k_opAssignKindBitAndEq:
            printf(" &= ");
            break;
        case k_opAssignKindBitOrEq:
            printf(" |= ");
            break;
        case k_opAssignKindBitXorEq:
            printf(" ^= ");
        case k_opAssignKindLeftShiftEq:
            printf(" <<= ");
            break;
        case k_opAssignKindRightShiftEq:
            printf(" >>= ");
            break;
        case k_opAssignKindBitClearEq:
            printf(" &^= ");
            break;
        default:
            printf("Error: invalid assign kind\n");
            exit(1);
    }
}

void pretty_indentFirstLineOfStmt(STMT *s, STMT *parent)
{
    if (parent == NULL)
    {
        pretty_INDENT();
        return;
    }

    int indent = 1;

    if (parent->kind == k_stmtKindIfStmt
        || parent->kind == k_stmtKindSwitch
        || parent->kind == k_stmtKindFor)
    {
        indent = 0;
    }

    if (indent)
        pretty_INDENT();
}

void pretty_endLastLineOfStmt(STMT *s, STMT *parent)
{
    if (parent == NULL)
    {
        printf("\n");
        return;
    }

    int newLine = 1;

    if (parent->kind == k_stmtKindIfStmt
        || parent->kind == k_stmtKindSwitch
        || parent->kind == k_stmtKindFor)
    {
        newLine = 0;
    }

    if (newLine)
        printf("\n");
}

void pretty_EXPRCASECLAUSE(EXPRCASECLAUSE *caseClause)
{
    if (caseClause == NULL)
        return;

    pretty_EXPRCASECLAUSE(caseClause->next);

    pretty_INDENT();

    switch (caseClause->kind)
    {
        case k_caseClause:
            printf("case ");
            pretty_EXP(caseClause->expList, 0);
            printf(":\n");
            indentLevel++;
            pretty_STMT(caseClause->stmtList, NULL);
            indentLevel--;
            break;
        case k_defaultClause:
            printf("default:\n");
            indentLevel++;
            pretty_STMT(caseClause->stmtList, NULL);
            indentLevel--;
            break;
        default:
            printf("Error: invalid case clause type");
    }
}
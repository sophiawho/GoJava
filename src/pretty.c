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
    printf("package %s\n", root->package);

    pretty_TOPLEVELDECL(root->rootTopLevelDecl);
}

void pretty_TOPLEVELDECL(TOPLEVELDECL *topLevelDecl)
{
    if (topLevelDecl == NULL)
        return;

    pretty_TOPLEVELDECL(topLevelDecl->next);

    switch (topLevelDecl->kind)
    {
        case k_topLevelDeclVar:
            pretty_VARSPEC(topLevelDecl->val.varDecl);
            break;
        case k_topLevelDeclType:
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
            printf(" % ");
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
        case k_expKindParentheses:
            printf("(");
            pretty_EXP(e->val.unary.rhs, 0);
            printf(")");
            break;
        case k_expKindFuncCall:
            // TODO
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
        case k_expKindAppend:
        case k_expKindLen:
        case k_expKindCap:
        default:
            printf("Error: invalid expression kind\n");
            exit(1);
    }

    // first expression in list is the last to be printed so no comma
    if (cur_i != 0)
        printf(", ");
}

void pretty_TYPE(TYPE *t)
{
    switch (t->kind)
    {
        case k_typeInt:
            printf("int");
            break;
        case k_typeFloat:
            printf("float64");
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
            printf("Error: invalid type kind\n");
    }
}

void pretty_FUNC(FUNC *f)
{
    printf("func ");
    printf("%s", f->name);
    printf("() ");
    printf("{\n");
    indentLevel++;
    pretty_STMT(f->rootStmt);
    indentLevel--;
    printf("}\n\n");
}

void pretty_STMT(STMT *s)
{
    if (s == NULL)
        return;

    // head recursion
    pretty_STMT(s->next);

    switch (s->kind)
    {
        case k_stmtKindBlock:
            pretty_STMT(s->val.blockStmt);
            break;
        case k_stmtKindExpStmt:
            pretty_INDENT();
            pretty_EXP(s->val.expStmt, 0);
            printf("\n");
            break;
    }
}
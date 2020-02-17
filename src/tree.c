#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

extern int yylineno;

PROG *makePROG(char *package, TOPLEVELDECL *rootTopLevelDecl) {
    PROG *p = malloc(sizeof(PROG));
    p->package = package;
    p->rootTopLevelDecl = rootTopLevelDecl;
    return p;
}

/* The IDENT and EXP fields in varspec may be a list, where the next node will be stored in the `.next` pointer.
In this case, multiple top level declarations will be initialized. */
TOPLEVELDECL *makeTopLevelDecl_var(VARSPEC *varspec) {
    TOPLEVELDECL *decl = malloc(sizeof(TOPLEVELDECL));
    decl->lineno = yylineno;
    decl->kind = k_topLevelDeclVar;
    decl->val.varDecl = varspec;

    IDENT *ident = varspec->ident;
    EXP *exp = varspec->rhs;

    // TODO: variableSpecs can also have a next pointer, eg:
    // var (
    //     x1, x2 int
    //     y1, y2 = 42, 43
    //     z2, z2 int = 1, 2
    // )   
    while (ident->next != NULL) {
        TOPLEVELDECL *decl_next = malloc(sizeof(TOPLEVELDECL));

        decl->next = decl_next;
        decl_next->lineno = yylineno;
        decl_next->kind = k_topLevelDeclVar;

        ident = ident->next;

        if (exp != NULL && exp->next != NULL) {
            exp = exp->next;
        }

        decl_next->val.varDecl = makeVarSpec(ident, exp, varspec->type);
        decl = decl_next;
    }
    printf("RETURNING: %s\n", decl->val.varDecl->ident->ident);
    return decl;
}

VARSPEC *makeVarSpec(IDENT *ident, EXP *rhs, TYPE *type) {
    VARSPEC *vs = malloc(sizeof(VARSPEC));
    vs->ident = ident;
    vs->rhs = rhs;
    vs->type = type;
    return vs;
}

IDENT *makeIDENT(char *ident) {
    IDENT *i = malloc(sizeof(ident));
    i->ident = ident;
    return i;
}

EXP *makeEXP_intLiteral(int intLiteral) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expKindIntLiteral;
	e->val.intLiteral = intLiteral;
	// TODO: set e->type
	return e;
}
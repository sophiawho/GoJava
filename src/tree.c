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

EXP *makeEXP_identifier(char *id) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expKindIdentifier;
	e->val.identExp.ident = id;
	return e;
}

EXP *makeEXP_intLiteral(int intLiteral) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expKindIntLiteral;
	e->val.intLiteral = intLiteral;
	// TODO: set e->type
	return e;
}

EXP *makeEXP_floatLiteral(float floatLiteral) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expKindFloatLiteral;
	e->val.floatLiteral = floatLiteral;
	// TODO: set e->type
	return e;
}

EXP *makeEXP_runeLiteral(char runeLiteral) {
    EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expKindRuneLiteral;
	e->val.runeLiteral = runeLiteral;
	// TODO: set e->type
	return e;
}

EXP *makeEXP_stringLiteral(char *stringLiteral) {
    EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expKindRuneLiteral;
	e->val.stringLiteral = stringLiteral;
	// TODO: set e->type
	return e;
}

TYPE *makeTYPE(TypeKind kind) {
    TYPE *type = malloc(sizeof(TYPE));
    type->kind = kind;
    return type;
}
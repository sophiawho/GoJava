#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

extern int yylineno;

PROG *makePROG(char *package, TOPLEVELDECL *rootTopLevelDecl) {
    PROG *p = malloc(sizeof(PROG));
    p->package = package;
    p->rootTopLevelDecl = rootTopLevelDecl;
    return p;
}

TOPLEVELDECL *makeTopLevelDecl_var(VARSPEC *varspec) {
    TOPLEVELDECL *decl = malloc(sizeof(TOPLEVELDECL));
    decl->lineno = yylineno;
    decl->kind = k_topLevelDeclVar;
    decl->val.varDecl = varspec;
    return decl;
}

TOPLEVELDECL *makeTopLevelDecl_type(TYPESPEC *typespec) {
	TOPLEVELDECL *decl = malloc(sizeof(TOPLEVELDECL));
	decl->lineno = yylineno;
	decl->kind = k_topLevelDeclType;
	decl->val.typeDecl = typespec;
	return decl;
}

VARSPEC *makeVarSpec(IDENT *ident, EXP *rhs, TYPE *type) {
    VARSPEC *vs = malloc(sizeof(VARSPEC));
    vs->ident = ident;
    vs->rhs = rhs;
    vs->type = type;
    return vs;
}

STRUCTSPEC *makeStructSpec(IDENT *attribute, TYPE *type) {
	STRUCTSPEC *ss = malloc(sizeof(STRUCTSPEC));
	ss->attribute = attribute;
	ss->type = type;
	return ss;
}

TYPESPEC *makeTypeSpec(IDENT *ident, TYPE *type) {
	TYPESPEC *ts = malloc(sizeof(TYPESPEC));
	ts->ident = ident;
	ts->type = type;
	return ts;
}

TYPESPEC *makeTypeSpec_struct(IDENT *ident, STRUCTSPEC *ss) {
	TYPESPEC *ts = malloc(sizeof(TYPESPEC));
	ts->ident = ident;
	TYPE *t = makeTYPE(k_typeStruct);
	t->val.structType = ss;
	return ts;
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
	e->type = makeTYPE(k_typeInt);
	return e;
}

EXP *makeEXP_floatLiteral(float floatLiteral) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expKindFloatLiteral;
	e->val.floatLiteral = floatLiteral;
	e->type = makeTYPE(k_typeFloat);
	return e;
}

EXP *makeEXP_runeLiteral(char runeLiteral) {
    EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expKindRuneLiteral;
	e->val.runeLiteral = runeLiteral;
	e->type = makeTYPE(k_typeRune);
	return e;
}

EXP *makeEXP_stringLiteral(char *stringLiteral) {
    EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expKindRuneLiteral;
	e->val.stringLiteral = stringLiteral;
	e->type = makeTYPE(k_typeString);
	return e;
}

EXP *makeEXP_binary(ExpressionKind op, EXP *lhs, EXP *rhs) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = op;
	e->val.binary.lhs = lhs;
	e->val.binary.rhs = rhs;
	e->type = makeTYPE(k_typeInfer);
	return e;
}

EXP *makeEXP_unary(ExpressionKind op, EXP *rhs) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = op;
	e->val.unary.rhs = rhs;
	e->type = makeTYPE(k_typeInfer);
	return e;
}

EXP *makeEXP_append(EXP *slice, EXP *addend) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expKindAppend;
	e->val.append.slice = slice;
	e->val.append.addend = addend;
	e->type = makeTYPE(k_typeInfer);
	return e;
}

EXP *makeEXP_len(EXP *lenExp) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expKindLen;
	e->val.lenExp = lenExp;
	e->type = makeTYPE(k_typeInfer);
	return e;
}

EXP *makeEXP_cap(EXP *capExp) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = k_expKindCap;
	e->val.capExp = capExp;
	e->type = makeTYPE(k_typeInfer);
	return e;
}

TYPE *makeTYPE_ident(char *identifier) {
	if (strcmp(identifier, "int") == 0) {
		return makeTYPE(k_typeInt);
	}
	else if (strcmp(identifier, "float64") == 0) {
		return makeTYPE(k_typeFloat);
	}
	else if (strcmp(identifier, "bool") == 0) {
		return makeTYPE(k_typeBool);
	}
	else if (strcmp(identifier, "rune") == 0) {
		return makeTYPE(k_typeRune);
	}
	else if (strcmp(identifier, "string") == 0) {
		return makeTYPE(k_typeString);
	}
	return NULL;
}

TYPE *makeTYPE_array(EXP *exp, TYPE *type) {
	TYPE *t = makeTYPE(k_typeArray);
	t->val.arrayType.exp = exp;
	t->val.arrayType.type = type;
	return t;
}

TYPE *makeTYPE_slice(TYPE *type) {
	TYPE *t = makeTYPE(k_typeSlice);
	t->val.sliceType.type = type;
	return t;
}

TYPE *makeTYPE(TypeKind kind) {
    TYPE *type = malloc(sizeof(TYPE));
    type->kind = kind;
    return type;
}
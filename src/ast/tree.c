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

TOPLEVELDECL *makeTopLevelDecl_func(FUNC *funcdecl) {
	TOPLEVELDECL *decl = malloc(sizeof(TOPLEVELDECL));
	decl->lineno = yylineno;
	decl->kind = k_topLevelDeclFunc;
	decl->val.funcDecl = funcdecl;
	return decl;
}

TYPE *makeTYPE_ident(char *identifier) {
	TYPE *t = makeTYPE(k_typeInfer);
	t->val.identifier = identifier;
	return t;
}

TYPE *makeTYPE_array(int size, TYPE *type) {
	TYPE *t = makeTYPE(k_typeArray);
	t->val.arrayType.size = size;
	t->val.arrayType.type = type;
	return t;
}

TYPE *makeTYPE_slice(TYPE *type) {
	TYPE *t = makeTYPE(k_typeSlice);
	t->val.sliceType.type = type;
	return t;
}

TYPE *makeTYPE_struct(STRUCTSPEC *ss)
{
	TYPE *t = makeTYPE(k_typeStruct);
	t->val.structType = ss;
	return t;
}

TYPE *makeTYPE(TypeKind kind) {
    TYPE *type = malloc(sizeof(TYPE));
    type->kind = kind;
	type->lineno = yylineno;
	type->parent = NULL;
    return type;
}

EXP *getGenericExpr(ExpressionKind kind) {
	EXP *e = malloc(sizeof(EXP));
	e->lineno = yylineno;
	e->kind = kind;
	return e;
}

EXP *makeEXP_identifier(char *id) {
	EXP *e = getGenericExpr(k_expKindIdentifier);
	e->val.identExp.ident = id;
	return e;
}

EXP *makeEXP_intLiteral(int intLiteral) {
	EXP *e = getGenericExpr(k_expKindIntLiteral);
	e->val.intLiteral = intLiteral;
	e->type = makeTYPE(k_typeInfer);
	return e;
}

EXP *makeEXP_floatLiteral(float floatLiteral) {
	EXP *e = getGenericExpr(k_expKindFloatLiteral);
	e->val.floatLiteral = floatLiteral;
	e->type = makeTYPE(k_typeInfer);
	return e;
}

EXP *makeEXP_runeLiteral(char runeLiteral) {
    EXP *e = getGenericExpr(k_expKindRuneLiteral);
	e->val.runeLiteral = runeLiteral;
	e->type = makeTYPE(k_typeInfer);
	return e;
}

EXP *makeEXP_stringLiteral(char *stringLiteral) {
    EXP *e = getGenericExpr(k_expKindStringLiteral);
	e->val.stringLiteral = stringLiteral;
	e->type = makeTYPE(k_typeInfer);
	return e;
}

EXP *makeEXP_binary(ExpressionKind op, EXP *lhs, EXP *rhs) {
	EXP *e = getGenericExpr(op);
	e->val.binary.lhs = lhs;
	e->val.binary.rhs = rhs;
	e->type = makeTYPE(k_typeInfer);
	return e;
}

EXP *makeEXP_unary(ExpressionKind op, EXP *rhs) {
	EXP *e = getGenericExpr(op);
	e->val.unary.rhs = rhs;
	e->type = makeTYPE(k_typeInfer);
	return e;
}

EXP *makeEXP_append(EXP *slice, EXP *addend) {
	EXP *e = getGenericExpr(k_expKindAppend);
	e->val.append.slice = slice;
	e->val.append.addend = addend;
	e->type = makeTYPE(k_typeInfer);
	return e;
}

EXP *makeEXP_len(EXP *lenExp) {
	EXP *e = getGenericExpr(k_expKindLen);
	e->val.lenExp = lenExp;
	e->type = makeTYPE(k_typeInfer);
	return e;
}

EXP *makeEXP_cap(EXP *capExp) {
	EXP *e = getGenericExpr(k_expKindCap);
	e->val.capExp = capExp;
	e->type = makeTYPE(k_typeInfer);
	return e;
}

EXP *makeEXP_funcCall(EXP *primaryExpr, EXP *expList) {
	EXP *e = getGenericExpr(k_expKindFuncCall);
	e->val.funcCall.primaryExpr = primaryExpr;
	e->val.funcCall.expList = expList;
	return e;
}

EXP *makeEXP_arrayAccess(EXP *arrayReference, EXP *indexExp) {
	EXP *e = getGenericExpr(k_expKindArrayAccess);
	e->val.arrayAccess.arrayReference = arrayReference;
	e->val.arrayAccess.indexExp = indexExp;
	return e;
}

EXP *makeEXP_fieldAccess(EXP *object, char *field) {
	EXP *e = getGenericExpr(k_expKindFieldAccess);
	e->val.fieldAccess.object = object;
	e->val.fieldAccess.field = field;
	return e;
}

FUNC *makeFunc(char *name, TYPE *returnType, TYPESPEC *inputParams, STMT *rootStmt) {
	FUNC *func = malloc(sizeof(FUNC));
	func->lineno = yylineno;
	func->name = name;
	func->returnType = returnType;
	func->inputParams = inputParams;
	func->rootStmt = rootStmt;
	return func;
}

IDENT *makeIDENT(char *ident) {
    IDENT *i = malloc(sizeof(ident));
    i->ident = ident;
    return i;
}

VARSPEC *makeVarSpec(IDENT *ident, EXP *rhs, TYPE *type) {
    VARSPEC *vs = malloc(sizeof(VARSPEC));
	vs->lineno = yylineno;
    vs->ident = ident;
    vs->rhs = rhs;
    vs->type = type;
    return vs;
}

TYPESPEC *makeTypeSpec(TypeSpecKind kind, IDENT *ident, TYPE *type) {
	TYPESPEC *ts = malloc(sizeof(TYPESPEC));
	ts->lineno = yylineno;
	ts->kind = kind;
	ts->ident = ident;
	ts->type = type;
	return ts;
}

// TODO never used, remove?
TYPESPEC *makeTypeSpec_struct(TypeSpecKind kind, IDENT *ident, STRUCTSPEC *ss) {
	TYPESPEC *ts = malloc(sizeof(TYPESPEC));
	ts->lineno = yylineno;
	ts->kind = kind;
	ts->ident = ident;
	TYPE *t = makeTYPE(k_typeStruct);
	t->val.structType = ss;
	ts->type = t;
	return ts;
}

STRUCTSPEC *makeStructSpec(IDENT *attribute, TYPE *type) {
	STRUCTSPEC *ss = malloc(sizeof(STRUCTSPEC));
	ss->attribute = attribute;
	ss->type = type;
	return ss;
}
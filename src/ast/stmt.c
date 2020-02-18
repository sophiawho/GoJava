#include <stdbool.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "stmt.h"

extern int yylineno;

STMT *getGenericStmt(StatementKind kind) {
    STMT *stmt = malloc(sizeof(STMT));
    stmt->lineno = yylineno;
    stmt->kind = kind;
    return stmt;
}

STMT *makeSTMT_expStmt(EXP *exp) {
    STMT *stmt = getGenericStmt(k_stmtKindExpStmt);
    stmt->val.expStmt = exp;
	return stmt;
}

STMT *makeSTMT_incDec(EXP *exp, int amount) {
	STMT *stmt = getGenericStmt(k_stmtKindIncDec);
    stmt->val.incDecStmt.exp = exp;
    stmt->val.incDecStmt.amount = amount;
	return stmt;
}

STMT *makeSTMT_assignStmt(EXP *lhs, AssignKind kind, EXP *rhs) {
    STMT *stmt = getGenericStmt(k_stmtKindAssign);
    stmt->val.assignStmt.lhs = lhs;
    stmt->val.assignStmt.rhs = rhs;
    stmt->val.assignStmt.kind = kind;
    return stmt;
}

STMT *makeSTMT_returnStmt(EXP *returnExp) {
    STMT *stmt = getGenericStmt(k_stmtKindReturn);
    stmt->val.returnExp = returnExp;
    return stmt;
}

STMT *makeSTMT_blockStmt(STMT *blockStmt) {
    STMT *stmt = getGenericStmt(k_stmtKindBlock);
    stmt->val.blockStmt = blockStmt;
    return stmt;
}

STMT *makeSTMT_varDecl(VARSPEC *varDecl) {
    STMT *stmt = getGenericStmt(k_stmtKindVarDecl);
    stmt->val.varDecl = varDecl;
    return stmt;
}

STMT *makeSTMT_typeDecl(TYPESPEC *typeDecl) {
    STMT *stmt = getGenericStmt(k_stmtKindTypeDecl);
    stmt->val.typeDecl = typeDecl;
    return stmt;
}

STMT *makeSTMT_print(EXP *expList, int newLine) {
    STMT *stmt = getGenericStmt(k_stmtKindPrint);
    stmt->val.printStmt.expList = expList;
    stmt->val.printStmt.newLine = newLine;
    return stmt;
}

STMT *makeSTMT_if(STMT *simpleStmt, EXP *condition, STMT *trueBody, STMT *falseBody) {
    STMT *stmt = getGenericStmt(k_stmtKindIfStmt);
    stmt->val.ifStmt.simpleStmt = simpleStmt;
    stmt->val.ifStmt.condition = condition;
    stmt->val.ifStmt.trueBody = trueBody;
    stmt->val.ifStmt.falseBody = falseBody;
    return stmt;
}

STMT *makeSTMT_for(ForLoopKind kind, STMT *body, EXP *condition, STMT *initStmt, STMT *postStmt) {
    STMT *stmt = getGenericStmt(k_stmtKindFor);
    stmt->val.forLoop.kind = kind;
    stmt->val.forLoop.body = body;
    stmt->val.forLoop.condition = condition;
    stmt->val.forLoop.initStmt = initStmt;
    stmt->val.forLoop.postStmt = postStmt;
    return stmt;
}

STMT *makeSTMT_switch(STMT *simpleStmt, EXP *exp, EXPRCASECLAUSE *caseClauses) {
    STMT *stmt = getGenericStmt(k_stmtKindSwitch);
    stmt->val.switchStmt.simpleStmt = simpleStmt;
    stmt->val.switchStmt.exp = exp;
    stmt->val.switchStmt.caseClauses = caseClauses;
    return stmt;
}

EXPRCASECLAUSE *makeExprCaseClause(CaseClauseKind kind, EXP *expList, STMT *stmtList) {
    EXPRCASECLAUSE *ecc = malloc(sizeof(EXPRCASECLAUSE));
    ecc->lineno = yylineno;
    ecc->kind = kind;
    ecc->expList = expList;
    ecc->stmtList = stmtList;
    return ecc;
}
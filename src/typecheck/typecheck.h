#ifndef TYPECHECK_H
#define TYPECHECK_H

#include <stdbool.h>

#include "../ast/tree.h"
#include "../ast/stmt.h"

void typePROG(PROG *root);
void typeTOPLEVELDECL(TOPLEVELDECL *tld);
void typeVARSPEC(VARSPEC *varspec);
void typeTYPESPEC(TYPESPEC *ts);
void typeSTRUCTSPEC(STRUCTSPEC *ss);
void typeIDENT(IDENT *ident);
void typeEXP(EXP *e);
void typeTYPE(TYPE *t);
void typeFUNC(FUNC *f);
void typeSTMT(STMT *s, TYPE *returnType);
void typeEXPRCASECLAUSE(EXPRCASECLAUSE *caseClause, TYPE *returnType, TYPE *expType);
TYPE *resolveType(TYPE *t);

// Helper functions
char *typeToString(TYPE *t);
SYMBOL *getSymbolFromExp(EXP *e);
void typeFUNCCALL(TYPESPEC *inputParams, EXP *args);
bool isEqualType(TYPE *t1, TYPE *t2);

#endif /* !TYPECHECK_H */
#ifndef TYPECHECK_H
#define TYPECHECK_H

#include <stdbool.h>

#include "../ast/tree.h"
#include "../ast/stmt.h"

void typePROG(PROG *root);
void typeTOPLEVELDECL(TOPLEVELDECL *tld);
void typeVARSPEC(VARSPEC *varspec);
void typeSTRUCTSPEC(STRUCTSPEC *ss);
void typeIDENT(IDENT *ident);
void typeEXP(EXP *e);
void typeTYPE(TYPE *t);
void typeFUNC(FUNC *f);
void typeSTMT(STMT *s, TYPE *returnType);
void typeEXPRCASECLAUSE(EXPRCASECLAUSE *caseClause);
TYPE *resolveType(TYPE *t);

// Helper functions
char *typeToString(TYPE *t);
SYMBOL *getSymbolFromExp(EXP *e);
void typeFUNCCALL(TYPESPEC *inputParams, EXP *args);
TYPE *getTypeFromArrayAccess(EXP *e);

#endif /* !TYPECHECK_H */
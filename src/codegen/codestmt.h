#ifndef CODESTMT_H
#define CODESTMT_H

#include "../ast/tree.h"
#include "../ast/stmt.h"
#include <stdbool.h>

void generateSTMT(STMT *s);
void generateTYPESPEC(TYPESPEC *ts);
void generateSTRUCTSPEC(STRUCTSPEC *ss);
void generateEXPRCASECLAUSE(EXPRCASECLAUSE *ecc);
void generateVarDecl(VARSPEC *vs);
void generateEXP(EXP *e, bool recurse);
void generateEXP_binary(EXP *e, int recurse, char *operator);
void generateEXP_stringCompare(EXP *e, int recurse, char *operator);
char *getStringFromType(TYPE *t, bool isPrimitive);

#endif
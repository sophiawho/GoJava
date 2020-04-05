#ifndef CODESTMT_H
#define CODESTMT_H

#include "../ast/tree.h"
#include "../ast/stmt.h"
#include <stdbool.h>

void generateSTMT(STMT *s);
void generateVarDecl(VARSPEC *vs);
void generateEXP(EXP *e, bool recurse);
void generateEXP_binary(EXP *e, int recurse, char *operator);
char *getStringFromType(TYPE *t, bool isPrimitive);

#endif
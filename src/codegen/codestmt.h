#ifndef CODESTMT_H
#define CODESTMT_H

#include "../ast/tree.h"
#include "../ast/stmt.h"
#include <stdbool.h>

void generateSTMT(STMT *s);
void generateVarDecl(VARSPEC *vs);
void generateEXP(EXP *e, bool recurse);
char *getStringFromType(TYPE *t, bool isPrimitive);

#endif
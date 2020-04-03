#ifndef CODESTMT_H
#define CODESTMT_H

#include "../ast/tree.h"
#include "../ast/stmt.h"

void generateSTMT(STMT *s);
void generateVarDecl(VARSPEC *vs);
void generateEXP(EXP *e);
char *getStringFromType(TYPE *t);

#endif
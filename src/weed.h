#include "ast/tree.h"
#include "ast/stmt.h"

#ifndef WEED_H
#define WEED_H

void weedPROG(PROG *p);
void weedTOPLEVELDECL(TOPLEVELDECL *d);
void weedTYPE(TYPE *t);
void weedSTMT(STMT *s);
void weedEXP(EXP *e);
void weedFUNC(FUNC *f);

#endif
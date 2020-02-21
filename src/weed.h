#include "ast/tree.h"
#include "ast/stmt.h"

#include <stdbool.h>

#ifndef WEED_H
#define WEED_H

void weedPROG(PROG *p);
void weedTOPLEVELDECL(TOPLEVELDECL *d);

void weedFUNC(FUNC *f);
void weedFUNC_checkForReturnStmt(FUNC *f);
void weedFUNC_inputParams(TYPESPEC *ts);
void weedFUNC_inputParams_id(IDENT *id);

void weedSTMT(STMT *s);
void weedSTMT_assign(STMT *s);
void weedSTMT_forLoop(STMT *s);
void weedSTMT_forLoop_postStmt(STMT *s);

void weedEXPRCASECLAUSE(EXPRCASECLAUSE *cc);
void weedEXPRCASECLAUSE_findDefaultCase(EXPRCASECLAUSE *cc, bool foundPreviousDefaultCase);

void weedEXP_eval(EXP *e);
void weedEXP_nonEval(EXP *e, bool allowBlankId);

void weedTYPESPEC(TYPESPEC *ts);
void weedTYPE(TYPE *t);

#endif
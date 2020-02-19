#include "ast/tree.h"
#include "ast/stmt.h"

void pretty_INDENT();

void pretty_PROG(PROG* root);
void pretty_TOPLEVELDECL(TOPLEVELDECL *topLevelDecl);
void pretty_VARSPEC(VARSPEC *varspec);
void pretty_IDENT(IDENT *ident, int cur_i);
void pretty_EXP(EXP *e, int cur_i);
void pretty_TYPE(TYPE *t);
void pretty_FUNC(FUNC *f);
void pretty_STMT(STMT *s);

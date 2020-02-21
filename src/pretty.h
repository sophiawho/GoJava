#include "ast/tree.h"
#include "ast/stmt.h"

void pretty_INDENT();

void pretty_PROG(PROG* root);
void pretty_TOPLEVELDECL(TOPLEVELDECL *topLevelDecl);
void pretty_VARSPEC(VARSPEC *varspec);
void pretty_TYPESPEC(TYPESPEC *ts, int cur_i);
void pretty_STRUCTSPEC(STRUCTSPEC *ss);
void pretty_IDENT(IDENT *ident, int cur_i);
void pretty_EXP(EXP *e, int cur_i);
void pretty_TYPE(TYPE *t);
void pretty_FUNC(FUNC *f);
void pretty_STMT(STMT *s, STMT *parent);
void pretty_assign(AssignKind kind);
void pretty_EXPRCASECLAUSE(EXPRCASECLAUSE *caseClause);

void pretty_indentFirstLineOfStmt(STMT *s, STMT *parent);
void pretty_endLastLineOfStmt(STMT *s, STMT *parent);
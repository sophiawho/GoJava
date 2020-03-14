#ifndef TYPECHECK_H
#define TYPECHECK_H

#include "../ast/tree.h"
#include "../ast/stmt.h"

TYPE checkEXP_binary(EXP *lhs, EXP *rhs);

#endif /* !TYPECHECK_H */
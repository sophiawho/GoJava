#ifndef STMT_H
#define STMT_H

typedef enum {
    k_stmtKindEmpty,
    k_stmtKindExpStmt,
    k_stmtKindIncDec,
    k_stmtKindAssign,
    k_stmtKindPrint,
    k_stmtKindVarDecl, 
    k_stmtKindTypeDecl, 
    k_stmtKindBlock,
    k_stmtKindShortDecl, 
    k_stmtKindIfStmt,
    k_stmtKindSwitch,
    k_stmtKindFor, 
    k_stmtKindBreak,
    k_stmtKindContinue,
    k_stmtKindReturn,
} StatementKind;

typedef enum {
    k_stmtAssign,
    k_stmtColonAssign,
    k_opAssignKindPlusEq,
    k_opAssignKindMinusEq,
    k_opAssignKindMultEq,
    k_opAssignKindDivEq,
    k_opAssignKindModEq,
    k_opAssignKindBitAndEq,
    k_opAssignKindBitOrEq,
    k_opAssignKindBitXorEq,
    k_opAssignKindLeftShiftEq,
    k_opAssignKindRightShiftEq,
    k_opAssignKindBitClearEq,
} AssignKind;

typedef enum {
    k_loopKindInfinite,
    k_loopKindWhile,
    k_loopKindThreePart,
} ForLoopKind;

struct STMT {
    int lineno;
    StatementKind kind;
    union {
        EXP *expStmt;
        // If amount == -1, the stmt is a decrement, elif amount == 1, the stmt is an increment
        struct { EXP *exp; int amount; } incDecStmt; 
        struct { EXP *lhs; AssignKind kind; EXP *rhs; } assignStmt;
        EXP *returnExp;
        STMT *blockStmt;
        VARSPEC *varDecl;
        TYPESPEC *typeDecl;
        // If tPRINTLN, newLine == 1, elif tPRINT, newLine == 0
        struct { EXP *expList; int newLine; } printStmt; 
        struct { STMT *simpleStmt; EXP *condition; STMT *trueBody; STMT *falseBody; } ifStmt;
        struct { ForLoopKind kind; STMT *body; EXP *condition; STMT *initStmt; STMT *postStmt; } forLoop; 
        struct { STMT *simpleStmt; EXP *exp; EXPRCASECLAUSE *caseClauses; } switchStmt;
    } val;
    STMT *next;
};
STMT *getGenericStmt(StatementKind kind);
STMT *makeSTMT_expStmt(EXP *exp);
STMT *makeSTMT_incDec(EXP *exp, int amount);
STMT *makeSTMT_assignStmt(EXP *lhs, AssignKind kind, EXP *rhs);
STMT *makeSTMT_returnStmt(EXP *returnExp);
STMT *makeSTMT_blockStmt(STMT *blockStmt);
STMT *makeSTMT_varDecl(VARSPEC *varDecl);
STMT *makeSTMT_typeDecl(TYPESPEC *typeDecl);
STMT *makeSTMT_print(EXP *expList, int newLine);
STMT *makeSTMT_block(STMT *stmt);
STMT *makeSTMT_if(STMT *simpleStmt, EXP *condition, STMT *trueBody, STMT *falseBody);
STMT *makeSTMT_for(ForLoopKind kind, STMT *body, EXP *condition, STMT *initStmt, STMT *postStmt); 
STMT *makeSTMT_switch(STMT *simpleStmt, EXP *exp, EXPRCASECLAUSE *caseClauses);

typedef enum {
    k_caseClause,
    k_defaultClause,
} CaseClauseKind;

struct EXPRCASECLAUSE {
    int lineno;
    CaseClauseKind kind;
    EXP *expList;
    STMT *stmtList;
    EXPRCASECLAUSE *next;
};
EXPRCASECLAUSE *makeExprCaseClause(CaseClauseKind kind, EXP *expList, STMT *stmtList);

#endif /* STMT_H */
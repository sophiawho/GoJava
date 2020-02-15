#ifndef TREE_H
#define TREE_H

typedef struct PROG PROG;
typedef struct TOPLEVELDECL TOPLEVELDECL;
typedef struct TYPE TYPE;
typedef struct STMT STMT;
typedef struct EXP EXP;
typedef struct FUNC FUNC;

// Helper structs
typedef struct IDENT_LIST IDENT_LIST;
typedef struct EXP_LIST EXP_LIST;

struct IDENT_LIST {
    char *ident;
    IDENT_LIST *next;
}
IDENT_LIST *makeIdentList(IDENT_LIST *list, char *ident);

struct EXP_LIST {
    EXP *exp;
    EXP_LIST *next;
}
EXP_LIST *makeExpList(EXP_LIST *list, EXP *exp);

// TODO: Figure out how parser will call AST for vars with multiple identifiers

typedef enum {
    k_typeInt,
    k_typeFloat,
    k_typeBool,
    k_typeRune,
    k_typeString,
    k_typeSlice,
    k_typeArray,
    k_typeStruct,
    k_typeInfer,
} TypeKind;

/*
Support Types that have structs with multiple fields, ie:
type point struct {
    x, y float 64
}
where "float" could be another struct

Support type int float
Each type has a name: "int" would be the name, but float will be the type
*/
struct TYPE {
    TypeKind kind;
    union {
        struct { char *attribute; TYPE *type; structVal *next; } structType; // TODO: Figure out best struct for structType after looking at parser
        struct { int size; TYPE *type; } arrayType;
    } val;
}; 
TYPE *makeTYPE(TypeKind kind, char *attributes[], TYPE *types[]);

struct PROG {
    char *pkgIdent; // A package declaration is the key word package followed by an identifier
    TOPLEVELDECL *root;
};
PROG *makePROG(char *package, TOPLEVELDECL *root);

typedef enum {
    k_topLevelDeclVar,
    k_topLevelDeclType,
    k_topLevelDeclFunc,
} TopLevelDeclKind;

struct TOPLEVELDECL {
    int lineno;
    TopLevelDeclKind kind;
    union {
        struct { char *ident; EXP *rhs; TYPE *type; } varDecl;
        struct { char *typeName; TYPE *type; } typeDecl;
        FUNC *funcDecl;
    } val;
    TOPLEVELDECL *next;
};
TOPLEVELDECL *makeTopLevelDecl_var(char *ident, EXP *rhs, TYPE *type); // may pass in an IDENT, EXP list
TOPLEVELDECL *makeTopLevelDecl_type(char *typeName; TYPE *type); 
TOPLEVELDECL *makeTopLevelDecl_func(FUNC *funcdecl);

struct FUNC {
    int lineno;
    STMT *root;
    union {
        struct { char *ident; TYPE *type; inputParameter *next; } inputParameter;
        TYPE *returnType;
    } val;
};

typedef enum {
    k_stmtKindPrint, // bool for printLn
    k_stmtKindIncDec,
    k_stmtKindVarDecl, 
    k_stmtKindTypeDecl, 
    k_stmtKindBlock,
    k_stmtKindShortDecl, // IdentifierList, ExpressionList
    k_stmtKindAssign, // TODO: The LHS and RHS can both be lists - the numIdentifiers must match the numExpressions
    k_stmtKindIfStmt,
    k_stmtKindWhile,
    k_stmtKindSwitch, // TODO
    k_stmtKindFor, 
    k_stmtKindBreak,
    k_stmtKindContinue,
    k_stmtKindReturn,
} StatementKind;

struct STMT {
    int lineno;
    StatementKind kind;
    union {
        struct { EXP_LIST *expList;  bool newLine; } printStmt;
        struct { EXP *exp; int amount } incDecStmt; // If amount == -1, the statement is a decrement; if amount == 1, the statement is an increment
        struct { char *ident; EXP *rhs; TYPE *type; } varDecl;
        struct { char *typeName; TYPE *type; } typeDecl;
        struct { char *ident; EXP *rhs; } shortDecl;
        // TODO: Add structs for each kind of stmt
        struct { STMT *stmt } blockStmt;
        struct { EXP *condition; STMT *trueBody; STMT *falseBody; } ifStmt;
		struct { EXP *condition; STMT *body; } whileStmt;
        struct { char type; EXP *condition; STMT *initStmt; STMT *postStmt} forLoop; // type: 'i' = infinite, 'w' = while, 2 = '3' = 3 part loop
        EXP_LIST *returnExpList;
    } val;
    STMT *next;
}
STMT *makeSTMT_print(EXP_LIST *expList, bool newLine);
STMT *makeSTMT_incDec(EXP *exp, int amount);
STMT *makeSTMT_block(STMT *stmt);
STMT *makeSTMT_if(EXP *condition, STMT *trueBody, STMT *falseBody);
STMT *makeSTMT_while(EXP *condition, STMT *body);
STMT *makeSTMT_return(EXP_LIST *returnExpList);
// TODO: Write function signatures

typedef enum {
    k_expKindIdentifier,
    k_expKindIntLiteral,
    k_expKindFloatLiteral,
    k_expKindBoolLiteral,
    k_expKindRuneLiteral,
    k_expKindStringLiteral,
    // Binary operators
    k_expKindAnd,
    k_expKindOr,
    // Relational binary operators
    k_expKindEq,
    k_expKindNotEq,
    k_expKindLess,
    k_expKindLessEq,
    k_expKindGrtr,
    k_expKindGrtrEq,
    // Additive binary operators
    k_expKindAdd,
    k_expKindMinus,
    k_expKindBitOr,
    k_expKindBitXOR,
    // Multiplicative binary operators
    k_expKindMult,
    k_expKindDiv,
    k_expKindMod,
    k_expKindLeftShift,
    k_expKindRightShift,
    k_expKindBitAnd,
    k_expKindBitClear,
    // Golite supports four unary operators
    k_expKindUPlus,
    k_expKindUMinus,
    k_expKindBang,
    k_expKindUBitXOR, // ATTN: This is similarly named to binary op k_expKindBitXOR
    k_expKindParentheses,
    k_expKindFuncCall,
    k_expKindIndex, // Index into arrays and slices, eg: a := x[0]
} ExpressionKind;

struct EXP {
    int lineno;
    ExpressionKind kind;
    TYPE *type;
    union {
        struct { char *ident; SYMBOL *idsym; } identExp;
		int intLiteral;
		float floatLiteral;
		bool boolLiteral;
		char runeLiteral;
        char *stringLiteral;
		struct { EXP *lhs; EXP *rhs; } binary;
		struct { EXP *rhs; } unary;
        struct { char *funcName; IDENT_LIST *args; } funcCall;
        struct { char *ident; int indexNum; } indexExp; 
    } val;
};
EXP *makeEXP_identifier(char *id);
EXP *makeEXP_intLiteral(int intLiteral);
EXP *makeEXP_floatLiteral(float floatLiteral);
EXP *makeEXP_boolLiteral(int boolLiteral); // 0 for False, 1 for True
EXP *makeEXP_runeLiteral(char runeLiteral);
EXP *makeEXP_stringLiteral(char *stringLiteral);
EXP *makeEXP_binary(ExpressionKind op, EXP *lhs, EXP *rhs);
EXP *makeEXP_unary(ExpressionKind op, EXP *rhs);
EXP *makeEXP_funcCall(char *funcName, IDENT_LIST *args);
EXP *makeEXP_index(char *ident, int index);

#endif /* !TREE_H */
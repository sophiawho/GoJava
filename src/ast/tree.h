#ifndef TREE_H
#define TREE_H

// Main AST nodes
typedef struct PROG PROG;
typedef struct TOPLEVELDECL TOPLEVELDECL;
typedef struct TYPE TYPE;
typedef struct STMT STMT; // Refer to stmt.c stmt.h
typedef struct EXP EXP;
typedef struct FUNC FUNC;

// Helper list structs with next pointer
typedef struct IDENT IDENT;
typedef struct VARSPEC VARSPEC;
typedef struct TYPESPEC TYPESPEC;
typedef struct STRUCTSPEC STRUCTSPEC;
typedef struct EXPRCASECLAUSE EXPRCASECLAUSE; // Refer to stmt.c stmt.h

// Symbol
typedef struct SYMBOL SYMBOL;

struct PROG {
    char *package; // A package declaration is the key word package followed by an identifier
    TOPLEVELDECL *rootTopLevelDecl;
};
PROG *makePROG(char *package, TOPLEVELDECL *rootTopLevelDecl);

typedef enum {
    k_topLevelDeclVar,
    k_topLevelDeclType,
    k_topLevelDeclFunc,
} TopLevelDeclKind;

struct TOPLEVELDECL {
    int lineno;
    TopLevelDeclKind kind;
    union {
        VARSPEC *varDecl;
        TYPESPEC *typeDecl;
        FUNC *funcDecl;
    } val;
    TOPLEVELDECL *next;
};
TOPLEVELDECL *makeTopLevelDecl_var(VARSPEC *varspec);
TOPLEVELDECL *makeTopLevelDecl_type(TYPESPEC *typedecl);
TOPLEVELDECL *makeTopLevelDecl_func(FUNC *funcdecl);

typedef enum {
    k_typeSlice,
    k_typeArray,
    k_typeStruct,
    k_typeInfer,
    k_typeInt,
    k_typeFloat,
    k_typeBool,
    k_typeRune,
    k_typeString
} TypeKind;

// type a []int
// type b []int
// type c []int
// type d c

// var _a c
// var _b d

// var x_1 a
//     kind -> k_typeLiteral
//     a -> identifier

// var x_2 b
//     kind -> k_typeLiteral
//     b -> identifier

// var x_3 []a

// var x_4 []a

// TYPE *t_x_1 
// TYPE *t_x_2
//     t_x_1.kind == t_x_2.kind && strmcp(t_x_1.identifier, t_x_2.identifier) == 0


// var x1 []int
//     [] -> k_typeSlice
//     int -> identifier

// var x2 []int
//     [] -> k_typeSlice
//     int -> identifier



// var x3 a
// x3 != x1

// k_typeSlice

// compare(vs1->type, vs2->type)
// {
//     strcmp(type1, type2) == 0
// }

// type b a
// var x a
// var y b

// SYMBOL TABLE
// SYMBOL   TYPE 
// x        *some_pointer1
// y        *some_pointer2

// while b.parent != null, b==a, b=b.parent


struct TYPE {
    TypeKind kind;
    int lineno;
    union {
        char *identifier;
        STRUCTSPEC *structType;
        struct { TYPE *type; } sliceType;
        struct { int size; TYPE *type; } arrayType;
    } val;
    char *typeName; // for symbol table
    TYPE *parent;
}; 
TYPE *makeTYPE_ident(char *identifier); 
TYPE *makeTYPE_array(int size, TYPE *type); 
TYPE *makeTYPE_slice(TYPE *type);
TYPE *makeTYPE_struct(STRUCTSPEC *ss);
TYPE *makeTYPE(TypeKind kind);

typedef enum {
    k_expKindIdentifier,
    // Literals
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
    k_expKindFuncCall,
    k_expKindArrayAccess,
    k_expKindFieldAccess,
    // Builtins
    k_expKindAppend,
    k_expKindLen,
    k_expKindCap,
    // Parenthesized expressions
    k_expKindUParenthesized
} ExpressionKind;

struct EXP {
    int lineno;
    ExpressionKind kind;
    TYPE *type;
    union {
        struct { char *ident; } identExp;
		int intLiteral;
		float floatLiteral;
		int boolLiteral;
		char runeLiteral;
        char *stringLiteral;
		struct { EXP *lhs; EXP *rhs; } binary;
		struct { EXP *rhs; } unary;
        struct { EXP *slice; EXP *addend; } append;
        EXP *lenExp;
        EXP *capExp;
        struct { EXP *primaryExpr; EXP *expList; } funcCall;
        struct { EXP *arrayReference; EXP *indexExp; } arrayAccess;
        struct { EXP *object; char *field; } fieldAccess;
    } val;
    EXP *next;
};
EXP *getGenericExpr(ExpressionKind kind);
EXP *makeEXP_identifier(char *id);
EXP *makeEXP_intLiteral(int intLiteral);
EXP *makeEXP_floatLiteral(float floatLiteral);
EXP *makeEXP_boolLiteral(int boolLiteral); // 0 for False, 1 for True
EXP *makeEXP_runeLiteral(char runeLiteral);
EXP *makeEXP_stringLiteral(char *stringLiteral);
EXP *makeEXP_binary(ExpressionKind op, EXP *lhs, EXP *rhs);
EXP *makeEXP_unary(ExpressionKind op, EXP *rhs);
EXP *makeEXP_append(EXP *slice, EXP *addend);
EXP *makeEXP_len(EXP *lenExp);
EXP *makeEXP_cap(EXP *capExp);
EXP *makeEXP_funcCall(EXP *primaryExpr, EXP *expList);
EXP *makeEXP_arrayAccess(EXP *arrayReference, EXP *indexExp);
EXP *makeEXP_fieldAccess(EXP *object, char *field);

struct FUNC {
    int lineno;
    char *name;
    TYPE *returnType; // optional
    TYPESPEC *inputParams; // optional
    STMT *rootStmt;
};
FUNC *makeFunc(char *name, TYPE *returnType, TYPESPEC *inputParams, STMT *rootStmt);

struct IDENT {
    char *ident;
    IDENT *next;
};
IDENT *makeIDENT(char *ident);

struct VARSPEC {
    int lineno;
    IDENT *ident;
    EXP *rhs;
    TYPE *type;
    VARSPEC *next;
};
VARSPEC *makeVarSpec(IDENT *ident, EXP *rhs, TYPE *type);

typedef enum {
    k_typeSpecKindTypeDeclaration,
    k_typeSpecKindParameterList
} TypeSpecKind;

struct TYPESPEC {
    int lineno;
    TypeSpecKind kind;
    IDENT *ident;
    TYPE *type;
    TYPESPEC *next;
};
TYPESPEC *makeTypeSpec(TypeSpecKind kind, IDENT *ident, TYPE *type);
//TYPESPEC *makeTypeSpec_struct(TypeSpecKind kind, IDENT *ident, STRUCTSPEC *ss);

struct STRUCTSPEC {
    IDENT *attribute; // can have multiple attributes per type
    TYPE *type;
    STRUCTSPEC *next;
};
STRUCTSPEC *makeStructSpec(IDENT *attribute, TYPE *type);

typedef enum {
  k_symbolKindVar,
  k_symbolKindType,
  k_symbolKindFunc,
  k_symbolKindConstant
} SymbolKind;

struct SYMBOL {
    char *name;
    SymbolKind kind;
    int shadowNum;
    union {
        TYPE* type;
        VARSPEC* varSpec; // var decl and shortvardecl
        FUNC* funcSpec;
    } val;
    struct SYMBOL *next;
};

#endif /* !TREE_H */
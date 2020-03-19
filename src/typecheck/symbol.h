#ifndef SYMBOL_H
#define SYMBOL_H

#include "../ast/tree.h"
#include "../ast/stmt.h"
#include <stdbool.h>

#define HASH_SIZE 317

int print_sym_table;

typedef struct SymbolTable {
    SYMBOL *table[HASH_SIZE];
    struct SymbolTable *parent;
} SymbolTable;

void makeSymbolTable(PROG *root);

// Include lineno so that any error message can reference the line number
SYMBOL *putSymbol(SymbolTable *t, char *name, SymbolKind kind, int lineno);
SYMBOL *putSymbol_Func(SymbolTable *t, char *name, FUNC *funcSpec, int lineno);
SYMBOL *putSymbol_Type(SymbolTable *t, char *name, TYPE *type, int lineno);
SYMBOL *putSymbol_Var(SymbolTable *t, char *name, VARSPEC *varSpec, int lineno);
SYMBOL *getSymbol(SymbolTable *t, char *name, int lineno);

SymbolTable *initSymbolTable();
SymbolTable *scopeSymbolTable(SymbolTable *parent);

void symTOPLEVELDECL(TOPLEVELDECL *tld, SymbolTable *symTable);
void symFUNC(FUNC *f, SymbolTable *symTable);
void symSTMT(STMT *s, SymbolTable *scope);
void symSTMT_assign(STMT *s, SymbolTable *scope);
void symSTMT_forLoop(STMT *s, SymbolTable *scope);
void symTYPESPEC(TYPESPEC *ts, SymbolTable *symTable);
void symVARSPEC(VARSPEC *vs, SymbolTable *scope);
void symEXP(EXP *exp, SymbolTable *scope);
void symEXPRCASECLAUSE(EXPRCASECLAUSE *cc, SymbolTable *scope);

// Helper functions
void putBaseTypeSymbols(SymbolTable *symTable);
void putBaseConstants(SymbolTable *sym, char *ident, int boolValue);
void putBaseTypeSpec(SymbolTable *symTable, char *typeName, TypeKind typeKind);

TYPE *findParentType(SymbolTable *symTable, TYPE *t);
TYPE *findFieldTypeForStruct(SymbolTable *symTable, TYPE *t);

void printIndent();
void openScope();
void closeScope();
void printSymbol(SYMBOL *s);
void printType(TYPE *t);
int Hash(char *str);

#endif /* !SYMBOL_H */

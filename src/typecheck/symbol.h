#ifndef SYMBOL_H
#define SYMBOL_H

#include "../ast/tree.h"
#include "../ast/stmt.h"

#define HASH_SIZE 317

int print_sym_table = 0;

typedef struct Symbol {
    char *name;
    TYPE *type;
    struct Symbol *next;
} Symbol;

typedef struct SymbolTable {
    Symbol *table[HASH_SIZE];
    struct SymbolTable *parent;
} SymbolTable;

void makeSymbolTable(PROG *root);

// Include lineno so that any error message can reference the line number
Symbol *putSymbol(SymbolTable *t, char *name, TYPE *type, int lineno);
Symbol *getSymbol(SymbolTable *t, char *name);

SymbolTable *initSymbolTable();
SymbolTable *scopeSymbolTable(SymbolTable *parent);

#endif /* !SYMBOL_H */
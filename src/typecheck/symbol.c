#include "symbol.h"
#include "../ast/tree.h"
#include "../ast/stmt.h"
#include "../error.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern int print_sym_table;
SymbolTable *initSymbolTable()
{
    SymbolTable *t = malloc(sizeof(SymbolTable));

    for (int i = 0; i < HASH_SIZE; i++)
    {   
        t->table[i] = NULL;
    }
    t->parent = NULL;
    return t;
}

SymbolTable *scopeSymbolTable(SymbolTable *parent)
{
    SymbolTable *scope = initSymbolTable();
    scope->parent = parent;
    return scope;
}

int Hash(char *str)
{
    unsigned int hash = 0;
    while (*str) hash = (hash << 1) + *str++;
    return hash % HASH_SIZE;
}

Symbol *getSymbol(SymbolTable *t, char *name)
{
    int hash = Hash(name);

    for (Symbol *s = t->table[hash]; s; s = s->next) 
    {
        if (strcmp(s->name, name) == 0) return s;
    }

    if (t->parent == NULL) return NULL;
    return getSymbol(t->parent, name);
}

Symbol *getSymbolFromScope(SymbolTable *t, char *name)
{
    int hash = Hash(name);

    for (Symbol *s = t->table[hash]; s; s = s->next) 
    {
        if (strcmp(s->name, name) == 0) return s;
    }
    return NULL;
}

Symbol *putSymbol(SymbolTable *t, char *name, TYPE *type, int lineno)
{
    int hash = Hash(name);
    for (Symbol *s = t->table[hash]; s; s->next)
    {   
        if (strcmp(s->name, name) == 0) throwErrorRedeclaredId(lineno, s->name);
    }

    Symbol *s = malloc(sizeof(Symbol));
    s->name = name;
    s->type = type;
    s->next = t->table[hash];
    t->table[hash] = s;
    return s;
}

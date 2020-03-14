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
SYMBOL *getSymbol(SymbolTable *t, char *name);

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

void printIndent();
void openScope();
void closeScope();
void printSymbol(SYMBOL *s);
void printType(TYPE *t);

#endif /* !SYMBOL_H */

/**
REFERENCE COMPILER INPUT:
package test

var aba int

func main() {
	type num int
	type natural num

	var x int = 7;
	var y num = num(7);
	var z natural = natural(7);

	type floats []float64
	type naturals []natural

	{
		type new []naturals
	}

	var a floats
	a = append(a, 7.2)
	var b naturals
	b = append(b, natural(8))
}
---------------------------------------------------------------------
REFERENCE COMPILER OUTPUT:
{
	int [type] = int
	float64 [type] = float64
	bool [type] = bool
	rune [type] = rune
	string [type] = string
	true [constant] = bool
	false [constant] = bool
	{
		aba [variable] = int
		main [function] = () -> void
		{
			num [type] = num -> int
			natural [type] = natural -> num -> int
			x [variable] = int
			y [variable] = num
			z [variable] = natural
			floats [type] = floats -> []float64
			naturals [type] = naturals -> []natural
			a [variable] = floats
			b [variable] = naturals
		}
	}
}

*/
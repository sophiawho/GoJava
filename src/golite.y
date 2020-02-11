%{
#include <stdio.h>
#include <stdlib.h>

// #include "tree.h"

extern int yylineno;
// extern EXP *root;

int g_tokens = 1;

int yylex();
void yyerror(const char *s) { fprintf(stderr, "Error: (line %d) %s\n", yylineno, s); exit(1); }
%}

%locations
%error-verbose

%code requires
{
	// #include "tree.h"
}

%union {
	int int_val;
	char *string_val;
	// EXP *exp;
}

// %type <exp> program exp

%token <int_val> tINTVAL
%token <string_val> tIDENTIFIER 

%left '+' '-'
%left '*' '/'

%start program

%% 
program: tINTVAL;

%%

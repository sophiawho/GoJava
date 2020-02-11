%{
#include <stdio.h>
#include <string.h>

#include "parser.tab.h"
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;

extern int g_tokens;
%}

/* Helper definitions */
DIGIT [0-9]
LETTER [a-zA-Z_]
SYMBOL [!#$%&()*+,-./:;<=>?@[]\^_`{}|~]

%option yylineno
%option noinput
%option nounput
%x MULTILINE_COMMENT

%%

"//".*

"/*"					{ BEGIN(MULTILINE_COMMENT); }
<MULTILINE_COMMENT>"*/" { BEGIN(INITIAL); }
<MULTILINE_COMMENT>. 	{ }
<MULTILINE_COMMENT>\n 	{ }

[ \t\n\r]+

break { if (g_tokens) printf("tBREAK\n"); }
case { if (g_tokens) printf("tCASE\n"); }
chan { if (g_tokens) printf("tCHAN\n"); }
const { if (g_tokens) printf("tCONST\n"); }
continue { if (g_tokens) printf("tCONTINUE\n"); }
default { if (g_tokens) printf("tDEFAULT\n"); }
defer { if (g_tokens) printf("tDEFER\n"); }
else { if (g_tokens) printf("tELSE\n"); }
fallthrough { if (g_tokens) printf("tFALLTHROUGH\n"); }
for { if (g_tokens) printf("tFOR\n"); }
func { if (g_tokens) printf("tFUNC\n"); }
go { if (g_tokens) printf("tGO\n"); }
goto { if (g_tokens) printf("tGOTO\n"); }
if { if (g_tokens) printf("tIF\n"); }
import { if (g_tokens) printf("tIMPORT\n"); }
interface { if (g_tokens) printf("tINTERFACE\n"); }
map { if (g_tokens) printf("tMAP\n"); }
package { if (g_tokens) printf("tPACKAGE\n"); }
range { if (g_tokens) printf("tRANGE\n"); }
return { if (g_tokens) printf("tRETURN\n"); }
select { if (g_tokens) printf("tSELECT\n"); }
struct { if (g_tokens) printf("tSTRUCT\n"); }
switch { if (g_tokens) printf("tSWITCH\n"); }
type { if (g_tokens) printf("tTYPE\n"); }
var { if (g_tokens) printf("tVAR\n"); }

"+"      { if (g_tokens) printf("+\n"); }
"-"      { if (g_tokens) printf("-\n"); }
"*"      { if (g_tokens) printf("*\n"); }
"/"      { if (g_tokens) printf("/\n"); }
"%"      { if (g_tokens) printf("%%\n"); } // escape % with %%
"&"      { if (g_tokens) printf("&\n"); }
"|"      { if (g_tokens) printf("|\n"); }
"^"      { if (g_tokens) printf("^\n"); }
"<<"     { if (g_tokens) printf("<<\n"); }
">>"     { if (g_tokens) printf(">>\n"); }
"&^"     { if (g_tokens) printf("&^\n"); }
"+="     { if (g_tokens) printf("+=\n"); }
"-="     { if (g_tokens) printf("-=\n"); }
"*="     { if (g_tokens) printf("*=\n"); }
"/="     { if (g_tokens) printf("/=\n"); }
"%="     { if (g_tokens) printf("%%=\n"); } // escape % with %%
"&="     { if (g_tokens) printf("&=\n"); }
"|="     { if (g_tokens) printf("|=\n"); }
"^="     { if (g_tokens) printf("^=\n"); }
"<<="    { if (g_tokens) printf("<<=\n"); }
">>="    { if (g_tokens) printf(">>=\n"); }
"&^="    { if (g_tokens) printf("&^=\n"); }
"&&"     { if (g_tokens) printf("&&\n"); }
"||"     { if (g_tokens) printf("||\n"); }
"<-"     { if (g_tokens) printf("<-\n"); }
"++"     { if (g_tokens) printf("++\n"); }
"--"     { if (g_tokens) printf("--\n"); }
"=="     { if (g_tokens) printf("==\n"); }
"<"      { if (g_tokens) printf("<\n"); }
">"      { if (g_tokens) printf(">\n"); }
"="      { if (g_tokens) printf("=\n"); }
"!"      { if (g_tokens) printf("!\n"); }
"!="     { if (g_tokens) printf("!=\n"); }
"<="     { if (g_tokens) printf("<=\n"); }
">="     { if (g_tokens) printf(">=\n"); }
":="     { if (g_tokens) printf(":=\n"); }
"..."    { if (g_tokens) printf("...\n"); }
"("      { if (g_tokens) printf("(\n"); }
"["      { if (g_tokens) printf("[\n"); }
"{"      { if (g_tokens) printf("{\n"); }
","      { if (g_tokens) printf(",\n"); }
"."      { if (g_tokens) printf(".\n"); }
")"      { if (g_tokens) printf(")\n"); }
"]"      { if (g_tokens) printf("]\n"); }
"}"      { if (g_tokens) printf("}\n"); }
";"      { if (g_tokens) printf(";\n"); }
":"      { if (g_tokens) printf(":\n"); }

0|([1-9][0-9]*) {
	yylval.int_val = atoi(yytext);
	return tINTVAL;
}

[a-zA-Z_][a-zA-Z0-9_]* { 
	yylval.string_val = strdup(yytext);
	return tIDENTIFIER;
}

.       { fprintf(stderr, "Error: (line %d) unexpected character '%s'\n", yylineno, yytext); exit(1); }

%%

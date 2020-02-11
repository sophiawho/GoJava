%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.tab.h"
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;

extern int g_tokens;

/* Helper code */
char *get_raw_string(char *str) {
	char *raw = malloc((2 * strlen(str) + 1)*sizeof(char));
	int i = 0;
	int j = 0;
	while (str[i] != '\0') {
		if (str[i] == '\\') {
			raw[j] = '\\';
			j++;
		}
		raw[j] = str[i];
		i++; j++;
	}
	raw[j] = '\0';
	return raw;
}

%}

/* Helper definitions */
DIGIT [0-9]
LETTER [a-zA-Z_]
SYMBOL []!#$%&()*+,-./:;<=>?@[]\^_`{}|~
ESCAPE [atbvfnr\\']

%option yylineno
%option noinput
%option nounput
%x MULTILINE_COMMENT

%%

"//".*

"/*"					{ BEGIN(MULTILINE_COMMENT); }
<MULTILINE_COMMENT>"*/" { BEGIN(INITIAL); }
<MULTILINE_COMMENT><<EOF>> { fprintf(stderr, "Error: (line %d) Unclosed multi-line comment.\n", yylineno); exit(1); }
<MULTILINE_COMMENT>. 	{ }
<MULTILINE_COMMENT>\n 	{ }

[ \t\n\r]+

break 		{ if (g_tokens) printf("tBREAK\n"); }
case 		{ if (g_tokens) printf("tCASE\n"); }
chan 		{ if (g_tokens) printf("tCHAN\n"); }
const 		{ if (g_tokens) printf("tCONST\n"); }
continue 	{ if (g_tokens) printf("tCONTINUE\n"); }
default 	{ if (g_tokens) printf("tDEFAULT\n"); }
defer 		{ if (g_tokens) printf("tDEFER\n"); }
else 		{ if (g_tokens) printf("tELSE\n"); }
fallthrough { if (g_tokens) printf("tFALLTHROUGH\n"); }
for 		{ if (g_tokens) printf("tFOR\n"); }
func 		{ if (g_tokens) printf("tFUNC\n"); }
go 			{ if (g_tokens) printf("tGO\n"); }
goto 		{ if (g_tokens) printf("tGOTO\n"); }
if 			{ if (g_tokens) printf("tIF\n"); }
import 		{ if (g_tokens) printf("tIMPORT\n"); }
interface 	{ if (g_tokens) printf("tINTERFACE\n"); }
map 		{ if (g_tokens) printf("tMAP\n"); }
package 	{ if (g_tokens) printf("tPACKAGE\n"); }
range 		{ if (g_tokens) printf("tRANGE\n"); }
return 		{ if (g_tokens) printf("tRETURN\n"); }
select 		{ if (g_tokens) printf("tSELECT\n"); }
struct 		{ if (g_tokens) printf("tSTRUCT\n"); }
switch 		{ if (g_tokens) printf("tSWITCH\n"); }
type 		{ if (g_tokens) printf("tTYPE\n"); }
var 		{ if (g_tokens) printf("tVAR\n"); }

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

0|([1-9]{DIGIT}*) { // Decimal Integer
    if (g_tokens) printf("tINTVAL(%s)\n", yytext);
    yylval.intval = atoi(yytext);
    return tINTVAL;
}

0[0-7][0-7]* { // Octal integer
	if (g_tokens) printf("tINTVAL(%ld)\n", strtol(yytext, NULL, 8));
	yylval.intval = strtol(yytext, NULL, 8);
	return tINTVAL;
}

0(x|X)[0-9a-fA-F][0-9a-fA-F]* { // Hexadecimal integer
	if (g_tokens) printf("tINTVAL(%ld)\n", strtol(yytext, NULL, 16));
	yylval.intval = strtol(yytext, NULL, 16);
	return tINTVAL;
}

({DIGIT}*)\.{DIGIT}+ { // Float value, ie: .12
	if (g_tokens) printf("tFLOATVAL(%s)\n", yytext);
    yylval.floatval = atof(yytext);
    return tFLOATVAL;
}

({DIGIT}+)\.{DIGIT}* { // Float value, ie: 12.
	if (g_tokens) printf("tFLOATVAL(%s)\n", yytext);
    yylval.floatval = atof(yytext);
    return tFLOATVAL;
}

\'([^']|\\{ESCAPE})\' { // Rune literal (char literal)
	if (g_tokens) printf("tRUNEVAL(%s)\n", yytext);
	if (yyleng == 4) {
		switch (yytext[2]) {
			case 'a':
				yylval.runeval = '\a'; return tRUNEVAL;
			case 't':
				yylval.runeval = '\t'; return tRUNEVAL;
			case 'b':
				yylval.runeval = '\b'; return tRUNEVAL;
			case 'v':
				yylval.runeval = '\v'; return tRUNEVAL;
			case 'f':
				yylval.runeval = '\f'; return tRUNEVAL;
			case 'n':
				yylval.runeval = '\n'; return tRUNEVAL;
			case 'r':
				yylval.runeval = '\r'; return tRUNEVAL;
			case '\\':
				yylval.runeval = '\\'; return tRUNEVAL;
			case '\'':
				yylval.runeval = '\''; return tRUNEVAL;
		}
	}
	yylval.runeval = yytext[1];
	return tRUNEVAL;
}

\"(\\.|[^\\"])*\" { // Interpreted string literal
    if (g_tokens) printf("tSTRVAL(%s)\n", yytext);
    yylval.strval = strdup(yytext);
    return tSTRVAL;
}

`[^`]*` { // Raw string literal
	if (g_tokens) printf("tSTRVAL(%s)\n", get_raw_string(yytext));
	yylval.strval = get_raw_string(yytext); 
	return tSTRVAL;
}

.       { fprintf(stderr, "Error: (line %d) unexpected character '%s'\n", yylineno, yytext); exit(1); }

%%

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.tab.h"
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;

extern int g_tokens;

int lastToken = -1;
#define RETURN(token) lastToken = token; return token;

int needsSemiColon() {
    return lastToken == tIDENTIFIER ||
    lastToken == tINTVAL ||
    lastToken == tFLOATVAL ||
    lastToken == tRUNEVAL ||
    lastToken == tSTRVAL ||
    lastToken == tBREAK ||
    lastToken == tCONTINUE ||
    lastToken == tFALLTHROUGH ||
    lastToken == tRETURN ||
    lastToken == tINCREMENT ||
    lastToken == tDECREMENT ||
    lastToken == tRPAR ||
    lastToken == tRBRACKET ||
    lastToken == tRBRACE;
}

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

"/*"					    { BEGIN(MULTILINE_COMMENT); }
<MULTILINE_COMMENT>"*/"     { BEGIN(INITIAL); }
<MULTILINE_COMMENT><<EOF>>  { fprintf(stderr, "Error: (line %d) Unclosed multi-line comment.\n", yylineno); exit(1); }
<MULTILINE_COMMENT>. 	    { }
<MULTILINE_COMMENT>\n 	    { }

[ \t\r]+
\n {
    if (needsSemiColon()) {
        if (g_tokens) printf(";\n");
        RETURN(';');
    }
}

break 		{ if (g_tokens) printf("tBREAK\n"); RETURN(tBREAK); }
case		{ if (g_tokens) printf("tCASE\n"); RETURN(tCASE); }
chan { if (g_tokens) printf("tCHAN\n"); RETURN(tCHAN); }
const { if (g_tokens) printf("tCONST\n"); RETURN(tCONST); }
continue { if (g_tokens) printf("tCONTINUE\n"); RETURN(tCONTINUE); }
default { if (g_tokens) printf("tDEFAULT\n"); RETURN(tDEFAULT); }
defer { if (g_tokens) printf("tDEFER\n"); RETURN(tDEFER); }
else { if (g_tokens) printf("tELSE\n"); RETURN(tELSE); }
fallthrough { if (g_tokens) printf("tFALLTHROUGH\n"); RETURN(tFALLTHROUGH); }
for { if (g_tokens) printf("tFOR\n"); RETURN(tFOR); }
func { if (g_tokens) printf("tFUNC\n"); RETURN(tFUNC); }
go { if (g_tokens) printf("tGO\n"); RETURN(tGO); }
goto { if (g_tokens) printf("tGOTO\n"); RETURN(tGOTO); }
if { if (g_tokens) printf("tIF\n"); RETURN(tIF); }
import { if (g_tokens) printf("tIMPORT\n"); RETURN(tIMPORT); }
interface { if (g_tokens) printf("tINTERFACE\n"); RETURN(tINTERFACE); }
map { if (g_tokens) printf("tMAP\n"); RETURN(tMAP); }
package { if (g_tokens) printf("tPACKAGE\n"); RETURN(tPACKAGE); }
range { if (g_tokens) printf("tRANGE\n"); RETURN(tRANGE); }
return { if (g_tokens) printf("tRETURN\n"); RETURN(tRETURN); }
select { if (g_tokens) printf("tSELECT\n"); RETURN(tSELECT); }
struct { if (g_tokens) printf("tSTRUCT\n"); RETURN(tSTRUCT); }
switch { if (g_tokens) printf("tSWITCH\n"); RETURN(tSWITCH); }
type { if (g_tokens) printf("tTYPE\n"); RETURN(tTYPE); }
var { if (g_tokens) printf("tVAR\n"); RETURN(tVAR); }

"+"      { if (g_tokens) printf("tADD\n"); RETURN(tADD); }
"-"      { if (g_tokens) printf("tMINUS\n"); RETURN(tMINUS); }
"*"      { if (g_tokens) printf("tMULT\n"); RETURN(tMULT); }
"/"      { if (g_tokens) printf("tDIV\n"); RETURN(tDIV); }
"%"      { if (g_tokens) printf("tMOD\n"); RETURN(tMOD); }
"&"      { if (g_tokens) printf("tBITAND\n"); RETURN(tBITAND); }
"|"      { if (g_tokens) printf("tBITOR\n"); RETURN(tBITOR); }
"^"      { if (g_tokens) printf("tBITXOR\n"); RETURN(tBITXOR); }
"<<"     { if (g_tokens) printf("tLEFTSHIFT\n"); RETURN(tLEFTSHIFT); }
">>"     { if (g_tokens) printf("tRIGHTSHIFT\n"); RETURN(tRIGHTSHIFT); }
"&^"     { if (g_tokens) printf("tBITCLEAR\n"); RETURN(tBITCLEAR); }
"+="     { if (g_tokens) printf("tPLUSEQ\n"); RETURN(tPLUSEQ); }
"-="     { if (g_tokens) printf("tMINUSEQ\n"); RETURN(tMINUSEQ); }
"*="     { if (g_tokens) printf("tMULTEQ\n"); RETURN(tMULTEQ); }
"/="     { if (g_tokens) printf("tDIVEQ\n"); RETURN(tDIVEQ); }
"%="     { if (g_tokens) printf("tMODEQ\n"); RETURN(tMODEQ); }
"&="     { if (g_tokens) printf("tBITANDEQ\n"); RETURN(tBITANDEQ); }
"|="     { if (g_tokens) printf("tBITOREQ\n"); RETURN(tBITOREQ); }
"^="     { if (g_tokens) printf("tBITXOREQ\n"); RETURN(tBITXOREQ); }
"<<="    { if (g_tokens) printf("tLEFTSHIFTEQ\n"); RETURN(tLEFTSHIFTEQ); }
">>="    { if (g_tokens) printf("tRIGHTSHIFTEQ\n"); RETURN(tRIGHTSHIFTEQ); }
"&^="    { if (g_tokens) printf("tBITCLEAREQ\n"); RETURN(tBITCLEAREQ); }
"&&"     { if (g_tokens) printf("tAND\n"); RETURN(tAND); }
"||"     { if (g_tokens) printf("tOR\n"); RETURN(tOR); }
"<-"     { if (g_tokens) printf("tLEFTARROW\n"); RETURN(tLEFTARROW); }
"++"     { if (g_tokens) printf("tINCREMENT\n"); RETURN(tINCREMENT); }
"--"     { if (g_tokens) printf("tDECREMENT\n"); RETURN(tDECREMENT); }
"=="     { if (g_tokens) printf("tEQ\n"); RETURN(tEQ); }
"<"      { if (g_tokens) printf("tLESS\n"); RETURN(tLESS); }
">"      { if (g_tokens) printf("tGRTR\n"); RETURN(tGRTR); }
"="      { if (g_tokens) printf("tASSIGN\n"); RETURN(tASSIGN); }
"!"      { if (g_tokens) printf("tBANG\n"); RETURN(tBANG); }
"!="     { if (g_tokens) printf("tNOTEQ\n"); RETURN(tNOTEQ); }
"<="     { if (g_tokens) printf("tLESSEQ\n"); RETURN(tLESSEQ); }
">="     { if (g_tokens) printf("tGRTREQ\n"); RETURN(tGRTREQ); }
":="     { if (g_tokens) printf("tCOLONASSIGN\n"); RETURN(tCOLONASSIGN); }
"..."    { if (g_tokens) printf("tELLIPSIS\n"); RETURN(tELLIPSIS); }
"("      { if (g_tokens) printf("tLPAR\n"); RETURN(tLPAR); }
"["      { if (g_tokens) printf("tLBRACKET\n"); RETURN(tLBRACKET); }
"{"      { if (g_tokens) printf("tLBRACE\n"); RETURN(tLBRACE); }
","      { if (g_tokens) printf("tCOMMA\n"); RETURN(tCOMMA); }
"."      { if (g_tokens) printf("tPERIOD\n"); RETURN(tPERIOD); }
")"      { if (g_tokens) printf("tRPAR\n"); RETURN(tRPAR); }
"]"      { if (g_tokens) printf("tRBRACKET\n"); RETURN(tRBRACKET); }
"}"      { if (g_tokens) printf("tRBRACE\n"); RETURN(tRBRACE); }
";"      { if (g_tokens) printf("tSEMICOLON\n"); RETURN(tSEMICOLON); }
":"      { if (g_tokens) printf("tCOLON\n"); RETURN(tCOLON); }

0|([1-9]{DIGIT}*) { // Decimal Integer
    if (g_tokens) printf("tINTVAL(%s)\n", yytext);
    yylval.intval = atoi(yytext);
    RETURN(tINTVAL);
}

0[0-7][0-7]* { // Octal integer
	if (g_tokens) printf("tINTVAL(%ld)\n", strtol(yytext, NULL, 8));
	yylval.intval = strtol(yytext, NULL, 8);
	RETURN(tINTVAL);
}

0(x|X)[0-9a-fA-F][0-9a-fA-F]* { // Hexadecimal integer
	if (g_tokens) printf("tINTVAL(%ld)\n", strtol(yytext, NULL, 16));
	yylval.intval = strtol(yytext, NULL, 16);
	RETURN(tINTVAL);
}

({DIGIT}*)\.{DIGIT}+ { // Float value, ie: .12
	if (g_tokens) printf("tFLOATVAL(%s)\n", yytext);
    yylval.floatval = atof(yytext);
    RETURN(tFLOATVAL);
}

({DIGIT}+)\.{DIGIT}* { // Float value, ie: 12.
	if (g_tokens) printf("tFLOATVAL(%s)\n", yytext);
    yylval.floatval = atof(yytext);
    RETURN(tFLOATVAL);
}

\'([^']|\\{ESCAPE})\' { // Rune literal (char literal)
	if (g_tokens) printf("tRUNEVAL(%s)\n", yytext);
	if (yyleng == 4) {
		switch (yytext[2]) {
			case 'a':
				yylval.runeval = '\a'; RETURN(tRUNEVAL);
			case 't':
				yylval.runeval = '\t'; RETURN(tRUNEVAL);
			case 'b':
				yylval.runeval = '\b'; RETURN(tRUNEVAL);
			case 'v':
				yylval.runeval = '\v'; RETURN(tRUNEVAL);
			case 'f':
				yylval.runeval = '\f'; RETURN(tRUNEVAL);
			case 'n':
				yylval.runeval = '\n'; RETURN(tRUNEVAL);
			case 'r':
				yylval.runeval = '\r'; RETURN(tRUNEVAL);
			case '\\':
				yylval.runeval = '\\'; RETURN(tRUNEVAL);
			case '\'':
				yylval.runeval = '\''; RETURN(tRUNEVAL);
		}
	}
	yylval.runeval = yytext[1];
	RETURN(tRUNEVAL);
}

\"(\\.|[^\\"])*\" { // Interpreted string literal
    if (g_tokens) printf("tSTRVAL(%s)\n", yytext);
    yylval.strval = strdup(yytext);
    RETURN(tSTRVAL);
}

`[^`]*` { // Raw string literal
	if (g_tokens) printf("tSTRVAL(%s)\n", get_raw_string(yytext));
	yylval.strval = get_raw_string(yytext); 
	RETURN(tSTRVAL);
}

[a-zA-Z_][a-zA-Z0-9_]* { // Identifier
	if (g_tokens) printf("tIDENTIFIER(%s)\n", yytext);
	yylval.strval = strdup(yytext);
    RETURN(tIDENTIFIER);
}

.       { fprintf(stderr, "Error: (line %d) unexpected character '%s'\n", yylineno, yytext); exit(1); }

%%

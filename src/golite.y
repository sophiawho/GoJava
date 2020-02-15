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
	int intval;
	float floatval;
	char runeval;
	char *strval;
	// EXP *exp;
}

// %type <exp> program exp

%token <intval> tINTVAL
%token <floatval> tFLOATVAL
%token <runeval> tRUNEVAL
%token <strval> tSTRVAL
%token <strval> tIDENTIFIER

%token tBREAK
%token tCASE
%token tCHAN
%token tCONST
%token tCONTINUE
%token tDEFAULT
%token tDEFER
%token tELSE
%token tFALLTHROUGH
%token tFOR
%token tFUNC
%token tGO
%token tGOTO
%token tIF
%token tIMPORT
%token tINTERFACE
%token tMAP
%token tPACKAGE
%token tRANGE
%token tRETURN
%token tSELECT
%token tSTRUCT
%token tSWITCH
%token tTYPE
%token tVAR
%token tPRINT
%token tPRINTLN
%token tAPPEND
%token tLEN
%token tCAP

%token tLEFTSHIFT
%token tRIGHTSHIFT
%token tAND
%token tOR
%token tBITOREQ
%token tLESSEQ
%token tCOLONASSIGN
%token tBITCLEAREQ
%token tMULTEQ
%token tBITANDEQ
%token tLBRACKET
%token tRBRACKET
%token tBITXOR
%token tBITXOREQ
%token tINCREMENT
%token tDECREMENT
%token tEQ
%token tBANG
%token tMOD
%token tBITAND
%token tLPAR
%token tRPAR
%token tMULT
%token tADD
%token tCOMMA
%token tMINUS
%token tDIVEQ
%token tELLIPSIS
%token tPERIOD
%token tDIV
%token tMINUSEQ
%token tLEFTARROW
%token tPLUSEQ
%token tBITCLEAR
%token tMODEQ
%token tCOLON
%token tLBRACE
%token tSEMICOLON
%token tBITOR
%token tLESS
%token tNOTEQ
%token tLEFTSHIFTEQ
%token tRIGHTSHIFTEQ
%token tASSIGN
%token tRBRACE
%token tGRTR
%token tGRTREQ

%left '+' '-'
%left '*' '/'
// TODO write precedence for all operators

%start program

// TODO able to parse blank programs
// TODO are we doing methods?

%% 
program: packageClause topLevelDecls;

packageClause: tPACKAGE tIDENTIFIER;

topLevelDecls: ;
	| topLevelDecl topLevelDecls;

topLevelDecl: decl;
	| functionDecl ;

decl: constDecl;
	| typeDecl;
	| variableDecl;

constDecl: ;

typeDecl: ;

variableDecls: ;
	| variableDecl variableDecls;

variableDecl: tVAR tIDENTIFIER tIDENTIFIER;
	| tVAR identifiers tASSIGN expression;
	| tVAR identifiers tIDENTIFIER tASSIGN expression;

identifiers: ;
	| tIDENTIFIER tCOMMA identifiers;
	| tIDENTIFIER;

functionDecl: tFUNC tIDENTIFIER tLPAR variableDecls tRPAR;
	| tFUNC tIDENTIFIER tLPAR variableDecls tRPAR tIDENTIFIER;

expressions: ;
	| expression tCOMMA expressions;

expression: 
	| expression binaryOperator expression;
	| functionCall; 
	| tADD expression;
	| tMINUS expression;
	| tBANG expression;
	| tBITXOR expression;
	| tLBRACKET tINTVAL tRBRACKET tIDENTIFIER;
	| tIDENTIFIER tLBRACKET tINTVAL tRBRACKET;
	| tINTVAL;
	| tFLOATVAL;
	| tRUNEVAL;
	| tSTRVAL;
	| tIDENTIFIER;

functionCall: tIDENTIFIER tLPAR functionArgs tRPAR;

functionArgs: ;
	| tIDENTIFIER tCOMMA functionArgs;

binaryOperator: tOR;
	| tAND;
	| tEQ;
	| tNOTEQ;
	| tLESS;
	| tLESSEQ;
	| tGRTR;
	| tGRTREQ;
	| tADD;
	| tMINUS;
	| tBITOR;
	| tBITXOR;
	| tMULT;
	| tDIV;
	| tMOD;
	| tLEFTSHIFT;
	| tRIGHTSHIFT;
	| tBITAND;
	| tBITCLEAR;

%%

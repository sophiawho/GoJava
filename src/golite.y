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

%left tOR tAND tEQ tNOTEQ tLESS tLESSEQ tGRTR tGRTREQ tBITOR tMULT tDIV tMOD tLEFTSHIFT tRIGHTSHIFT tBITAND tBITCLEAR
%left tADD tMINUS tBANG tBITXOR
// TODO write precedence for all operators

%start program

// TODO able to parse blank programs
// TODO are we doing methods?

%% 
program: packageClause topLevelDecls;

packageClause: tPACKAGE tIDENTIFIER tSEMICOLON;

topLevelDecls:
	| topLevelDecls topLevelDecl
	;

topLevelDecl: variableDecl 
	| typeDecl
	| functionDecl
	;

variableDecl: tVAR variableSpec
	| tVAR tLPAR variableSpecs tRPAR
	;

variableSpecs:
	| variableSpecs variableSpec
	;

variableSpec: identifiers type tSEMICOLON
	| identifiers tASSIGN expressions tSEMICOLON
	| identifiers type tASSIGN expressions tSEMICOLON
	;

typeDecl: tTYPE typeSpec
	| tTYPE tLPAR typeSpecs tRPAR
	;

typeSpecs:
	| typeSpecs typeSpec
	;

typeSpec: tIDENTIFIER type tSEMICOLON
	| tIDENTIFIER tSTRUCT tLBRACE structSpecs tRBRACE tSEMICOLON
	;

structSpecs:
	| structSpecs structSpec
	;

structSpec: identifiers tIDENTIFIER tSEMICOLON;

identifiers: tIDENTIFIER
	| identifiers tCOMMA tIDENTIFIER
	;

type: sliceType
	| arrayType
	| tIDENTIFIER
	;

sliceType: tLBRACKET tRBRACKET type;

arrayType: tLBRACKET tINTVAL tRBRACKET type;

expressions: expression
	| expressions tCOMMA expression
	;

expression: expression tOR expression
	| expression tAND expression
	| expression tEQ expression
	| expression tNOTEQ expression
	| expression tLESS expression
	| expression tLESSEQ expression
	| expression tGRTR expression
	| expression tGRTREQ expression
	| expression tADD expression
	| expression tMINUS expression
	| expression tBITOR expression
	| expression tBITXOR expression
	| expression tMULT expression
	| expression tDIV expression
	| expression tMOD expression
	| expression tLEFTSHIFT expression
	| expression tRIGHTSHIFT expression
	| expression tBITAND expression
	| expression tBITCLEAR expression
	| tADD expression
	| tMINUS expression
	| tBANG expression
	| tBITXOR expression
	| tIDENTIFIER tLBRACKET tINTVAL tRBRACKET
	| tINTVAL
	| tFLOATVAL
	| tRUNEVAL
	| tSTRVAL
	| tIDENTIFIER
	;

functionDecl: tFUNC tIDENTIFIER tLPAR inputParams tRPAR block // no return type
	| tFUNC tIDENTIFIER tLPAR inputParams tRPAR identifiers block
	;

inputParams: 
	| inputParams tCOMMA identifiers tIDENTIFIER 
	| identifiers tIDENTIFIER
	;

block: tLBRACE stmts tRBRACE tSEMICOLON;

stmts: 
	| stmts stmt
	;

stmt: tRETURN tSEMICOLON
	| tRETURN tIDENTIFIER tSEMICOLON
	| variableDecl
	| assignmentStmt tSEMICOLON
	;

assignmentStmt: tIDENTIFIER tASSIGN expression
	| tIDENTIFIER tLBRACKET tINTVAL tRBRACKET tASSIGN expression
	| tIDENTIFIER tPERIOD tIDENTIFIER tASSIGN expression
	;

%%

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
%left tLPAR tRPAR
// TODO write precedence for all operators

%start program

// TODO able to parse blank programs
// TODO are we doing methods?

%% 
program: 
	| packageClause topLevelDecls;

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

structSpec: identifiers type tSEMICOLON;

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

expression: binaryExpr
	| unaryExpr
	| builtinExpr
	| functionCallExpr
	| tIDENTIFIER tLBRACKET tINTVAL tRBRACKET
	| tIDENTIFIER tPERIOD tIDENTIFIER
	| tLPAR expression tRPAR
	| tINTVAL
	| tFLOATVAL
	| tRUNEVAL
	| tSTRVAL
	| tIDENTIFIER
	;

binaryExpr: expression tOR expression
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
	;
 
unaryExpr: tADD expression
	| tMINUS expression
	| tBANG expression
	| tBITXOR expression
	;

builtinExpr: tAPPEND tLPAR expression tCOMMA expression tRPAR
	| tLEN tLPAR expression tRPAR
	| tCAP tLPAR expression tRPAR
	;

functionCallExpr: type tLPAR expression tRPAR
	| type tLPAR tRPAR
	;

functionDecl:  tFUNC tIDENTIFIER tLPAR inputParams tRPAR optType block tSEMICOLON
	| tFUNC tIDENTIFIER tLPAR inputParams tRPAR optType tLBRACE statements returnStmt tRBRACE tSEMICOLON
	;

inputParams: 
	| inputParams tCOMMA identifiers type 
	| identifiers type
	;

optType: 
	| type
	;

block: tLBRACE statements tRBRACE;

statements: 
	| statements statement
	;

statement: simpleStmt tSEMICOLON
	| returnStmt tSEMICOLON
	| block tSEMICOLON
	| variableDecl
	| typeDecl
	| tPRINT tLPAR expressions tRPAR tSEMICOLON
	| tPRINTLN tLPAR expressions tRPAR tSEMICOLON
	| ifStmt tSEMICOLON
	| switchStmt tSEMICOLON
	| forStmt tSEMICOLON
	| tBREAK tSEMICOLON
	| tCONTINUE tSEMICOLON
	;

simpleStmt: /* empty statement */
	| expression
	| expressions tLEFTARROW expression
	| expression tINCREMENT
	| expression tDECREMENT
	| expressions tASSIGN expressions
	| expression assignOp expression
	| expressions tCOLONASSIGN expressions
	;

returnStmt: tRETURN
	| tRETURN expression
	;

assignOp: tPLUSEQ
	| tMINUSEQ
	| tMULTEQ
	| tDIVEQ
 	| tMODEQ
	| tBITANDEQ
	| tBITOREQ
	| tBITXOREQ
	| tLEFTSHIFTEQ
	| tRIGHTSHIFTEQ
	| tBITCLEAREQ
	;

ifStmt: tIF simpleStmt tSEMICOLON expression block
	| tIF simpleStmt tSEMICOLON expression block tELSE block
	| tIF simpleStmt tSEMICOLON expression block tELSE ifStmt
	| tIF expression block
	| tIF expression block tELSE block
	| tIF expression block tELSE ifStmt
	;

switchStmt: tSWITCH simpleStmt tSEMICOLON expression tLBRACE exprCaseClauses tRBRACE
	| tSWITCH expression tLBRACE exprCaseClauses tRBRACE
	;

exprCaseClauses: 
	| exprCaseClauses exprCaseClause
	;

exprCaseClause: tCASE expressions tCOLON statements
	| tDEFAULT tCOLON statements
	;

forStmt: tFOR block 
	| tFOR expression block
	| tFOR simpleStmt tSEMICOLON expression tSEMICOLON simpleStmt block
	;

%%

%{
#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

extern int yylineno;

extern PROG *root;

int g_tokens = 1;

int yylex();
void yyerror(const char *s) { fprintf(stderr, "Error: (line %d) %s\n", yylineno, s); exit(1); }
%}

%locations
%error-verbose

%code requires
{
}

%union {
	int intval;
	float floatval;
	char runeval;
	char *strval;
	struct PROG *prog;
	struct TOPLEVELDECL *topleveldecl;
	struct TYPE *type;
	struct STMT *stmt;
	struct EXP *exp;
	struct FUNC *func;
	struct VARSPEC *varspec;
	struct TYPESPEC *typespec;
	struct STRUCTSPEC *structspec;
	struct IDENT *ident;
}

%type <prog> program
%type <topleveldecl> topLevelDecls topLevelDecl variableDecl typeDecl
%type <type> type sliceType arrayType
%type <stmt> statements statement simpleStmt returnStmt ifStmt switchStmt forStmt
%type <exp> expressions expression binaryExpr unaryExpr builtinExpr functionCallExpr
%type <func> functionDecl
%type <varspec> variableSpecs variableSpec
%type <typespec> typeSpecs typeSpec
%type <structspec> structSpecs structSpec
%type <ident> identifiers identifier

%token <intval> tINTVAL
%token <floatval> tFLOATVAL
%token <runeval> tRUNEVAL
%token <strval> tSTRVAL tIDENTIFIER

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

/*
Binary operators have 5 precendence levels.
Precedence    Operator
    5             *  /  %  <<  >>  &  &^
    4             +  -  |  ^
    3             ==  !=  <  <=  >  >=
    2             &&
    1             ||
*/
%left tOR
%left tAND
%left tEQ tNOTEQ tLESS tLESSEQ tGRTR tGRTREQ
%left tADD tMINUS tBITOR tBITXOR
%left tMULT tDIV tMOD tLEFTSHIFT tRIGHTSHIFT tBITAND tBITCLEAR

/* 
Unary operators have the highest precedence. 
		! 	  + 	- 		ˆ 
*/
%left tBANG UPLUS UMINUS UBITXOR tPERIOD tLBRACKET

%start program

%% 
program: /* empty */ { $$ = NULL; }
	| tPACKAGE tIDENTIFIER tSEMICOLON topLevelDecls { root = makePROG($2, $4); }
	;

topLevelDecls: /* empty */ { $$ = NULL; }
	| topLevelDecls topLevelDecl { $$ = $2; $$->next=$1; }
	;

topLevelDecl: variableDecl { $$ = $1; }
	| typeDecl { $$ = $1; }
	| functionDecl 
	;

variableDecl: tVAR variableSpec { $$ = makeTopLevelDecl_var($2); }
	| tVAR tLPAR variableSpecs tRPAR tSEMICOLON { $$ = makeTopLevelDecl_var($3); }
	;

variableSpecs: /* empty */ { $$ = NULL; }
	| variableSpecs variableSpec { $$ = $2; $$->next=$1; } 
	;

variableSpec: identifiers type tSEMICOLON { $$ = makeVarSpec($1, NULL, $2); }
	| identifiers tASSIGN expressions tSEMICOLON { $$ = makeVarSpec($1, $3, NULL); }
	| identifiers type tASSIGN expressions tSEMICOLON { $$ = makeVarSpec($1, $4, $2); }
	;

typeDecl: tTYPE typeSpec { $$ = makeTopLevelDecl_type($2); }
	| tTYPE tLPAR typeSpecs tRPAR tSEMICOLON { $$ = makeTopLevelDecl_type($3); }
	;

typeSpecs: /* empty */ { $$ = NULL; }
	| typeSpecs typeSpec { $$ = $2; $$->next=$1; }
	;

typeSpec: identifier type tSEMICOLON { $$ = makeTypeSpec($1, $2); }
	| identifier tSTRUCT tLBRACE structSpecs tRBRACE tSEMICOLON { $$ = makeTypeSpec_struct($1, $4); }
	;

structSpecs: /* empty */ { $$ = NULL; }
	| structSpecs structSpec { $$ = $2; $$->next = $1; }
	;

structSpec: identifiers type tSEMICOLON { $$ = makeStructSpec($1, $2); }
	;

identifiers: identifier { $$ = $1; }
	| identifiers tCOMMA identifier { $$ = $3; $$->next=$1; }
	;

identifier: tIDENTIFIER { $$ = makeIDENT($1); }

type: sliceType { $$ = $1; }
	| arrayType { $$ = $1; }
	| tIDENTIFIER { $$ = makeTYPE_ident($1); } 
	;

sliceType: tLBRACKET tRBRACKET type { $$ = makeTYPE_slice($3); }
	;

arrayType: tLBRACKET expression tRBRACKET type { $$ = makeTYPE_array($2, $4); }
	;

expressions: expression { $$ = $1; }
	| expressions tCOMMA expression { $$ = $3; $$->next=$1; }
	;

expression: binaryExpr { $$ = $1; }
	| unaryExpr { $$ = $1; }
	| builtinExpr { $$ = $1; }
	| functionCallExpr
	| expression tLBRACKET expression tRBRACKET { /*array access */ }
	| expression tPERIOD tIDENTIFIER { /*field access*/ }
	| tINTVAL { $$ = makeEXP_intLiteral($1); }
	| tFLOATVAL { $$ = makeEXP_floatLiteral($1); }
	| tRUNEVAL { $$ = makeEXP_runeLiteral($1); }
	| tSTRVAL { $$ = makeEXP_stringLiteral($1); }
	| tIDENTIFIER { $$ = makeEXP_identifier($1); }
	;

binaryExpr: expression tOR expression { $$ = makeEXP_binary(k_expKindOr, $1, $3); }
	| expression tAND expression { $$ = makeEXP_binary(k_expKindAnd, $1, $3); }
	| expression tEQ expression { $$ = makeEXP_binary(k_expKindEq, $1, $3); }
	| expression tNOTEQ expression { $$ = makeEXP_binary(k_expKindNotEq, $1, $3); }
	| expression tLESS expression { $$ = makeEXP_binary(k_expKindLess, $1, $3); }
	| expression tLESSEQ expression { $$ = makeEXP_binary(k_expKindLessEq, $1, $3); }
	| expression tGRTR expression { $$ = makeEXP_binary(k_expKindGrtr, $1, $3); }
	| expression tGRTREQ expression { $$ = makeEXP_binary(k_expKindGrtrEq, $1, $3); }
	| expression tADD expression { $$ = makeEXP_binary(k_expKindAdd, $1, $3); }
	| expression tMINUS expression { $$ = makeEXP_binary(k_expKindMinus, $1, $3); }
	| expression tBITOR expression { $$ = makeEXP_binary(k_expKindBitOr, $1, $3); }
	| expression tBITXOR expression { $$ = makeEXP_binary(k_expKindBitXOR, $1, $3); }
	| expression tMULT expression { $$ = makeEXP_binary(k_expKindMult, $1, $3); }
	| expression tDIV expression { $$ = makeEXP_binary(k_expKindDiv, $1, $3); }
	| expression tMOD expression { $$ = makeEXP_binary(k_expKindMod, $1, $3); }
	| expression tLEFTSHIFT expression { $$ = makeEXP_binary(k_expKindLeftShift, $1, $3); }
	| expression tRIGHTSHIFT expression { $$ = makeEXP_binary(k_expKindRightShift, $1, $3); }
	| expression tBITAND expression { $$ = makeEXP_binary(k_expKindBitAnd, $1, $3); }
	| expression tBITCLEAR expression { $$ = makeEXP_binary(k_expKindBitClear, $1, $3); }
	;
 
unaryExpr: tADD expression %prec UPLUS { $$ = makeEXP_unary(k_expKindUPlus, $2); }
	| tMINUS expression %prec UMINUS { $$ = makeEXP_unary(k_expKindUMinus, $2); }
	| tBANG expression { $$ = makeEXP_unary(k_expKindBang, $2); }
	| tBITXOR expression %prec UBITXOR { $$ = makeEXP_unary(k_expKindUBitXOR, $2); }
	| tLPAR expression tRPAR { $$ = makeEXP_unary(k_expKindParentheses, $2); }
	;

builtinExpr: tAPPEND tLPAR expression tCOMMA expression tRPAR { $$ = makeEXP_append($3, $5); }
	| tLEN tLPAR expression tRPAR { $$ = makeEXP_len($3); }
	| tCAP tLPAR expression tRPAR { $$ = makeEXP_cap($3); }
	;

functionCallExpr: type tLPAR expressions tRPAR
	| type tLPAR tRPAR
	;

functionDecl:  tFUNC tIDENTIFIER tLPAR inputParams tRPAR optType block tSEMICOLON
	| tFUNC tIDENTIFIER tLPAR inputParams tRPAR optType tLBRACE statements returnStmt tRBRACE tSEMICOLON
	;

inputParams: /* empty */
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

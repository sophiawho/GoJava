%{
#include <stdio.h>
#include <stdlib.h>

#include "ast/tree.h"
#include "ast/stmt.h"

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
	// Main AST nodes
	struct PROG *prog;
	struct TOPLEVELDECL *topleveldecl;
	struct TYPE *type;
	struct STMT *stmt;
	struct EXP *exp;
	struct FUNC *func;
	// Helper list structs
	struct VARSPEC *varspec;
	struct TYPESPEC *typespec;
	struct STRUCTSPEC *structspec;
	struct IDENT *ident;
	struct EXPRCASECLAUSE *exprcaseclause;
}

%type <prog> program
%type <topleveldecl> topLevelDecls topLevelDecl 
%type <type> type sliceType arrayType optType
%type <stmt> statements statement simpleStmt block returnStmt ifStmt switchStmt forStmt
%type <exp> expressions expression binaryExpr unaryExpr builtinExpr functionCallExpr
%type <func> functionDecl
%type <varspec> variableSpecs variableSpec variableDecl
%type <typespec> typeSpecs typeSpec inputParams inputParam typeDecl
%type <structspec> structSpecs structSpec
%type <ident> identifiers identifier
%type <exprcaseclause> exprCaseClauses exprCaseClause

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
program: /* empty */ 								{ $$ = NULL; }
	| tPACKAGE tIDENTIFIER tSEMICOLON topLevelDecls { root = makePROG($2, $4); }
	;

topLevelDecls: /* empty */ 				{ $$ = NULL; }
	| topLevelDecls topLevelDecl 		{ $$ = $2; $$->next=$1; }
	;

topLevelDecl: variableDecl 	{ $$ = makeTopLevelDecl_var($1); }
	| typeDecl 				{ $$ = makeTopLevelDecl_type($1); }
	| functionDecl 			{ $$ = makeTopLevelDecl_func($1); }
	;

variableDecl: tVAR variableSpec 				{ $$ = $2; }
	| tVAR tLPAR variableSpecs tRPAR tSEMICOLON { $$ = $3; }
	;

variableSpecs: /* empty */ 			{ $$ = NULL; }
	| variableSpecs variableSpec 	{ $$ = $2; $$->next=$1; } 
	;

variableSpec: identifiers type tSEMICOLON 			{ $$ = makeVarSpec($1, NULL, $2); }
	| identifiers tASSIGN expressions tSEMICOLON 	{ $$ = makeVarSpec($1, $3, NULL); }
	| identifiers type tASSIGN expressions tSEMICOLON { $$ = makeVarSpec($1, $4, $2); }
	;

typeDecl: tTYPE typeSpec 						{ $$ = $2; }
	| tTYPE tLPAR typeSpecs tRPAR tSEMICOLON 	{ $$ = $3; }
	;

typeSpecs: /* empty */ 		{ $$ = NULL; }
	| typeSpecs typeSpec 	{ $$ = $2; $$->next=$1; }
	;

typeSpec: identifier type tSEMICOLON							{ $$ = makeTypeSpec($1, $2); }
	| identifier tSTRUCT tLBRACE structSpecs tRBRACE tSEMICOLON { $$ = makeTypeSpec_struct($1, $4); }
	;

structSpecs: /* empty */ 		{ $$ = NULL; }
	| structSpecs structSpec 	{ $$ = $2; $$->next = $1; }
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

sliceType: tLBRACKET tRBRACKET type 			{ $$ = makeTYPE_slice($3); }
	;

arrayType: tLBRACKET tINTVAL tRBRACKET type 	{ $$ = makeTYPE_array($2, $4); }
	;

expressions: expression 			{ $$ = $1; }
	| expressions tCOMMA expression { $$ = $3; $$->next=$1; }
	;

expression: binaryExpr 							{ $$ = $1; }
	| unaryExpr 								{ $$ = $1; }
	| builtinExpr 								{ $$ = $1; }
	| functionCallExpr							{ $$ = $1; }
	| expression tLBRACKET expression tRBRACKET { $$ = makeEXP_arrayAccess($1, $3); }
	| expression tPERIOD tIDENTIFIER 			{ $$ = makeEXP_fieldAccess($1, $3);  }
	| tINTVAL 									{ $$ = makeEXP_intLiteral($1); }
	| tFLOATVAL 								{ $$ = makeEXP_floatLiteral($1); }
	| tRUNEVAL 									{ $$ = makeEXP_runeLiteral($1); }
	| tSTRVAL 									{ $$ = makeEXP_stringLiteral($1); }
	| tIDENTIFIER 								{ $$ = makeEXP_identifier($1); }
	;

binaryExpr: expression tOR expression 	{ $$ = makeEXP_binary(k_expKindOr, $1, $3); }
	| expression tAND expression 		{ $$ = makeEXP_binary(k_expKindAnd, $1, $3); }
	| expression tEQ expression 		{ $$ = makeEXP_binary(k_expKindEq, $1, $3); }
	| expression tNOTEQ expression 		{ $$ = makeEXP_binary(k_expKindNotEq, $1, $3); }
	| expression tLESS expression 		{ $$ = makeEXP_binary(k_expKindLess, $1, $3); }
	| expression tLESSEQ expression 	{ $$ = makeEXP_binary(k_expKindLessEq, $1, $3); }
	| expression tGRTR expression 		{ $$ = makeEXP_binary(k_expKindGrtr, $1, $3); }
	| expression tGRTREQ expression 	{ $$ = makeEXP_binary(k_expKindGrtrEq, $1, $3); }
	| expression tADD expression 		{ $$ = makeEXP_binary(k_expKindAdd, $1, $3); }
	| expression tMINUS expression 		{ $$ = makeEXP_binary(k_expKindMinus, $1, $3); }
	| expression tBITOR expression 		{ $$ = makeEXP_binary(k_expKindBitOr, $1, $3); }
	| expression tBITXOR expression 	{ $$ = makeEXP_binary(k_expKindBitXOR, $1, $3); }
	| expression tMULT expression 		{ $$ = makeEXP_binary(k_expKindMult, $1, $3); }
	| expression tDIV expression 		{ $$ = makeEXP_binary(k_expKindDiv, $1, $3); }
	| expression tMOD expression 		{ $$ = makeEXP_binary(k_expKindMod, $1, $3); }
	| expression tLEFTSHIFT expression 	{ $$ = makeEXP_binary(k_expKindLeftShift, $1, $3); }
	| expression tRIGHTSHIFT expression { $$ = makeEXP_binary(k_expKindRightShift, $1, $3); }
	| expression tBITAND expression 	{ $$ = makeEXP_binary(k_expKindBitAnd, $1, $3); }
	| expression tBITCLEAR expression 	{ $$ = makeEXP_binary(k_expKindBitClear, $1, $3); }
	;
 
unaryExpr: tADD expression %prec UPLUS 	{ $$ = makeEXP_unary(k_expKindUPlus, $2); }
	| tMINUS expression %prec UMINUS 	{ $$ = makeEXP_unary(k_expKindUMinus, $2); }
	| tBANG expression 					{ $$ = makeEXP_unary(k_expKindBang, $2); }
	| tBITXOR expression %prec UBITXOR 	{ $$ = makeEXP_unary(k_expKindUBitXOR, $2); }
	| tLPAR expression tRPAR 			{ $$ = $2; }
	;

builtinExpr: tAPPEND tLPAR expression tCOMMA expression tRPAR 	{ $$ = makeEXP_append($3, $5); }
	| tLEN tLPAR expression tRPAR 								{ $$ = makeEXP_len($3); }
	| tCAP tLPAR expression tRPAR 								{ $$ = makeEXP_cap($3); }
	;

functionCallExpr: type tLPAR expressions tRPAR		{ $$ = makeEXP_funcCall($1, $3); }
	| type tLPAR tRPAR								{ $$ = makeEXP_funcCall($1, NULL); }
	;

functionDecl:  
	tFUNC tIDENTIFIER tLPAR inputParams tRPAR optType block tSEMICOLON { 
		$$ = makeFunc($2, $6, $4, $7); 
	}
	;

inputParams: /* empty */ 			{ $$ = NULL; }
	| inputParams tCOMMA inputParam { $$ = $3; $$->next = $1; }
	| inputParam 					{ $$ = $1; }
	;

inputParam: identifiers type 		{ $$ = makeTypeSpec($1, $2); }
	;

optType: /* empty */ 				{ $$ = NULL; }
	| type 							{ $$ = $1; }
	;

block: tLBRACE statements tRBRACE 	{ $$ = makeSTMT_blockStmt($2); }
	;

statements: /* empty */ 			{ $$ = NULL; }
	| statements statement 			{ $$ = $2; $$->next = $1; }
	;

statement: simpleStmt tSEMICOLON 	{ $$ = $1; }
	| returnStmt tSEMICOLON 		{ $$ = $1; }
	| block tSEMICOLON 				{ $$ = $1; }
	| variableDecl 					{ $$ = makeSTMT_varDecl($1); }
	| typeDecl 						{ $$ = makeSTMT_typeDecl($1); }
	| tPRINT tLPAR expressions tRPAR tSEMICOLON 	{ $$ = makeSTMT_print($3, 0); }
	| tPRINTLN tLPAR expressions tRPAR tSEMICOLON 	{ $$ = makeSTMT_print($3, 1); }
	| tPRINT tLPAR tRPAR tSEMICOLON 	{ $$ = makeSTMT_print(NULL, 0); }
	| tPRINTLN tLPAR tRPAR tSEMICOLON 	{ $$ = makeSTMT_print(NULL, 1); }
	| ifStmt tSEMICOLON				{ $$ = $1; }
	| switchStmt tSEMICOLON			{ $$ = $1; }
	| forStmt tSEMICOLON			{ $$ = $1; }
	| tBREAK tSEMICOLON 			{ $$ = getGenericStmt(k_stmtKindBreak); }
	| tCONTINUE tSEMICOLON 			{ $$ = getGenericStmt(k_stmtKindContinue); }
	;

simpleStmt: /* empty */ 					{ $$ = NULL; }
	| expression 							{ $$ = makeSTMT_expStmt($1); }
	| expression tINCREMENT 				{ $$ = makeSTMT_incDec($1, 1); }
	| expression tDECREMENT 				{ $$ = makeSTMT_incDec($1, -1); }
	| expressions tASSIGN expressions 		{ $$ = makeSTMT_assignStmt($1, k_stmtAssign, $3); }
	| expressions tCOLONASSIGN expressions 	{ $$ = makeSTMT_assignStmt($1, k_stmtColonAssign, $3); }
	| expression tPLUSEQ expression 		{ $$ = makeSTMT_assignStmt($1, k_opAssignKindPlusEq, $3); }
	| expression tMINUSEQ expression 		{ $$ = makeSTMT_assignStmt($1, k_opAssignKindMinusEq, $3); }
	| expression tMULTEQ expression 		{ $$ = makeSTMT_assignStmt($1, k_opAssignKindMultEq, $3); }
	| expression tDIVEQ expression 			{ $$ = makeSTMT_assignStmt($1, k_opAssignKindDivEq, $3); }
	| expression tMODEQ expression 			{ $$ = makeSTMT_assignStmt($1, k_opAssignKindModEq, $3); }
	| expression tBITANDEQ expression 		{ $$ = makeSTMT_assignStmt($1, k_opAssignKindBitAndEq, $3); }
	| expression tBITOREQ expression 		{ $$ = makeSTMT_assignStmt($1, k_opAssignKindBitOrEq, $3); }
	| expression tBITXOREQ expression 		{ $$ = makeSTMT_assignStmt($1, k_opAssignKindBitXorEq, $3); }
	| expression tLEFTSHIFTEQ expression 	{ $$ = makeSTMT_assignStmt($1, k_opAssignKindLeftShiftEq, $3); }
	| expression tRIGHTSHIFTEQ expression 	{ $$ = makeSTMT_assignStmt($1, k_opAssignKindRightShiftEq, $3); }
	| expression tBITCLEAREQ expression 	{ $$ = makeSTMT_assignStmt($1, k_opAssignKindBitClearEq, $3); }
	;

returnStmt: tRETURN							{ $$ = makeSTMT_returnStmt(NULL); }
	| tRETURN expression					{ $$ = makeSTMT_returnStmt($2); }
	;

ifStmt: tIF simpleStmt tSEMICOLON expression block 				{ $$ = makeSTMT_if($2, $4, $5, NULL); }
	| tIF simpleStmt tSEMICOLON expression block tELSE block	{ $$ = makeSTMT_if($2, $4, $5, $7); }
	| tIF simpleStmt tSEMICOLON expression block tELSE ifStmt	{ $$ = makeSTMT_if($2, $4, $5, $7); }
	| tIF expression block										{ $$ = makeSTMT_if(NULL, $2, $3, NULL); }
	| tIF expression block tELSE block							{ $$ = makeSTMT_if(NULL, $2, $3, $5); }
	| tIF expression block tELSE ifStmt							{ $$ = makeSTMT_if(NULL, $2, $3, $5); }
	;

switchStmt: tSWITCH simpleStmt tSEMICOLON expression tLBRACE exprCaseClauses tRBRACE	{ $$ = makeSTMT_switch($2, $4, $6); }
	| tSWITCH expression tLBRACE exprCaseClauses tRBRACE								{ $$ = makeSTMT_switch(NULL, $2, $4); }
	;

exprCaseClauses: /* empty */ 			{ $$ = NULL; }
	| exprCaseClauses exprCaseClause	{ $$ = $2; $$->next = $1; }
	;

exprCaseClause: tCASE expressions tCOLON statements				{ $$ = makeExprCaseClause(k_caseClause, $2, $4); }
	| tDEFAULT tCOLON statements								{ $$ = makeExprCaseClause(k_defaultClause, NULL, $3); }
	;

forStmt: tFOR block 													{ $$ = makeSTMT_for(k_loopKindInfinite, $2, NULL, NULL, NULL); }
	| tFOR expression block												{ $$ = makeSTMT_for(k_loopKindWhile, $3, $2, NULL, NULL); }
	| tFOR simpleStmt tSEMICOLON expression tSEMICOLON simpleStmt block	{ $$ = makeSTMT_for(k_loopKindThreePart, $7, $4, $2, $6); }
	;

%%

#ifndef CODEGEN_H
#define CODEGEN_H

#include "../ast/tree.h"
#include "../ast/stmt.h"
#include <stdio.h>
#include "codestruct.h"
// #include "../typecheck/symbol.h"
// #include "../pretty.h"

FILE *outputFile;
int indent;
int temp_counter;

void generateINDENT(int indent);
void generatePROG(PROG *root, char *filename);
void generateTOPLEVELDECL(TOPLEVELDECL *tld);
void generateFUNC(FUNC *f);
void generateHeader(char *classname, TOPLEVELDECL *tld);
void generateImports();
void generateGlobalVariables();
void generateFooter();
char *prepend(char *str);
void openOutputFile(char *filename);

#endif
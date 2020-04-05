#ifndef CODEGEN_H
#define CODEGEN_H

#include "../ast/tree.h"
#include "../ast/stmt.h"
#include <stdio.h>
// #include "../typecheck/symbol.h"
// #include "../pretty.h"

FILE *outputFile;
int indent;
#define FLOAT_FORMAT_METHOD_NAME "FloatFormat.format"

void generateINDENT(int indent);
void generatePROG(PROG *root, char *filename);
void generateTOPLEVELDECL(TOPLEVELDECL *tld);
void generateFUNC(FUNC *f);
void generateHeader(char *classname);
void generateImports();
void generateGlobalVariables();
void generateFooter();
char *prepend(char *str);
void openOutputFile(char *filename);

#endif
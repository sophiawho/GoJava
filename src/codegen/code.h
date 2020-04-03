#ifndef CODEGEN_H
#define CODEGEN_H

#include "../ast/tree.h"
#include "../ast/stmt.h"
#include <stdio.h>
// #include "../typecheck/symbol.h"
// #include "../pretty.h"

FILE *outputFile;

void generatePROG(PROG *root, char *filename);
void generateTOPLEVELDECL(TOPLEVELDECL *tld);
void generateFUNC(FUNC *f);
void generateHeader(char *classname);
void generateFooter();
char *prepend(char *str);
void openOutputFile(char *filename);

#endif
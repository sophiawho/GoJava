#ifndef CODEGEN_H
#define CODEGEN_H

#include "../ast/tree.h"
#include "../ast/stmt.h"
// #include "../typecheck/symbol.h"
// #include "../pretty.h"

void generatePROG(PROG *root, char *filename);
void generateHeader(char *classname);
void generateFooter();
char *prepend(char *str);
void openOutputFile(char *filename);

#endif
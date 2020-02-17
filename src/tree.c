#include <stdbool.h> 
#include <stdlib.h>
#include "tree.h"

extern int yylineno;

PROG *makePROG(char *package, TOPLEVELDECL *rootTopLevelDecl) {
    PROG *p = malloc(sizeof(PROG));
    p->package = package;
    p->rootTopLevelDecl = rootTopLevelDecl;
    return p;
}

#include <stdbool.h> 
#include <stdlib.h>
#include "tree.h"

<<<<<<< HEAD
extern int yylineno;

PROG *makePROG(char *package, TOPLEVELDECL *rootTopLevelDecl) {
    PROG *p = malloc(sizeof(PROG));
    p->package = package;
    p->rootTopLevelDecl = rootTopLevelDecl;
    return p;
}
=======
extern int yylineno;
>>>>>>> 567336fbbb78c3d1cba2cb3658a49675dcdf97b3

#ifndef CODESTRUCT_H
#define CODESTRUCT_H

#include "../ast/tree.h"
#include "../ast/stmt.h"
#include <stdbool.h>

typedef struct StructElement StructElement;

// A helper node
// Store a list of all structs already seen in program
struct StructElement 
{
    TYPE *type;
    struct StructElement *next;
};
StructElement *makeStructElement(TYPE *t);

void generateGlobalStructs(TOPLEVELDECL *tld);

void generateStructFunc(FUNC *f);
void generateStructVarDecl(VARSPEC *vs);
void generateStructTypeSpec(TYPESPEC *ts);
void generateStructStmt(STMT *s);

void addToStructList(TYPE *t);
int checkExists(TYPE *t);

#endif
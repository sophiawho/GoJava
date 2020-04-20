/**

TODO

Traverse entire program
Print out structs from type definitions, var definitions, untagged functions as: `__golite__struct__$structNumber`
- Helper functions
- Eg: equality

How to tell if two untagged structs are equal? ie: Struct definition equality

Struct scoping?

*/

#include "codestruct.h"
#include "../ast/tree.h"
#include "../ast/stmt.h"
#include "../typecheck/typecheck.h"

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Uniquely identify structs
int codegenTag = 0;

TYPE *t = NULL;

StructElement *makeStructElement(TYPE *t) {
    StructElement *se = malloc(sizeof(StructElement));
    se->type = t;
    se->next = NULL;
    return se;
}

StructElement *head = NULL;

void generateGlobalStructs(TOPLEVELDECL *tld) {
    if (tld == NULL) return;
    generateGlobalStructs(tld->next);
    switch (tld->kind) {
        case k_topLevelDeclFunc:
            generateStructFunc(tld->val.funcDecl);
            break;
        case k_topLevelDeclType:
            generateStructTypeSpec(tld->val.typeDecl);
            break;
        case k_topLevelDeclVar:
            generateStructVarDecl(tld->val.varDecl);
            break;
    }
}

void generateStructFunc(FUNC *f) {
    generateStructStmt(f->rootStmt);
    if (f->returnType != NULL && f->returnType->kind == k_typeStruct) {
        addToStructList(f->returnType);
    }
    if (f->inputParams == NULL) return;
    for (TYPESPEC *cur = f->inputParams; cur; cur=cur->next) {
        if (cur->type->kind == k_typeStruct) {
            addToStructList(cur->type);
        }
    }
}

void generateStructVarDecl(VARSPEC *vs) {
    for (VARSPEC *cur = vs; cur; cur = cur->next) {
        if (cur->type->kind == k_typeStruct) {
            addToStructList(cur->type);
        }
    }
}

void generateStructTypeSpec(TYPESPEC *ts) {
    for (TYPESPEC *cur = ts; cur; cur = cur->next) {
        if (cur->type->kind == k_typeStruct) {
            addToStructList(cur->type);
        }
    }
}

void generateStructStmt(STMT *s) {
    if (s == NULL) return;
    generateStructStmt(s->next);
    switch (s->kind) {
        case k_stmtKindVarDecl:
            generateStructVarDecl(s->val.varDecl);
            break; 
        case k_stmtKindTypeDecl:
            generateStructTypeSpec(s->val.typeDecl);
            break; 
        case k_stmtKindBlock:
            generateStructStmt(s->val.blockStmt);
            break;
        case k_stmtKindIfStmt:
            generateStructStmt(s->val.ifStmt.trueBody);
            generateStructStmt(s->val.ifStmt.falseBody);
            break;
        case k_stmtKindSwitch:
            if (s->val.switchStmt.caseClauses == NULL) return;
            for (EXPRCASECLAUSE *cur = s->val.switchStmt.caseClauses; cur; cur = cur->next) {
                generateStructStmt(cur->stmtList);
            }
            break;
        case k_stmtKindFor:
            generateStructStmt(s->val.forLoop.body);
            break; 
        default:
            break;
    }
}

void addToStructList(TYPE *t) {
    int curTag = checkExists(t);
    // Check if struct was previously declared; if so, add tag number and exit
    if (curTag != -1) {
        t->val.structType.codegenTag = curTag;
        return;
    }
    // Else, add tag number and increment codegenTag
    // Print out struct
}

int checkExists(TYPE *t) {
    StructElement *cur = head;
    while (cur != NULL) {
        if (isEqualType(cur->type, t)) {
            return cur->type->val.structType.codegenTag;
        }
        cur = cur->next;
    }
    return -1;
}
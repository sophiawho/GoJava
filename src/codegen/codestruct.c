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

void generateCopyHelper(int codegenTag, STRUCTSPEC *structSpec) {
    fprintf(outputFile, "\n\tpublic __golite__struct__%d copy(){\n", codegenTag);

    indent = 2;

    generateINDENT(indent);
    fprintf(outputFile, "__golite__struct__%d copy = new __golite__struct__%d();\n", codegenTag, codegenTag);
    generateINDENT(indent);

    for (STRUCTSPEC *ss = structSpec; ss; ss=ss->next) {
        bool generatedField = false;
        for (IDENT *i = ss->attribute; i; i=i->next) {
            if (isBlankId(i->ident)) continue;
            generatedField = true;
            fprintf(outputFile, "copy.%s = this.%s", prepend(i->ident), prepend(i->ident));
            if (i->next) {
                fprintf(outputFile, ";\n"); generateINDENT(indent);
            }
        }
        if (ss->next && generatedField) {
            fprintf(outputFile, ";\n"); generateINDENT(indent);
        }
    }
    fprintf(outputFile, ";\n");

    generateINDENT(indent); fprintf(outputFile, "return copy;\n");

    indent = 0;

    fprintf(outputFile, "\t}\n");
}

// Edge case: Set uninitialized value of string arrays to be ""
void generateConstructor(int codegenTag, STRUCTSPEC *structSpec) {
    fprintf(outputFile, "\n\tpublic __golite__struct__%d(){\n", codegenTag);
    indent = 2;
    for (STRUCTSPEC *ss = structSpec; ss; ss=ss->next) {
        if (ss->type->kind != k_typeArray) continue;
        char *arrayType = getStringFromType(ss->type->val.arrayType.type, true);
        if (strcmp(arrayType, "String") != 0) continue;
        for (IDENT *i = ss->attribute; i; i=i->next) {
            if (isBlankId(i->ident)) continue;
            generateINDENT(indent);
            fprintf(outputFile, "Arrays.fill(%s, \"\");\n", prepend(i->ident));
        }
    }

    indent = 0;
    fprintf(outputFile, "\t}\n");
}

void addToStructList(TYPE *t) {
    int curTag = checkExists(t);
    // Check if struct was previously declared; if so, add tag number and exit
    if (curTag != -1) {
        t->val.structType.codegenTag = curTag;
        return;
    }
    // Else, add tag number and increment codegenTag
    t->val.structType.codegenTag = codegenTag;
    
    // Print out struct
    fprintf(outputFile, "\nclass __golite__struct__%d {\n", codegenTag);
    indent++;
    generateSTRUCTSPEC(t->val.structType.structSpec);
    indent--;

    // .copy function
    generateCopyHelper(codegenTag, t->val.structType.structSpec);

    // constructor function
    generateConstructor(codegenTag, t->val.structType.structSpec);

    fprintf(outputFile, "}\n");
    codegenTag++;

    // Add type to current list of structs
    StructElement *se = makeStructElement(t);
    if (head == NULL) {
        head = se;
    } else {
        StructElement *cur = head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = se;
    }
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
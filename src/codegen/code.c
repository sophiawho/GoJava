#include "../ast/tree.h"
#include "../ast/stmt.h"
#include "codestmt.h"

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code.h"

int initFuncCounter = 0;

void generateINDENT(int indent){
    for (int i = 0; i < indent; i++) {
        fprintf(outputFile, "\t");
    }
}

// Helper function to prepend all identifiers with `__golite__`
char *prepend(char *str) {
    char *prefix = "__golite__";
    char *output = malloc((strlen(str) + strlen(prefix) + 1) * sizeof(char));
    sprintf(output, "%s%s", prefix, str);
    return output;
}

void generatePROG(PROG *root, char *filename) {
    if (root == NULL) return;
    indent = 0; 
    initFuncCounter = 0;
    openOutputFile(filename);

    char *basec = strdup(filename);
    char *className = basename(basec);
    void generateImports();
    generateHeader(className);
    generateTOPLEVELDECL(root->rootTopLevelDecl);
    // Use `prepend` to prepend __golite__ to all function and identifier names, ie: main() becomes __golite__main()
    // Special case for init functions: Since there may be multiple, append a unique counter to the function name in lexical order, ie __golite__init_0, __golite__init_1, etc
    generateFooter();

    fclose(outputFile);
}

void generateTOPLEVELDECL(TOPLEVELDECL *tld) {
    if (tld == NULL) return;
    generateTOPLEVELDECL(tld->next);
    switch (tld->kind) {
        case k_topLevelDeclFunc:
            generateFUNC(tld->val.funcDecl);
            break;
        case k_topLevelDeclType:
            break;
        case k_topLevelDeclVar:
            break;
    }
}

void generateFUNC(FUNC *f) {
    // Java function header
    if (strcmp(f->name, "_") == 0) {
        return;
    } else if (strcmp(f->name, "init") == 0) {
        fprintf(outputFile, "\n\t@SuppressWarnings({\"unchecked\", \"deprecation\"})\n");
        fprintf(outputFile, "\tpublic static void %s_%d() {\n", prepend(f->name), initFuncCounter);
        initFuncCounter++;
    } else {
        // TODO print return type instead of void
        fprintf(outputFile, "\n\t@SuppressWarnings({\"unchecked\", \"deprecation\"})\n");
        fprintf(outputFile, "\tpublic static void %s() {\n", prepend(f->name));
    }
    indent=2;
    generateSTMT(f->rootStmt->val.blockStmt);
    indent=0;
    // TODO print function parameters
    fprintf(outputFile, "\t}\n");
}

void generateImports()
{
    fprintf(outputFile, "org.apache.commons.lang3.ArrayUtils;\n");
}

void generateHeader(char *className) {
    // Copy over helper Slice.java
    FILE *sliceClass = fopen("helpers/Slice.java", "r");
    if (sliceClass) {
        char *line = (char*)malloc(1024);
        while (fgets(line, sizeof(line), sliceClass)) {
            fputs(line, outputFile);
        }
        fclose(sliceClass);
    } else {
        printf("Helper class Slice.java not in project.\n");
        exit(1);
    }
    fprintf(outputFile, "\n\npublic class %s {\n", className);
}

void generateFooter() {
    fprintf(outputFile, "\n\t@SuppressWarnings({\"unchecked\", \"deprecation\"})\n");
    fprintf(outputFile, "\tpublic static void main(String[] args) {\n");
    for (int i = 0; i < initFuncCounter; i++) {
        fprintf(outputFile, "\t\t__golite__init_%d();\n", i);
    }
    fprintf(outputFile, "\t\t__golite__main();\n");
    fprintf(outputFile, "\t}\n");
    fprintf(outputFile, "}\n");
}

// Helper function to open file pointer for writing generated Java code
void openOutputFile(char *filename) {
    char *outputFileName = malloc((strlen(filename)+6) * sizeof(char));
    for (int i=0; filename[i]; i++) {
        outputFileName[i] = filename[i];
    }
    strcat(outputFileName, ".java");
    outputFile = fopen(outputFileName, "w+");
}
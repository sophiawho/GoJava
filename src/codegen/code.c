#include "../ast/tree.h"
#include "../ast/stmt.h"

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code.h"

FILE *outputFile;
int indent = 0;

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
    openOutputFile(filename);

    char *basec = strdup(filename);
    char *className = basename(basec);
    generateHeader(className);
    // TODO generate code for all top level declarations
    // Use `prepend` to prepend __golite__ to all function and identifier names, ie: main() becomes __golite__main()
    // Special case for init functions: Since there may be multiple, append a unique counter to the function name in lexical order, ie __golite__init_0, __golite__init_1, etc
    generateFooter();

    fclose(outputFile);
}

void generateHeader(char *className) {
    fprintf(outputFile, "public class %s {\n", className);
}

void generateFooter() {
    fprintf(outputFile, "\n\tpublic static void main(String[] args) {\n");
    // TODO Call 0+ __golite__init functions; Somehow keep track of number of init functions
    // for (int i = 0; i < num_init_functions; i++) {
    //     fprintf(outputFile, "\t\t__golite__init_%d();\n", i);
    // }
    fprintf(outputFile, "\t\t__golite__main();\n");
    fprintf(outputFile, "\t}\n");
    fprintf(outputFile, "}\n");
}

// Helper function to open file pointer for writing generated Java code
void openOutputFile(char *filename) {
    char *outputFileName = malloc((strlen(filename)+5)*sizeof(char));
    for (int i=0; filename[i]; i++) {
        outputFileName[i] = filename[i];
    }
    strcat(outputFileName, ".java");
    outputFile = fopen(outputFileName, "w+");
}
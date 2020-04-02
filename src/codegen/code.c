#include "../ast/tree.h"
#include "../ast/stmt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code.h"

FILE *outputFile;
int indent = 0;

void generatePROG(PROG *root, char *filename) {
    if (root == NULL) return;
    indent = 0;
    openOutputFile(filename);
    fclose(outputFile);
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
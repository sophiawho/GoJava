#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "error.h"

int errors = 0;

void throwError(char *description, int lineno)
{
    fprintf(stderr, "Error: (line %d) %s\n", lineno, description);
    exit(EXIT_FAILURE);
}

void throwSpecialFunctionDeclarationError(char *id, int lineno) {
    fprintf(stderr, "Error: (line %d) %s must be a function\n", lineno, id);
    exit(EXIT_FAILURE);
}

void throwSpecialFunctionParameterError(char *id, int lineno) {
    fprintf(stderr, "Error: (line %d) %s must have no parameters and no return value\n", lineno, id);
    exit(EXIT_FAILURE);
}

void throwErrorUndefinedId(int lineno, char *id)
{
    fprintf(stderr, "Error: (line %d) \"%s\" is not declared\n", lineno, id);
    exit(EXIT_FAILURE);
}

void throwErrorRedeclaredId(int lineno, char *id)
{
    fprintf(stderr, "Error: (line %d) \"%s\" is already declared\n", lineno, id);
    exit(EXIT_FAILURE);
}

void throwInternalError(char *description, int lineno)
{
    fprintf(stderr, "Internal Error: (line %d) %s\n", lineno, description);
    exit(EXIT_FAILURE);
}

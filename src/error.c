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

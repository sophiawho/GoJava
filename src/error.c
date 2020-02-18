#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "error.h"

int errors = 0;

// void yyerror(char *s)
// {
//     fprintf(stderr,"%s\n",s);
//     fprintf(stderr,"Error: (line %d) syntax error before %s\n", 
//         lineno, yytext);
//     fprintf(stderr,"*** compilation terminated\n");
//     exit(EXIT_FAILURE);
// }

void throwError(char *description, int lineno)
{
    fprintf(stderr, "Error: (line %d) %s", lineno, description);
    exit(EXIT_FAILURE);
}

// void reportError(char *s, int lineno)
// {
//     fprintf(stderr, "Error: (line %d)", lineno);
//     printf("*** %s at line %i of file %s\n",s,lineno,currentfile);
//     errors++;
// }

// void reportStrError(char *s, char *name, int lineno)
// {
//     printf("*** ");
//     printf(s,name);
//     printf(" at line %i of file %s\n",lineno,currentfile);
//     errors++;
// }

void reportGlobalError(char *s)
{
    printf("*** %s\n",s);
    errors++;
}

void reportStrGlobalError(char *s, char *name)
{
    printf("*** ");
    printf(s,name);
    printf("\n");
    errors++;
}

void noErrors()
{
    if (errors!=0) 
    {
        fprintf(stderr,"*** compilation terminated\n");
        exit(EXIT_FAILURE);
    }
}

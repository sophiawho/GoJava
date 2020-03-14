#include <stdio.h>
#include <string.h>

#include "ast/tree.h"
#include "typecheck/symbol.h"
#include "pretty.h"
#include "weed.h"

void yyparse();
int yylex();
PROG *root = NULL;

int main(int argc, char *argv[])
{
        extern int g_tokens;
        
        if(strcmp(argv[1], "scan") == 0){
                g_tokens = 0;
                while(yylex()) {}
                printf("OK\n");
        }
        else if (strcmp(argv[1], "tokens") == 0){
                g_tokens = 1;
                while(yylex()) {}
                printf("OK\n");
        }
        else if (strcmp(argv[1], "parse") == 0) {
                g_tokens = 0;
                yyparse();
                weedPROG(root);
                printf("OK\n");
        }
        else if (strcmp(argv[1], "pretty") == 0) {
                g_tokens = 0;
                yyparse();
                pretty_PROG(root);
        }
        else if (strcmp(argv[1], "symbol") == 0) {
                g_tokens = 0;
                yyparse();
                weedPROG(root);
                print_sym_table = 1;
                makeSymbolTable(root);
        }
        else if (strcmp(argv[1], "typecheck") == 0) {
                g_tokens = 0;
                yyparse();
                weedPROG(root);
                makeSymbolTable(root);
                printf("OK\n");
        }

        return 0;
}

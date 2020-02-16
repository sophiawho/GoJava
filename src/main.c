#include <stdio.h>
#include <string.h>

void yyparse();
int yylex();

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
                printf("OK\n");
        }

        return 0;
}

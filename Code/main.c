#include <stdio.h>

int yyrestart(FILE* f);
int yyparse();
int yyerror(char* msg);

int main(int argc, char const* argv[])
{
    if(argc <= 1){
        return 1;
    }
    FILE* f = fopen(argv[1], "r");
    if(!f){
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    yyparse();
    return 0;
}
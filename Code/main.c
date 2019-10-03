#include "parser_tree.h"
#include <stdio.h>

int yyrestart(FILE* f);
int yyparse();
int yyerror(char* msg);
extern int lexical_error;

int main(int argc, char const* argv[])
{
    init_st();
    int ret = -1;
    if(argc <= 1){
        return 1;
    }
    FILE* f = fopen(argv[1], "r");
    if(!f){
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    ret = yyparse();
    // printf("yyparse() returns %d\n", ret);
    if (ret == 0 && lexical_error == 0)
        print_tree(global_root, 0);
    return 0;
}
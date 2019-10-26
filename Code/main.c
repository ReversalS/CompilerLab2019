#include "semantic.h"
// #include <stdio.h>

int yyrestart(FILE* f);
int yyparse();
int yyerror(char* msg);
int syntax_error = 0;
extern int lexical_error;
extern int yylineno;
// extern int yydebug;

int main(int argc, char const* argv[])
{
    init_type_set();
    initSymbolTable();
    if (argc <= 1) {
        return 1;
    }
    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    // yydebug = 1;
    yyparse();
    if (syntax_error == 0 && lexical_error == 0){
        // print_tree(global_root, 0);
        ;
    }
    if (global_root != NULL){
        deconstruct(global_root);
    }
    return 0;
}

int yyerror(char* msg)
{
    printf("Error type B at Line %d: %s\n", yylineno, msg);
    syntax_error = 1;
    return 0;
}
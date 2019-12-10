#include "semantic.h"
#include "trans_program.h"
#include "compile.h"
// #include <stdio.h>
#define RELEASE

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

    int t1, t2;
    t1 = construct_function("read", construct_basic(INT_BASIC));
    insertSymbol("read", 1, t1, 0, 0, 0);
    t2 = construct_function("write", construct_basic(INT_BASIC));
    add_member(t2, "isikagigopmionem", construct_basic(INT_BASIC));
    insertSymbol("write", 1, t2, 0, 0, 0);

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

    // if (syntax_error == 0 && lexical_error == 0){
    //     print_tree(global_root, 0);
    // }
    translate_Program(global_root);
#ifdef RELEASE
    if (argc <= 2) {
        return 1;
    }
    init(argv[2]);
    // FILE* out = fopen(argv[2], "w");
    // if (!out) {
    //     perror(argv[2]);
    //     return 1;
    // }
    // PRINT(out, global_root);
    // fp = stdout;

    compile(&global_root->code);
    fclose(fp);
#else
    PRINT(stdout, global_root);
#endif

    if (global_root != NULL) {
        // deconstruct_code(&global_root->code);
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
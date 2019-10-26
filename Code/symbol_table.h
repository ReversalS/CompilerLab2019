#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "enums.h"

#define TABLE_SIZE 256
#define STACK_DEPTH 16

// typedef struct Function Function;
// typedef struct Parameter Parameter;
// typedef struct Struct Struct;
// typedef struct Variable Variable;

// struct Function{
//     char* name;
//     Type* return_type;
//     Parameter* parameter;
//     int parameter_num;
//     Location location;
// };

// struct Parameter{
//     char* name;
//     Type* type;
// };

// struct Struct{
//     char* name;
//     Type* type;
//     Location* location;
// };

// struct Variable{
//     char* name;
//     Type* type;
//     Location* location;
// };

// Function function_table[TABLE_SIZE];
// Struct struct_table[TABLE_SIZE];
// Variable global_variable_table[TABLE_SIZE];

// Variable* local_variable_stack[STACK_SIZE];
// int stack_top = 0;

// // init, deconstruct, etc

// void push_table();
// void pop_table();

// // return true or false
// int exist_function(char* name);
// int exist_struct(char* name);
// int exist_global_variable(char* name);
// int exist_local_variable(char* name);
// int exist_variable(char* name);

// // return Type*
// Type* get_variable_type(char* name);
// Type* get_struct_type(char* name);
// Type* get_parameter_type(char* func_name, int index);

// // insert
// void insert_function(char* name, Type* return_type, int lineno, int column);
// void add_parameter(char* func_name, Type* type, char* para_name);
// void insert_struct(char* name, Type* type, int lineno, int column);
// void insert_global_variable(char* name, Type* type, int lineno, int column);
// void insert_local_variable(char* name, Type* type, int lineno, int column);


/*------------------------------------------------
* 建议的模块设计
* Functional Style
* define USE_RBTREE to switch from list to red-black tree
-------------------------------------------------*/

typedef struct Symbol Symbol;

#define USE_RBTREE

#ifdef USE_RBTREE

    #include "rbtree.h"
    typedef struct {
        struct rb_root root;
        int nodeCount;
    } StackItem;

    struct ExtendedRBnode {
        struct rb_node rb_node;
        char key[32];
        int index;  //index in the symbol table
    };

    typedef struct ExtendedRBnode erb_node;

    void constructRBTree(struct rb_root* root);
    void destructRBTree(struct rb_root* root);
    erb_node *searchNode(struct rb_root* root, char* key);
    int insertNode(struct rb_root* root, char* key, int index);
    void deleteNode(struct rb_root* root, char* key);
    void printRBTree(struct rb_root* root);   //further designed

    #define myfree(ptr) free(ptr)
    #define truncate31(str) str[31]='\0'

#else

    #include "list.h"

#endif

struct Symbol{
    int id;
    int type_id;
    Location location;
    int attribute_id;
};

#define FILL_SYMBOL(dest, name, tid, row, col, attr)\
    dest.id = symbolCount;/* name id or not */\
    dest.type_id = type_id;\
    dest.location.line = lineno;\
    dest.location.column = column;\
    dest.attribute_id = attribute_id;    

#define COPY_SYMBOL(dest, src) memcpy(dest, src, sizeof(Symbol))

Symbol symbol_table[TABLE_SIZE];
int symbolCount;
StackItem symbol_stack[STACK_DEPTH];
int stackTop;
StackItem prior_symbols; //for structure definitions and functions

void initSymbolTable();

int pushSymbolStack();

int popSymbolStack();

void printSymbolStack();

Symbol* getSymbol(char* name, int is_prior);

int insertSymbol(char* name, int is_prior, int type_id, int lineno, int column, int attribute_id);

#define updateSymbolType(id) symbol_table[id].type_id = id

#endif
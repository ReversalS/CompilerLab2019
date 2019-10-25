#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

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
-------------------------------------------------*/

typedef struct Symbol Symbol;

#ifdef USE_RBTREE
// #include "rbtree.h"
// typedef struct rb_root StackItem;
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
void initSymbolTable();

StackItem symbol_stack[STACK_DEPTH];
int stackTop;

/* operating symbol stack */
int pushSymbolStack();
int popSymbolStack();
void printSymbolStack();

/* operating current symbol stack frame */
Symbol* getSymbol(char* name);
int insertSymbol(char* name, int type_id, int lineno, int column, int attribute_id);
#define existSymbol(name) (getSymbol(name) != NULL)
#define getSymbolType(name) getSymbol(name)->type_id

#endif
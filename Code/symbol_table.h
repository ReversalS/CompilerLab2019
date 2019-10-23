#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "enums.h"

#define TABLE_SIZE 256
#define STACK_SIZE 16

typedef struct Function Function;
typedef struct Parameter Parameter;
typedef struct Struct Struct;
typedef struct Variable Variable;

struct Function{
    char* name;
    Type* return_type;
    Parameter* parameter;
    int parameter_num;
    Location location;
};

struct Parameter{
    char* name;
    Type* type;
};

struct Struct{
    char* name;
    Type* type;
    Location* location;
};

struct Variable{
    char* name;
    Type* type;
    Location* location;
};

Function function_table[TABLE_SIZE];
Struct struct_table[TABLE_SIZE];
Variable global_variable_table[TABLE_SIZE];

Variable* local_variable_stack[STACK_SIZE];
int stack_top = 0;

// init, deconstruct, etc

void push_table();
void pop_table();

// return true or false
int exist_function(char* name);
int exist_struct(char* name);
int exist_global_variable(char* name);
int exist_local_variable(char* name);
int exist_variable(char* name);

// return Type*
Type* get_variable_type(char* name);
Type* get_struct_type(char* name);
Type* get_parameter_type(char* func_name, int index);

// insert
void insert_function(char* name, Type* return_type, int lineno, int column);
void add_parameter(char* func_name, Type* type, char* para_name);
void insert_struct(char* name, Type* type, int lineno, int column);
void insert_global_variable(char* name, Type* type, int lineno, int column);
void insert_local_variable(char* name, Type* type, int lineno, int column);


#endif
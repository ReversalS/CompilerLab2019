#ifndef TYPES_H
#define TYPES_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "errors.h"

typedef struct Type Type;
typedef struct FieldList FieldList;
#define TRUE 1
#define FALSE 0

enum KIND{
    BASIC, ARRAY, STRUCTURE
};

enum B_TYPE{
    INT_B, FLOAT_B
};

struct Type{
    enum KIND kind;
    union {
        enum B_TYPE basic;
        struct{
            Type* elem;
            int size;
        } array;
        struct {
            char* name;
            FieldList* fields;
        } structure;
    } u;
};

struct FieldList{
    char* name;
    Type* type;
    FieldList* next;
};

int eq(Type* t1, Type* t2);
void deconstruct_type(Type* p);
void deconstruct_field(FieldList* p);
void copy_type(Type** dest, Type* src);
void copy_field(FieldList** dest, FieldList* src);
void copy_name(char** dest, char* src);

Type* construct_basic(enum B_TYPE type);
Type* construct_array(Type* base, int size);
Type* construct_struct(char* name);
void add_field(Type* base, char* name, Type* type);

void print_type(Type* p, int level, int newline);
void print_array(Type* p, int level);
void print_field(FieldList* p, int level);
#endif
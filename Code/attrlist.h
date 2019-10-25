#ifndef ATTRLIST_H
#define ATTRLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AttrList AttrList;
typedef enum ATTR_TYPE ATTR_TYPE;
typedef struct Var_Dec Var_Dec;

enum ATTR_TYPE {
    ARRAY_SIZE,
    VAR_DEC
};

struct Var_Dec {
    char* id;
    int opt_array_size_num;
    int* opt_array_size;
};

struct AttrList {
    ATTR_TYPE kind;
    union {
        int array_size;
        Var_Dec var_dec;
    } attr;
    AttrList* next;
};

void deconstruct_attrlist(AttrList* p);
void copy_str(char** dest, char* src);
// copy construct
// TODO:
void copy_attrlist(AttrList** dest, AttrList* src);
void insert(AttrList** p, ATTR_TYPE type, void* val);
void append(AttrList** p, ATTR_TYPE type, void* val);
// constructor
// TODO:
void update(AttrList* p, ATTR_TYPE type, void* val);

#endif
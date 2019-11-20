#ifndef ATTRLIST_H
#define ATTRLIST_H

/********** include and macro **********/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********** typedef **********/
typedef struct AttrList AttrList;
typedef enum ATTR_TYPE ATTR_TYPE;
typedef struct Var_Dec Var_Dec;
typedef struct Var_Def Var_Def;
typedef struct Para Para;

/********** structure definition **********/
enum ATTR_TYPE {
    ARRAY_SIZE,
    VAR_DEC,
    VAR_DEF,
    PARA
};

struct Var_Dec {
    char* id;
    int opt_array_size_num;
    int* opt_array_size;
    int opt_init_type_id;
};

struct Var_Def{
    int var_num;
    char** id;
    int *type_id;
    int *init;
    int line;
};

struct Para{
    char* id;
    int type;
    int line;
};

struct AttrList {
    ATTR_TYPE kind;
    union {
        // vardec
        long array_size;
        // format vardec
        Var_Dec var_dec;
        // vardec with type
        Var_Def var_def;
        // func para
        Para para;
    } attr;
    AttrList* next;
};

/********** function definition **********/
void deconstruct_attrlist(AttrList* p);
void copy_str(char** dest, char* src);

// copy construct
void copy_attrlist(AttrList** dest, AttrList* src);
void insert(AttrList** p, ATTR_TYPE type, void* val);
void append(AttrList** p, ATTR_TYPE type, void* val);

// constructor
void update(AttrList* p, ATTR_TYPE type, void* val);

#endif
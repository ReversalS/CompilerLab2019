#ifndef BASIC_BLOCK_H
#define BASIC_BLOCK_H

/********** include and macro **********/
#include "ic_list.h"
#include "container.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********** typedef **********/
typedef struct BasicBlock BasicBlock;
typedef BasicBlock* BBList;
typedef struct VarInfo VarInfo;

/********** structure definition **********/
struct BasicBlock{
    Code code;
    Dict var_info;
    int ic_num;
    BBList prev;
    BBList next;
};

struct VarInfo{
    int dirt;
    int ic_num;
    int current_use;
    int* use;
};

/********** function definition **********/
BBList split(Code* code);
char* op2key(Operand* op);
void init_var_info(Code* code, Dict* var_info, int* ic_num);
void insert_var(Dict* dict, Operand* op, int ic_num);
void print_var_info(void* key, void* value);
void set_use(VarInfo* t, int pos);

#endif
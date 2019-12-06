#ifndef BASIC_BLOCK_H
#define BASIC_BLOCK_H

/********** include and macro **********/
#include "ic_list.h"
#include "gene_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********** typedef **********/
typedef struct BasicBlock BasicBlock;
typedef BasicBlock* BBList;

/********** structure definition **********/
struct BasicBlock{
    Code code;
    BBList prev;
    BBList next;
    List vars;
};

struct VarInfo{
    char id[32];
    int offset;
    int next_live_pos;
};

/********** function definition **********/
BBList split(Code* code);
void init_vars(BBList bb);
void insert_var(List* list, Operand* op);
int vars_eq(void* left, void* right);
void vars_print(void* data);

#endif
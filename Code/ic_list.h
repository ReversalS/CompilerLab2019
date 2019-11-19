#ifndef IC_LIST_H
#define IC_LIST_H

/********** include and macro **********/
#include "ir.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********** typedef **********/
typedef struct ListNode ListNode;
typedef struct Code Code;

/********** structure definition **********/
struct ListNode {
    InterCode* ic;
    ListNode* prev;
    ListNode* next;
};

struct Code{
    ListNode* start;
    ListNode* end;
};

/********** function definition **********/
ListNode* create_list_node(InterCode* ic);
void concatenate_code(Code* result, Code* c1, Code* c2);
void append_ic(Code* result, Code* code, ListNode* node);
void insert_ic(Code* result, Code* code, ListNode* node);
void remove_ic(Code* code, ListNode* node);
void deconstruct_code(Code* code);
void print_code(FILE* fp, Code* code);

#endif
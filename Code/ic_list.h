#ifndef IC_LIST_H
#define IC_LIST_H

/********** include and macro **********/
#include "ic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define is_null(code) (code->start == NULL && code->end == NULL)
#define CREATE(root, node) create_code(&root->code, node)
#define CONCAT(root, child1, child2) concatenate_code(&root->code, &child1->code, &child2->code)
#define APPEND1(root, node) append_ic(&root->code, &root->code, node)
#define APPEND2(root, child, node) append_ic(&root->code, &child->code, node)
#define INSERT1(root, node) insert_ic(&root->code, &root->code, node)
#define INSERT2(root, child, node) insert_ic(&root->code, &child->code, node)
#define PRINT(fp, root) print_code(fp, &root->code);

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

void create_code(Code* code, ListNode* node);
void concatenate_code(Code* result, Code* c1, Code* c2);
void append_ic(Code* result, Code* code, ListNode* node);
void insert_ic(Code* result, Code* code, ListNode* node);
void remove_ic(Code* code, ListNode* node);

void deconstruct_code(Code* code);

void print_code(FILE* fp, Code* code);

#endif
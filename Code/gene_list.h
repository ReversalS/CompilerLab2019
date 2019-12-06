#ifndef GENE_LIST_H
#define GENE_LIST_H

/********** include and macro **********/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********** typedef **********/
typedef struct List List;
typedef struct Cell Cell;
typedef Cell* ListIter;

/********** structure definition **********/
struct Cell{
    void* data;
    Cell* prev;
    Cell* next;
};

struct List{
    Cell head;
    ListIter begin;
    ListIter end;
    int size;
    int (*eq)(void*, void*);
    void (*print)(void*);
};

/********** function definition **********/
void construct_list(List* list, int (*eq)(void*, void*),void(*print)(void*));
void insert_data(List* list, void* data);
void insert_data_u(List* list, void* data);
void remove_data(List* list, void* data);
void deconstruct_list(List* list);
ListIter find(List* list, void* data);
void print_list(List* list);

#endif
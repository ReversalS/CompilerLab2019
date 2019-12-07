#ifndef CONTAINER_H
#define CONTAINER_H

/********** include and macro **********/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********** typedef **********/
typedef struct List List;
typedef struct Cell Cell;
typedef Cell* ListIter;

typedef struct Dict Dict;
typedef struct Item Item;
typedef Item* DictIter;

/********** structure definition **********/
struct Item {
    void* key;
    void* value;
    Item* prev;
    Item* next;
};

struct Dict {
    Item head;
    DictIter begin, end;
    int size;
    int (*eq)(void*, void*);
    void (*print)(void*, void*);
};

struct Cell {
    void* data;
    Cell* prev;
    Cell* next;
};

struct List {
    Cell head;
    ListIter begin, end;
    int size;
    int (*eq)(void*, void*);
    void (*print)(void*);
};

/********** function definition **********/
char* string(char* str);
char* string_t(int id);
int str_eq(void* str1, void* str2);

void construct_dict(Dict* dict, int(*eq)(void*, void*), void(*print)(void*, void*));
void insert_item(Dict* dict, void* key, void* value);
void remove_item(Dict* dict, void* key);
void deconstruct_dict(Dict* dict);
DictIter find_dict(Dict* dict, void* key);
void print_dict(Dict* dict);

void construct_list(List* list, int (*eq)(void*, void*), void (*print)(void*));
void insert_data(List* list, void* data);
void insert_data_u(List* list, void* data);
void remove_data(List* list, void* data);
void deconstruct_list(List* list);
ListIter find_list(List* list, void* data);
void print_list(List* list);

#endif
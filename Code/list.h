#ifndef LIST_H
#define LIST_H

#include <stdio.h>

struct Symbol;

struct ListNode{
    char* name;
    int index;  //index in symbol table
    struct ListNode* next;
};

typedef struct {
    struct ListNode head;
    int nodeCount;  //symbol count in this stack frame (for symbol table recycling)
} StackItem;

void constructList(StackItem* list);
int destructList(StackItem* list);
int insertNode(StackItem* list, char* name, int index);
int removeNode(StackItem* list, char* name);
int searchNode(StackItem* list, char* name);

void printList(StackItem* list);

#endif
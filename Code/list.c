#include "list.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void constructList(StackItem* list) {
    list->head.next = NULL;
    list->nodeCount = 0;
}

/* return node count in order to free symbol table */
int destructList(StackItem* list) {
    if (list->head.next == NULL)
        return 0;   //directly return 0, may (not very likely) have problem with nodeCount
    struct ListNode* p = list->head.next;
    struct ListNode* del;
    while (p != NULL) {
        del = p;
        p = p->next;
        free(del);
    }
    return list->nodeCount;
}

/* return -1 if failed */
int insertNode(StackItem* list, char* name, int index) {
    struct ListNode* newnode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newnode->name = name;
    newnode->index = index;
    newnode->next = list->head.next;
    list->head.next = newnode;
    //increment node count
    list->nodeCount++;
    return 0;
}

/* return -1 if failed */
int removeNode(StackItem* list, char* name) {
    struct ListNode *p = list->head.next;
    struct ListNode* q = &(list->head);
    while (strcmp(p->name, name) != 0 && p != NULL) {
        p = p->next;
        q = q->next;
    }
    if (p != NULL) {
        q->next = p->next;
        free(p);    //TODO: to be modified to adapt to scene
    }
}

int searchNode(StackItem* list, char* name) {
    struct ListNode* p = list->head.next;
    while (p != NULL) {
        if (strcmp(p->name, name) == 0)
            break;
        p = p->next;
    }
    return p->index;
}

void printList(StackItem* list) {
    struct ListNode* p = list->head.next;
    while (p != NULL) {
        printf("%s ", p->name);
        p = p->next;
    }
    printf("\n");
}
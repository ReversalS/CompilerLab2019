#include "ic_list.h"

ListNode* create_list_node(InterCode* ic)
{
    ListNode* temp = (ListNode*)malloc(sizeof(ListNode));
    memset(temp, 0, sizeof(ListNode));
    temp->ic = ic;
    return temp;
}

void concatenate_code(Code* result, Code* c1, Code* c2)
{
    if (c1->end == NULL || c2->start == NULL) {
        return;
    } else {
        c1->end->next = c2->start;
        c2->start->prev = c1->end;
        result->start = c1->start;
        result->end = c2->end;
    }
}

void append_ic(Code* result, Code* code, ListNode* node)
{
    if (code->end == NULL) {
        return;
    } else {
        code->end->next = node;
        node->prev = code->end;
        result->start = code->start;
        result->end = node;
    }
}

void insert_ic(Code* result, Code* code, ListNode* node)
{
    if (code->start == NULL) {
        return;
    } else {
        node->next = code->start;
        code->start->prev = node;
        result->start = node;
        result->end = code->end;
    }
}

void remove_ic(Code* code, ListNode* node){
    if(code->end == node){
        code->end = node->prev;
    }
    if(code->start == node){
        code->start = node->next;
    }
    if(node->next != NULL && node->prev != NULL){
        node->next->prev = node->prev;
        node->prev->next = node->next;
    } else if(node->next == NULL && node->prev != NULL){
        node->prev->next = node->next;
    } else if(node->next != NULL && node->prev == NULL){
        node->next->prev = node->prev;
    } else {
        code->start = code->end = NULL;
    }
    deconstruct_ic(node->ic);
    free(node);
}


void deconstruct_code(Code* code)
{
    ListNode* p = code->start;
    ListNode* prev = code->start->prev;
    ListNode* next = code->end->next;
    while (p != code->end) {
        p = code->start;
        code->start = code->start->next;
        deconstruct_ic(p->ic);
        free(p);
    }
    deconstruct_ic(p->ic);
    free(p);
    code->start = code->end = NULL;
    if(prev != NULL){
        prev->next = next;
    }
    if(next != NULL){
        next->prev = prev;
    }
}

void print_code(FILE* fp, Code* code)
{
    char* temp;
    ListNode* p = code->start;
    while (p != code->end) {
        temp = ic_to_string(p->ic);
        fprintf(fp, "%s\n", temp);
        free(temp);
        p = p->next;
    }
    temp = ic_to_string(p->ic);
    fprintf(fp, "%s\n", temp);
    free(temp);
}
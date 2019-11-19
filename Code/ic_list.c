#include "ic_list.h"

ListNode* create_list_node(InterCode* ic){
    ListNode* temp = (ListNode*)malloc(sizeof(ListNode));
    memset(temp, 0, sizeof(ListNode));
    temp->ic = ic;
    return temp;
}

void concatenate_code(Code* c1, Code* c2){
    if(c1->end == NULL || c2->start == NULL){
        return ;
    } else {
        c1->end->next = c2->start;
        c2->start->prev = c1->end;
        c1->end = c2->end;
    }
}

void deconstruct_code(Code* code){
    ListNode* p = code->start;
    while(p != code->end){
        p = code->start;
        code->start = code->start->next;
        deconstruct_ic(p->ic);
        free(p);
    }
    deconstruct_ic(p->ic);
    free(p);
    code->start = code->end = NULL;
}

void print_code(FILE* fp, Code* code){
    char* temp;
    ListNode* p = code->start;
    while(p != NULL){
        temp = ic_to_string(p->ic);
        fprintf(fp, "%s\n", temp);
        free(temp);
        p = p->next;
    }
}
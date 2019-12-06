#include "basic_block.h"

BBList split(Code* code)
{
    if (is_null(code)) {
        return NULL;
    }
    BBList head = (BBList)malloc(sizeof(BasicBlock));
    memset(head, 0, sizeof(BasicBlock));
    BBList tail = head;
    head->code.start = code->start;
    ListNode* p = code->start->next;
    while (p != NULL && p->prev != code->end) {
        if (p->ic->kind == LABEL_DEF_ST || p->ic->kind == CALL_FUNC_ST) {
            BBList temp = (BBList)malloc(sizeof(BasicBlock));
            memset(temp, 0, sizeof(BasicBlock));
            tail->next = temp;
            temp->prev = tail;
            temp->code.start = p;
            tail = temp;
        } else if (p->prev != NULL) {
            if (p->prev->ic->kind == GOTO_ST || p->prev->ic->kind == IF_ST || p->prev->ic->kind == RETURN_ST || p->prev->ic->kind == CALL_FUNC_ST) {
                BBList temp = (BBList)malloc(sizeof(BasicBlock));
                memset(temp, 0, sizeof(BasicBlock));
                tail->next = temp;
                temp->prev = tail;
                temp->code.start = p;
                tail = temp;
            }
        }
        p = p->next;
    }
    BBList temp = head->next;
    while (temp != NULL) {
        temp->prev->code.end = temp->code.start->prev;
        temp = temp->next;
    }
    tail->code.end = code->end;
    return head;
}

void init_vars(BBList bb)
{
    construct_list(&bb->vars, vars_eq, vars_print);
    ListNode* p = bb->code.start;
    while (p != NULL && p->prev != bb->code.end) {
        insert_var(&bb->vars, p->ic->op1);
        insert_var(&bb->vars, p->ic->op2);
        insert_var(&bb->vars, p->ic->result);
        p = p->next;
    }
}

void insert_var(List* list, Operand* op)
{
    if (op != NULL) {
        char* str = (char*)malloc(32);
        memset(str, 0, 32);
        if (op->kind == VAR) {
            sprintf(str, "%s", op->u.var_name);
        } else if (op->kind == TEMP) {
            sprintf(str, "t%d", op->u.temp_id);
        } else {
            return;
        }
        insert_data_u(list, (void*)str);
    }
}

int vars_eq(void* left, void* right)
{
    if (strcmp((char*)left, (char*)right) == 0) {
        return 1;
    } else {
        return 0;
    }
}

void vars_print(void* data)
{
    printf("%s\n", (char*)data);
}
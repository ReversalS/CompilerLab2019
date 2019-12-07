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

char* op2key(Operand* op)
{
    if (op != NULL && op->kind != CONST) {
        if (op->kind == VAR) {
            return string(op->u.var_name);
        } else {
            return string_t(op->u.temp_id);
        }
    } else {
        return NULL;
    }
}

void init_var_info(Code* code, Dict* var_info, int* ic_num)
{
    ListNode* p = code->start;
    int current_ic;
    construct_dict(var_info, str_eq, print_var_info);
    int i = 0;
    while (p != NULL && p->prev != code->end) {
        i++;
        p = p->next;
    }
    *ic_num = i;
    p = code->start;
    while (p != NULL && p->prev != code->end) {
        insert_var(var_info, p->ic->op1, i);
        insert_var(var_info, p->ic->op2, i);
        insert_var(var_info, p->ic->result, i);
        p = p->next;
    }
    current_ic = i;
    p = code->end;
    while (p != NULL && p->next != code->start) {
        char* op1_key = op2key(p->ic->op1);
        char* op2_key = op2key(p->ic->op2);
        char* result_key = op2key(p->ic->result);
        DictIter op1_info = find_dict(var_info, op1_key);
        DictIter op2_info = find_dict(var_info, op2_key);
        DictIter result_info = find_dict(var_info, result_key);
        VarInfo* t1 = (op1_info == var_info->end) ? NULL : (VarInfo*)op1_info->value;
        VarInfo* t2 = (op2_info == var_info->end) ? NULL : (VarInfo*)op2_info->value;
        VarInfo* tr = (result_info == var_info->end) ? NULL : (VarInfo*)result_info->value;

        for(DictIter i = var_info->begin; i != var_info->end; i = i->next){
            VarInfo* t = (VarInfo*)i->value;
            t->use[current_ic - 1] = t->current_use;
        }

        if (p->ic->kind == ASSIGN_ST) {
            set_use(t1, NEVER_USE);
            set_use(t2, current_ic - 1);
        } else if (p->ic->kind == BIN_ST || p->ic->kind == IF_ST) {
            set_use(tr, NEVER_USE);
            set_use(t1, current_ic - 1);
            set_use(t2, current_ic - 1);
        } else if (p->ic->kind == RETURN_ST || p->ic->kind == ARG_ST || p->ic->kind == WRITE_ST) {
            set_use(t1, current_ic - 1);
        } else if (p->ic->kind == DEC_ST || p->ic->kind == CALL_FUNC_ST || p->ic->kind == READ_ST){
            set_use(t1, NEVER_USE);
        }

        free(op1_key);
        free(op2_key);
        free(result_key);
        p = p->prev;
        current_ic--;
    }
    print_dict(var_info);
}

void insert_var(Dict* dict, Operand* op, int ic_num)
{
    if (op != NULL) {
        if (op->kind == CONST) {
            return;
        } else {
            char* key;
            if (op->kind == VAR) {
                key = string(op->u.var_name);
            } else {
                key = string_t(op->u.temp_id);
            }
            DictIter i = find_dict(dict, key);
            if (i == dict->end) {
                VarInfo* temp = (VarInfo*)malloc(sizeof(VarInfo));
                memset(temp, 0, sizeof(VarInfo));
                temp->dirt = 0;
                temp->current_use = USE_IN_THE_FUTURE;
                temp->ic_num = ic_num;
                temp->use = (int*)malloc(ic_num * sizeof(int));
                memset(temp->use, 0, ic_num * sizeof(int));
                insert_item(dict, key, (void*)temp);
            } else {
                free(key);
            }
        }
    }
}

void print_var_info(void* key, void* value)
{
    printf("%s: ", (char*)key);
    VarInfo* t = (VarInfo*)value;
    for(int i = 0; i < t->ic_num; i++){
        printf("%d ", t->use[i]);
    }
    printf("\n");
}

void set_use(VarInfo* t, int pos){
    if (t != NULL){
        t->current_use = pos;
    }
}
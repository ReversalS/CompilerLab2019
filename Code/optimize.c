#include "optimize.h"

void simplify_bin(ListNode* p)
{
    int v1, v2, res;
    v1 = p->ic->op1->u.ival;
    v2 = p->ic->op2->u.ival;
    switch (p->ic->code.bin_kind) {
    case PLUS_IC:
        res = v1 + v2;
        break;
    case MINUS_IC:
        res = v1 - v2;
        break;
    case STAR_IC:
        res = v1 * v2;
        break;
    case DIV_IC:
        res = v1 / v2;
        break;
    }
    InterCode* temp = p->ic;
    p->ic = create_assign(NORMAL, temp->result, create_const(res));
    deconstruct_op(temp->op1);
    deconstruct_op(temp->op2);
    free(temp);
}

// TODO
void remove_imm_assign(Code* code)
{
    int current_id;
    enum { FINISH,
        REPLACE_TEMP,
        SIMPLIFY_BIN } state
        = FINISH;
    int imm_val;

    ListNode* p = code->start;
    ListNode* temp;
    if (p == NULL) {
        return;
    }
    do {
        while (p != code->end) {
            switch (state) {
            case FINISH:
                if (can_replace(p->ic)) {
                    current_id = p->ic->op1->u.temp_id;
                    imm_val = p->ic->op2->u.ival;
                    temp = p;
                    p = p->next;
                    remove_ic(code, temp);
                    state = REPLACE_TEMP;
                } else if (can_simplify(p->ic)) {
                    simplify_bin(p);
                    p = p->next;
                    state = SIMPLIFY_BIN;
                } else {
                    p = p->next;
                }
                break;
            case REPLACE_TEMP:
            case SIMPLIFY_BIN:
            }
        }
    } while (state != FINISH);
}
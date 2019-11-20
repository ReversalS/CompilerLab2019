#include "ir.h"

int temp_id = 0;
int label_id = 0;

void init_temp_state()
{
    for (int i = 0; i < MAX_TEMP_NUM; i++) {
        temp_state[i].state = NONACTIVE;
        temp_state[i].u.imm_val = 0;
        temp_state[i].u.var_name = NULL;
    }
}

char* op_to_string(Operand* op)
{
    char* temp = (char*)malloc(sizeof(char) * OPERAND_LEN);
    memset(temp, 0, sizeof(char) * OPERAND_LEN);
    switch (op->kind) {
    case VAR:
        sprintf(temp, "%s", op->u.var_name);
        break;
    case TEMP:
        sprintf(temp, "t%d", op->u.temp_id);
        break;
    case CONST:
        sprintf(temp, "#%d", op->u.ival);
        break;
    }
    return temp;
}

Operand* create_op(OperandType type, char* val)
{
    Operand* temp = (Operand*)malloc(sizeof(Operand));
    memset(temp, 0, sizeof(Operand));
    temp->kind = type;
    switch (type) {
    case VAR:
        copy_str(&temp->u.var_name, val);
        break;
    case TEMP:
        temp->u.temp_id = temp_id;
        temp_state[temp_id].state = ACTIVE;
        temp_id++;
        break;
    }
    return temp;
}

Operand* create_const(int val)
{
    Operand* temp = (Operand*)malloc(sizeof(Operand));
    memset(temp, 0, sizeof(Operand));
    temp->kind = CONST;
    temp->u.ival = val;
    return temp;
}

InterCode* create_ic()
{
    InterCode* temp = (InterCode*)malloc(sizeof(InterCode));
    memset(temp, 0, sizeof(InterCode));
    return temp;
}

char* ic_to_string(InterCode* ic)
{
    char* temp = (char*)malloc(sizeof(char) * CODE_LEN);
    char* t1;
    char* t2;
    char* t3;
    memset(temp, 0, sizeof(char) * CODE_LEN);
    switch (ic->kind) {
    case LABEL_DEF_ST:
        sprintf(temp, "LABEL label%d :", ic->code.label_id);
        break;
    case FUNC_DEF_ST:
        sprintf(temp, "FUNCTION %s :", ic->code.func_name);
        break;
    case ASSIGN_ST:
        t1 = op_to_string(ic->op1);
        t2 = op_to_string(ic->op2);
        switch (ic->code.assign_kind) {
        case NORMAL:
            sprintf(temp, "%s := %s", t1, t2);
            break;
        case GET_ADDR:
            sprintf(temp, "%s := &%s", t1, t2);
            break;
        case DEREF_LEFT:
            sprintf(temp, "*%s := %s", t1, t2);
            break;
        case DEREF_RIGHT:
            sprintf(temp, "%s := *%s", t1, t2);
            break;
        }
        free(t1);
        free(t2);
        break;
    case BIN_ST:
        t1 = op_to_string(ic->result);
        t2 = op_to_string(ic->op1);
        t3 = op_to_string(ic->op2);
        switch (ic->code.bin_kind) {
        case PLUS_IC:
            sprintf(temp, "%s := %s + %s", t1, t2, t3);
            break;
        case MINUS_IC:
            sprintf(temp, "%s := %s - %s", t1, t2, t3);
            break;
        case STAR_IC:
            sprintf(temp, "%s := %s * %s", t1, t2, t3);
            break;
        case DIV_IC:
            sprintf(temp, "%s := %s / %s", t1, t2, t3);
            break;
        }
        free(t1);
        free(t2);
        free(t3);
        break;
    case GOTO_ST:
        sprintf(temp, "GOTO label%d", ic->code.label_id);
        break;
    case IF_ST:
        t1 = op_to_string(ic->op1);
        t2 = op_to_string(ic->op2);
        switch (ic->code.if_stmt.relop) {
        case GT_IC:
            sprintf(temp, "IF %s > %s GOTO label%d", t1, t2, ic->code.if_stmt.label_id);
            break;
        case LT_IC:
            sprintf(temp, "IF %s < %s GOTO label%d", t1, t2, ic->code.if_stmt.label_id);
            break;
        case GE_IC:
            sprintf(temp, "IF %s >= %s GOTO label%d", t1, t2, ic->code.if_stmt.label_id);
            break;
        case LE_IC:
            sprintf(temp, "IF %s <= %s GOTO label%d", t1, t2, ic->code.if_stmt.label_id);
            break;
        case EQ_IC:
            sprintf(temp, "IF %s == %s GOTO label%d", t1, t2, ic->code.if_stmt.label_id);
            break;
        case NE_IC:
            sprintf(temp, "IF %s != %s GOTO label%d", t1, t2, ic->code.if_stmt.label_id);
            break;
        }
        free(t1);
        free(t2);
        break;
    case RETURN_ST:
        t1 = op_to_string(ic->op1);
        sprintf(temp, "RETURN %s", t1);
        free(t1);
        break;
    case DEC_ST:
        t1 = op_to_string(ic->op1);
        sprintf(temp, "DEC %s %d", t1, ic->code.dec_size);
        free(t1);
        break;
    case ARG_ST:
        t1 = op_to_string(ic->op1);
        sprintf(temp, "ARG %s", t1);
        free(t1);
        break;
    case CALL_FUNC_ST:
        t1 = op_to_string(ic->op1);
        sprintf(temp, "%s := CALL %s", t1, ic->code.func_name);
        free(t1);
        break;
    case PARAM_ST:
        t1 = op_to_string(ic->op1);
        sprintf(temp, "PARAM %s", t1);
        free(t1);
        break;
    case READ_ST:
        t1 = op_to_string(ic->op1);
        sprintf(temp, "READ %s", t1);
        free(t1);
        break;
    case WRITE_ST:
        t1 = op_to_string(ic->op1);
        sprintf(temp, "WRITE %s", t1);
        free(t1);
        break;
    }
    return temp;
}

InterCode* create_label()
{
    InterCode* temp = create_ic();
    temp->kind = LABEL_DEF_ST;
    temp->code.label_id = label_id;
    label_id++;
    return temp;
}

InterCode* create_func_def(char* name)
{
    InterCode* temp = create_ic();
    temp->kind = FUNC_DEF_ST;
    copy_str(&temp->code.func_name, name);
    return temp;
}

InterCode* create_assign(AssignType type, Operand* left, Operand* right)
{
    InterCode* temp = create_ic();
    temp->kind = ASSIGN_ST;
    temp->code.assign_kind = type;
    temp->op1 = left;
    temp->op2 = right;
    // test
    if(left->kind == TEMP){
        if(temp_state[left->u.temp_id].state == ACTIVE){
            if(type == NORMAL && right->kind == VAR){
                temp_state[left->u.temp_id].state = ASSIGNED_VAR;
                copy_str(&temp_state[left->u.temp_id].u.var_name, right->u.var_name);
            } else if(type == NORMAL && right->kind == CONST){
                temp_state[left->u.temp_id].state = ASSIGNED_CONST;
                temp_state[left->u.temp_id].u.imm_val = right->u.ival;
            } else {
                temp_state[left->u.temp_id].state = ASSIGNED;
            }
        } else {
            temp_state[left->u.temp_id].state = MULTI_ASSIGNED;
        }
    }
    return temp;
}

InterCode* create_bin(BinType type, Operand* result, Operand* op1, Operand* op2)
{
    InterCode* temp = create_ic();
    temp->kind = BIN_ST;
    temp->code.bin_kind = type;
    temp->result = result;
    temp->op1 = op1;
    temp->op2 = op2;
    if(result->kind == TEMP){
        if(temp_state[result->u.temp_id].state == ACTIVE){
            temp_state[result->u.temp_id].state = ASSIGNED;
        } else {
            temp_state[result->u.temp_id].state = MULTI_ASSIGNED;
        }
    }
    return temp;
}

InterCode* create_goto(int label)
{
    InterCode* temp = create_ic();
    temp->kind = GOTO_ST;
    temp->code.label_id = label;
    return temp;
}

InterCode* create_if(RelopType type, Operand* op1, Operand* op2, int label)
{
    InterCode* temp = create_ic();
    temp->kind = IF_ST;
    temp->code.if_stmt.label_id = label;
    temp->op1 = op1;
    temp->op2 = op2;
    temp->code.if_stmt.relop = type;
    return temp;
}

InterCode* create_return(Operand* return_val)
{
    InterCode* temp = create_ic();
    temp->kind = RETURN_ST;
    temp->op1 = return_val;
    return temp;
}

InterCode* create_dec(Operand* op, int size)
{
    InterCode* temp = create_ic();
    temp->kind = DEC_ST;
    temp->op1 = op;
    temp->code.dec_size = size;
    return temp;
}

InterCode* create_arg(Operand* arg)
{
    InterCode* temp = create_ic();
    temp->kind = ARG_ST;
    temp->op1 = arg;
    return temp;
}

InterCode* create_call_func(Operand* return_val, char* func_name)
{
    InterCode* temp = create_ic();
    temp->kind = CALL_FUNC_ST;
    copy_str(&temp->code.func_name, func_name);
    temp->op1 = return_val;
    return temp;
}

InterCode* create_param(Operand* param)
{
    InterCode* temp = create_ic();
    temp->kind = PARAM_ST;
    temp->op1 = param;
    return temp;
}

InterCode* create_read(Operand* read)
{
    InterCode* temp = create_ic();
    temp->kind = READ_ST;
    temp->op1 = read;
    return temp;
}

InterCode* create_write(Operand* write)
{
    InterCode* temp = create_ic();
    temp->kind = WRITE_ST;
    temp->op1 = write;
    return temp;
}

void deconstruct_op(Operand* op)
{
    if (op == NULL) {
        return;
    }
    switch (op->kind) {
    case VAR:
        free(op->u.var_name);
        break;
    case TEMP:
        temp_state[op->u.temp_id].state = NONACTIVE;
        break;
    }
    free(op);
}
void deconstruct_ic(InterCode* ic)
{
    if (ic == NULL) {
        return;
    }
    switch (ic->kind) {
    case FUNC_DEF_ST:
        free(ic->code.func_name);
        break;
    case CALL_FUNC_ST:
        free(ic->code.func_name);
        break;
    }
    deconstruct_op(ic->op1);
    deconstruct_op(ic->op2);
    deconstruct_op(ic->result);
    free(ic);
}
#include "ir.h"

int temp_id = 0;
int label_id = 0;

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
        temp_id++;
        break;
    }
    return temp;
}

Operand* create_const(int val){
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
        t1 = op_to_string(ic->code.assign.left);
        t2 = op_to_string(ic->code.assign.right);
        switch (ic->code.assign.kind) {
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
        t1 = op_to_string(ic->code.bin.result);
        t2 = op_to_string(ic->code.bin.op1);
        t3 = op_to_string(ic->code.bin.op2);
        switch (ic->code.bin.kind) {
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
        sprintf(temp, "GOTO label%d", ic->code.goto_id);
        break;
    case IF_ST:
        t1 = op_to_string(ic->code.if_statement.op1);
        t2 = op_to_string(ic->code.if_statement.op2);
        switch (ic->code.if_statement.relop) {
        case GT_IC:
            sprintf(temp, "IF %s > %s GOTO label%d", t1, t2, ic->code.if_statement.goto_id);
            break;
        case LT_IC:
            sprintf(temp, "IF %s < %s GOTO label%d", t1, t2, ic->code.if_statement.goto_id);
            break;
        case GE_IC:
            sprintf(temp, "IF %s >= %s GOTO label%d", t1, t2, ic->code.if_statement.goto_id);
            break;
        case LE_IC:
            sprintf(temp, "IF %s <= %s GOTO label%d", t1, t2, ic->code.if_statement.goto_id);
            break;
        case EQ_IC:
            sprintf(temp, "IF %s == %s GOTO label%d", t1, t2, ic->code.if_statement.goto_id);
            break;
        case NE_IC:
            sprintf(temp, "IF %s != %s GOTO label%d", t1, t2, ic->code.if_statement.goto_id);
            break;
        }
        free(t1);
        free(t2);
        break;
    case RETURN_ST:
        t1 = op_to_string(ic->code.return_val);
        sprintf(temp, "RETURN %s", t1);
        free(t1);
        break;
    case DEC_ST:
        t1 = op_to_string(ic->code.dec_statement.op);
        sprintf(temp, "DEC %s %d", t1, ic->code.dec_statement.size);
        free(t1);
        break;
    case ARG_ST:
        t1 = op_to_string(ic->code.arg);
        sprintf(temp, "ARG %s", t1);
        free(t1);
        break;
    case CALL_FUNC_ST:
        t1 = op_to_string(ic->code.call_func_statement.return_val);
        sprintf(temp, "%s := CALL %s", t1, ic->code.call_func_statement.func_name);
        free(t1);
        break;
    case PARAM_ST:
        t1 = op_to_string(ic->code.param);
        sprintf(temp, "PARAM %s", t1);
        free(t1);
        break;
    case READ_ST:
        t1 = op_to_string(ic->code.read);
        sprintf(temp, "READ %s", t1);
        free(t1);
        break;
    case WRITE_ST:
        t1 = op_to_string(ic->code.write);
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
    temp->code.assign.kind = type;
    temp->code.assign.left = left;
    temp->code.assign.right = right;
    return temp;
}

InterCode* create_bin(BinType type, Operand* result, Operand* op1, Operand* op2)
{
    InterCode* temp = create_ic();
    temp->kind = BIN_ST;
    temp->code.bin.kind = type;
    temp->code.bin.result = result;
    temp->code.bin.op1 = op1;
    temp->code.bin.op2 = op2;
    return temp;
}

InterCode* create_goto(int label)
{
    InterCode* temp = create_ic();
    temp->kind = GOTO_ST;
    temp->code.goto_id = label;
    return temp;
}

InterCode* create_if(RelopType type, Operand* op1, Operand* op2, int label)
{
    InterCode* temp = create_ic();
    temp->kind = IF_ST;
    temp->code.if_statement.goto_id = label;
    temp->code.if_statement.op1 = op1;
    temp->code.if_statement.op2 = op2;
    temp->code.if_statement.relop = type;
    return temp;
}

InterCode* create_return(Operand* return_val)
{
    InterCode* temp = create_ic();
    temp->kind = RETURN_ST;
    temp->code.return_val = return_val;
    return temp;
}

InterCode* create_dec(Operand* op, int size)
{
    InterCode* temp = create_ic();
    temp->kind = DEC_ST;
    temp->code.dec_statement.op = op;
    temp->code.dec_statement.size = size;
    return temp;
}

InterCode* create_arg(Operand* arg)
{
    InterCode* temp = create_ic();
    temp->kind = ARG_ST;
    temp->code.arg = arg;
    return temp;
}

InterCode* create_call_func(Operand* return_val, char* func_name)
{
    InterCode* temp = create_ic();
    temp->kind = CALL_FUNC_ST;
    copy_str(&temp->code.call_func_statement.func_name, func_name);
    temp->code.call_func_statement.return_val = return_val;
    return temp;
}

InterCode* create_param(Operand* param)
{
    InterCode* temp = create_ic();
    temp->kind = PARAM_ST;
    temp->code.param = param;
    return temp;
}

InterCode* create_read(Operand* read)
{
    InterCode* temp = create_ic();
    temp->kind = READ_ST;
    temp->code.read = read;
    return temp;
}

InterCode* create_write(Operand* write)
{
    InterCode* temp = create_ic();
    temp->kind = WRITE_ST;
    temp->code.write = write;
    return temp;
}

void deconstruct_op(Operand* op)
{
    switch (op->kind) {
    case VAR:
        free(op->u.var_name);
        break;
    }
    free(op);
}
void deconstruct_ic(InterCode* ic)
{
    switch (ic->kind) {
    case FUNC_DEF_ST:
        free(ic->code.func_name);
        break;
    case ASSIGN_ST:
        deconstruct_op(ic->code.assign.left);
        deconstruct_op(ic->code.assign.right);
        break;
    case BIN_ST:
        deconstruct_op(ic->code.bin.result);
        deconstruct_op(ic->code.bin.op1);
        deconstruct_op(ic->code.bin.op2);
        break;
    case IF_ST:
        deconstruct_op(ic->code.if_statement.op1);
        deconstruct_op(ic->code.if_statement.op2);
        break;
    case RETURN_ST:
        deconstruct_op(ic->code.return_val);
        break;
    case DEC_ST:
        deconstruct_op(ic->code.dec_statement.op);
        break;
    case ARG_ST:
        deconstruct_op(ic->code.arg);
        break;
    case CALL_FUNC_ST:
        deconstruct_op(ic->code.call_func_statement.return_val);
        free(ic->code.call_func_statement.func_name);
        break;
    case PARAM_ST:
        deconstruct_op(ic->code.param);
        break;
    case READ_ST:
        deconstruct_op(ic->code.read);
        break;
    case WRITE_ST:
        deconstruct_op(ic->code.write);
        break;
    }
}
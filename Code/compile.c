#include "compile.h"

void compile(Code* code)
{
    enum { INIT,
        FUNC } state;
    state = INIT;
    ListNode* p = code->start;
    Code c;
    while (p != NULL && p->prev != code->end) {
        switch (state) {
        case INIT:
            if (p->ic->kind == FUNC_DEF_ST) {
                c.start = p;
                state = FUNC;
            }
            break;
        case FUNC:
            if (p->next == NULL || p->next->ic->kind == FUNC_DEF_ST) {
                c.end = p;
                compile_func(&c);
                state = INIT;
            }
            break;
        }
        p = p->next;
    }
}

void compile_func(Code* code)
{
    // TODO func desc
    ListNode* p = code->start;
    Code temp;
    FuncDesc func_desc;
    memset(&func_desc, 0, sizeof(func_desc));
    char* key;
    VarPos* value;
    int n = 0;
    func_desc.current_sp = 0;
    construct_dict(&func_desc.var_pos, str_eq, print_var_pos);
    temp.end = code->end;
    int flag = 0;
    while (p != NULL && p->prev != code->end) {
        switch (p->ic->kind) {
        case FUNC_DEF_ST:
            // TODO emit func name
            emit_func(p->ic->code.func_name);
            emit_push(RA);
            emit_push(FP);
            emit_move(FP, SP);
            // push $ra
            // push $fp
            // mov $fp, $sp
            // printf("func name: %s\n", p->ic->code.func_name);
            break;
        case PARAM_ST:
            // TODO
            key = string(p->ic->op1->u.var_name);
            value = (VarPos*)malloc(sizeof(VarPos));
            value->offset = 8 + 4 * n;
            n++;
            insert_item(&func_desc.var_pos, key, value);
            // printf("param: %s\n", p->ic->op1->u.var_name);
            break;
        default:
            if (!flag) {
                temp.start = p;
                flag = 1;
            }
            insert_var_pos(&func_desc.var_pos, p->ic->op1);
            insert_var_pos(&func_desc.var_pos, p->ic->op2);
            insert_var_pos(&func_desc.var_pos, p->ic->result);
            break;
        }
        p = p->next;
    }
    BBList bblist = split(&temp);
    while (bblist != NULL) {
        // printf("CODE:\n");
        // print_code(stdout, &bblist->code);
        // printf("********\n");
        init_var_info(&bblist->code, &bblist->var_info, &bblist->ic_num);
        // printf("USAGE:\n");
        // print_dict(&bblist->var_info);
        compile_basic_block(&func_desc, bblist);
        // printf("===================\n");
        bblist = bblist->next;
    }
}

void compile_basic_block(FuncDesc* func_desc, BBList bblist)
{
    reset_reg_desc(func_desc, &bblist->var_info);
    ListNode* p = bblist->code.start;
    while (p != NULL && p->prev != bblist->code.end) {
        InterCode* ic = p->ic;
        Operand* op1 = ic->op1;
        Operand* op2 = ic->op2;
        Operand* result = ic->result;
        switch (p->ic->kind) {
        case LABEL_DEF_ST:
            emit_label(p->ic->code.label_id);
            break;
        case ASSIGN_ST:
            compile_assign(p->ic);
            break;
        case GOTO_ST:

            emit_j(p->ic->code.label_id);
            break;
        case DEC_ST:
            compile_dec(p->ic, func_desc);
            break;
        case BIN_ST:
            compile_bin(p->ic);
            break;
        case IF_ST:
            compile_if(p->ic);
            break;
        case RETURN_ST:
            compile_return(p->ic, func_desc);
            break;
        case ARG_ST:
            compile_arg(p->ic, func_desc);
            break;
        case CALL_FUNC_ST:
            compile_call_func(p->ic, func_desc);
            break;
        case READ_ST:
            compile_read(p->ic);
            break;
        case WRITE_ST:
            compile_write(p->ic);
            break;
        default:;
            break;
        }
        UPDATE_IC;
        p = p->next;
    }
    spill_all_reg();
}

void compile_assign(InterCode* ic)
{
    if (ic->code.assign_kind == GET_ADDR) {
        int offset = get_offset(string_t(ic->op2->u.temp_id));
        MIPS_REG dest = allocate(ic->op1->u.var_name);
        emit_addi(dest, FP, offset);
    } else {
        MIPS_REG right, left;
        right = op2reg(ic->op2, ic->is_addr.op2, V0);
        left = left2reg(ic->op1, ic->is_addr.op1);
        if (ic->is_addr.op1) {
            emit_sw(right, left, 0);
        } else {
            emit_move(left, right);
        }
        free_useless_reg(right);
    }
}

void compile_dec(InterCode* ic, FuncDesc* func_desc)
{
    DictIter i = find_dict(&func_desc->var_pos, string_t(ic->op1->u.temp_id));
    if (i == func_desc->var_pos.end) {
        perror("variable not found\n");
        exit(-1);
    } else {
        func_desc->current_sp -= ic->code.dec_size;
        emit_addi(SP, SP, -ic->code.dec_size);
        ((VarPos*)i->value)->offset = func_desc->current_sp;
    }
}

void compile_bin(InterCode* ic)
{
    MIPS_REG op1, op2, dest;
    op1 = op2reg(ic->op1, ic->is_addr.op1, V0);
    op2 = op2reg(ic->op2, ic->is_addr.op2, V1);
    dest = left2reg(ic->result, ic->is_addr.result);

    switch (ic->code.bin_kind) {
    case PLUS_IC:
        if (ic->is_addr.result) {
            emit_add(V0, op1, op2);
            emit_sw(V0, dest, 0);
        } else {
            emit_add(dest, op1, op2);
        }
        break;
    case MINUS_IC:
        if (ic->is_addr.result) {
            emit_sub(V0, op1, op2);
            emit_sw(V0, dest, 0);
        } else {
            emit_sub(dest, op1, op2);
        }
        break;
    case STAR_IC:
        if (ic->is_addr.result) {
            emit_mul(V0, op1, op2);
            emit_sw(V0, dest, 0);
        } else {
            emit_mul(dest, op1, op2);
        }
        break;
    case DIV_IC:
        if (ic->is_addr.result) {
            emit_div(V0, op1, op2);
            emit_sw(V0, dest, 0);
        } else {
            emit_div(dest, op1, op2);
        }
        break;
    }

    free_useless_reg(op1);
    free_useless_reg(op2);
}

void compile_if(InterCode* ic)
{
    MIPS_REG op1, op2;
    op1 = op2reg(ic->op1, ic->is_addr.op1, V0);
    op2 = op2reg(ic->op2, ic->is_addr.op2, V1);
    spill_all_reg();
    emit_branch(ic->code.if_stmt.relop, op1, op2, ic->code.if_stmt.label_id);
}

void compile_return(InterCode* ic, FuncDesc* func_desc)
{
    MIPS_REG op1;
    op1 = op2reg(ic->op1, ic->is_addr.op1, V0);
    emit_return(op1, func_desc->current_sp);
    free_useless_reg(op1);
}

void compile_arg(InterCode* ic, FuncDesc* func_desc)
{
    for (int j = 0; j < AVAIL_REG_NUM; j++) {
        MIPS_REG reg = avail_regs[j];
        if (reg_state[reg].state == OCCUPY) {
            DictIter i = find_dict(current_var_info, reg_state[reg].id);
            if (i == current_var_info->end) {
                perror("variable not found\n");
                exit(-1);
            } else {
                VarInfo* t = (VarInfo*)i->value;
                if (t->dirt == 1) {
                    t->dirt = 0;
                    int offset = get_offset(reg_state[reg].id);
                    emit_sw(reg, FP, offset);
                }
            }
        }
    }
    MIPS_REG op;
    if (ic->op1->kind == CONST) {
        emit_li(V0, ic->op1->u.ival);
        op = V0;
    } else {
        char* key = op2key(ic->op1);
        int found = 0;
        for (int i = 0; i < AVAIL_REG_NUM; i++) {
            MIPS_REG reg = avail_regs[i];
            if (reg_state[reg].state == OCCUPY && strcmp(reg_state[reg].id, key) == 0) {
                found = 1;
                op = reg;
            }
        }
        if (!found) {
            DictIter i = find_dict(&func_desc->var_pos, key);
            if (i == func_desc->var_pos.end) {
                perror("variable not found\n");
                exit(-1);
            } else {
                VarPos* t = (VarPos*)i->value;
                if (t->offset == -1) {
                    perror("fault\n");
                    exit(-1);
                } else {
                    emit_lw(V0, FP, t->offset);
                    op = V0;
                }
            }
        }
        if (ic->is_addr.op1) {
            emit_lw(V0, op, 0);
            op = V0;
        }
    }
    emit_push(op);
    func_desc->arg_num++;
}

void compile_call_func(InterCode* ic, FuncDesc* func_desc)
{
    emit_jal(ic->code.func_name);
    MIPS_REG op1 = left2reg(ic->op1, ic->is_addr.op1);
    emit_move(op1, V0);
    emit_addi(SP, SP, 4 * (func_desc->arg_num));
    func_desc->arg_num = 0;
}

void compile_read(InterCode* ic)
{
    emit_jal("read");
    MIPS_REG op1 = left2reg(ic->op1, ic->is_addr.op1);
    emit_move(op1, V0);
}

void compile_write(InterCode* ic)
{
    MIPS_REG op1 = op2reg(ic->op1, ic->is_addr.op1, V0);
    emit_move(A0, op1);
    emit_jal("write");
}

void init(const char* filename)
{
    fp = fopen(filename, "w");
    if (!fp) {
        perror(filename);
    }
    fprintf(fp, ".data\n");
    fprintf(fp, "_prompt: .asciiz \"Enter an integer: \"\n");
    fprintf(fp, "_ret: .asciiz \"\\n\"\n");
    fprintf(fp, ".globl main\n");
    fprintf(fp, ".text\n");
    fprintf(fp, "\nread:\n");
    fprintf(fp, "\tli $v0, 4\n");
    fprintf(fp, "\tla $a0, _prompt\n");
    fprintf(fp, "\tsyscall\n");
    fprintf(fp, "\tli $v0, 5\n");
    fprintf(fp, "\tsyscall\n");
    fprintf(fp, "\tjr $ra\n");
    fprintf(fp, "\nwrite:\n");
    fprintf(fp, "\tli $v0, 1\n");
    fprintf(fp, "\tsyscall\n");
    fprintf(fp, "\tli $v0, 4\n");
    fprintf(fp, "\tla $a0, _ret\n");
    fprintf(fp, "\tsyscall\n");
    fprintf(fp, "\tmove $v0, $0\n");
    fprintf(fp, "\tjr $ra\n");
}

void insert_var_pos(Dict* var_pos, Operand* op)
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
            DictIter i = find_dict(var_pos, key);
            if (i == var_pos->end) {
                VarPos* temp = (VarPos*)malloc(sizeof(VarPos));
                temp->offset = -1;
                insert_item(var_pos, key, temp);
            }
        }
    }
}

void print_var_pos(void* key, void* value)
{
    printf("%s offset: %d\n", (char*)key, ((VarPos*)value)->offset);
}

MIPS_REG op2reg(Operand* op, int is_addr, MIPS_REG k)
{
    MIPS_REG reg;
    if (op->kind == CONST) {
        emit_li(k, op->u.ival);
        return k;
    } else {
        reg = ensure(op2key(op));
        if (is_addr) {
            emit_lw(k, reg, 0);
            return k;
        } else {
            return reg;
        }
    }
}

MIPS_REG left2reg(Operand* result, int is_addr)
{
    MIPS_REG dest;
    char* key = op2key(result);
    if (is_addr) {
        dest = ensure(key);
    } else {
        dest = allocate(key);
        modify_var(key);
    }
    return dest;
}

void spill_all_reg()
{
    for (int i = 0; i < AVAIL_REG_NUM; i++) {
        MIPS_REG reg = avail_regs[i];
        if (reg_state[reg].state == OCCUPY) {
            spill_reg(reg);
        }
    }
}

void emit_label(int label)
{
    fprintf(fp, "label%d:\n", label);
}

void emit_func(char* func_name)
{
    fprintf(fp, "\n%s:\n", func_name);
}

void emit_j(int label)
{
    fprintf(fp, "\tj label%d\n", label);
}

void emit_jr(MIPS_REG reg)
{
    fprintf(fp, "\tjr %s\n", reg_name[reg]);
}

void emit_jal(char* func_name)
{
    fprintf(fp, "\tjal %s\n", func_name);
}

void emit_return(MIPS_REG dest, int sp)
{
    emit_addi(SP, SP, -sp);
    emit_move(V0, dest);
    emit_pop(FP);
    emit_pop(RA);
    emit_jr(RA);
}

void emit_addi(MIPS_REG dest, MIPS_REG src, int k)
{
    fprintf(fp, "\taddi %s, %s, %d\n", reg_name[dest], reg_name[src], k);
}

void emit_add(MIPS_REG result, MIPS_REG op1, MIPS_REG op2)
{
    fprintf(fp, "\tadd %s, %s, %s\n", reg_name[result], reg_name[op1], reg_name[op2]);
}

void emit_sub(MIPS_REG result, MIPS_REG op1, MIPS_REG op2)
{
    fprintf(fp, "\tsub %s, %s, %s\n", reg_name[result], reg_name[op1], reg_name[op2]);
}

void emit_mul(MIPS_REG result, MIPS_REG op1, MIPS_REG op2)
{
    fprintf(fp, "\tmul %s, %s, %s\n", reg_name[result], reg_name[op1], reg_name[op2]);
}

void emit_div(MIPS_REG result, MIPS_REG op1, MIPS_REG op2)
{
    fprintf(fp, "\tdiv %s, %s\n", reg_name[op1], reg_name[op2]);
    fprintf(fp, "\tmflo %s\n", reg_name[result]);
}

void emit_move(MIPS_REG dest, MIPS_REG src)
{
    fprintf(fp, "\tmove %s, %s\n", reg_name[dest], reg_name[src]);
}

void emit_lw(MIPS_REG rt, MIPS_REG base, int offset)
{
    fprintf(fp, "\tlw %s, %d(%s)\n", reg_name[rt], offset, reg_name[base]);
}

void emit_sw(MIPS_REG rt, MIPS_REG base, int offset)
{
    fprintf(fp, "\tsw %s, %d(%s)\n", reg_name[rt], offset, reg_name[base]);
}

void emit_li(MIPS_REG dest, int imm)
{
    fprintf(fp, "\tli %s, %d\n", reg_name[dest], imm);
}

void emit_push(MIPS_REG dest)
{
    emit_addi(SP, SP, -4);
    emit_sw(dest, SP, 0);
}

void emit_pop(MIPS_REG dest)
{
    emit_lw(dest, SP, 0);
    emit_addi(SP, SP, 4);
}

void emit_branch(RelopType relop, MIPS_REG op1, MIPS_REG op2, int label)
{
    char* x;
    switch (relop) {
    case EQ_IC:
        x = "eq";
        break;
    case NE_IC:
        x = "ne";
        break;
    case GE_IC:
        x = "ge";
        break;
    case GT_IC:
        x = "gt";
        break;
    case LE_IC:
        x = "le";
        break;
    case LT_IC:
        x = "lt";
        break;
    }
    fprintf(fp, "\tb%s %s, %s, label%d\n", x, reg_name[op1], reg_name[op2], label);
}

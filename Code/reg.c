#include "reg.h"

char* reg_name[REG_NUM] = { "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra" };
MIPS_REG avail_regs[AVAIL_REG_NUM] = { S0, S1, S2, S3, S4, S5, S6, S7, A0, A1, A2, A3, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9 };

void reset_reg_desc(FuncDesc* func_desc, Dict* var_info)
{
    for (int i = 0; i < AVAIL_REG_NUM; i++) {
        MIPS_REG reg = avail_regs[i];
        reg_state[reg].state = IDLE;
        memset(reg_state[reg].id, 0, 32);
    }
    current_func_desc = func_desc;
    current_var_info = var_info;
    current_ic = 0;
}

int get_offset(char* x)
{
    DictIter i = find_dict(&current_func_desc->var_pos, x);
    if (i == current_func_desc->var_pos.end) {
        perror("variable not found\n");
        exit(-1);
    } else {
        VarPos* temp = (VarPos*)i->value;
        if (temp->offset == -1) {
            current_func_desc->current_sp -= 4;
            temp->offset = current_func_desc->current_sp;
            emit_addi(SP, SP, -4);
        }
        return temp->offset;
    }
}

MIPS_REG ensure(char* x)
{
    for (int i = 0; i < AVAIL_REG_NUM; i++) {
        MIPS_REG reg = avail_regs[i];
        if (reg_state[reg].state == OCCUPY && strcmp(reg_state[reg].id, x) == 0) {
            return reg;
        }
    }
    MIPS_REG result = allocate(x);
    reg_state[result].state = OCCUPY;
    strcpy(reg_state[result].id, x);
    emit_lw(result, FP, get_offset(x));
    DictIter i = find_dict(current_var_info, x);
    if (i == current_var_info->end) {
        perror("variable not found\n");
        exit(-1);
    } else {
        ((VarInfo*)i->value)->dirt = 0;
        printf("load from memory\n");
    }
    return result;
}

MIPS_REG allocate(char* x)
{
    int max_use = -1;
    MIPS_REG max_reg = ZERO;
    for (int i = 0; i < AVAIL_REG_NUM; i++) {
        MIPS_REG reg = avail_regs[i];
        if (reg_state[reg].state == IDLE) {
            reg_state[reg].state = OCCUPY;
            strcpy(reg_state[reg].id, x);
            return reg;
        }
    }
    for (int i = 0; i < AVAIL_REG_NUM; i++) {
        VarInfo* t1;
        MIPS_REG reg = avail_regs[i];
        DictIter i = find_dict(current_var_info, reg_state[reg].id);
        if (i == current_var_info->end) {
            perror("variable not found\n");
            exit(-1);
        } else {
            t1 = (VarInfo*)i->value;
            if (t1->use[current_ic] == NEVER_USE) {
                return reg;
            } else {
                if (t1->use[current_ic] > max_use) {
                    max_use = t1->use[current_ic];
                    max_reg = reg;
                }
            }
        }
    }
    spill_reg(max_reg);
    reg_state[max_reg].state = OCCUPY;
    strcpy(reg_state[max_reg].id, x);
    return max_reg;
}

void free_reg(MIPS_REG reg)
{
    memset(reg_state[reg].id, 0, 32);
    reg_state[reg].state = IDLE;
}

void free_useless_reg(MIPS_REG reg){
    if(reg_state[reg].state == OCCUPY){
        DictIter i = find_dict(current_var_info, reg_state[reg].id);
        if( i == current_var_info->end){
            perror("variable not found\n");
            exit(-1);
        } else {
            if(((VarInfo*)i->value)->use[current_ic] == NEVER_USE){
                free_reg(reg);
            }
        }
    }
}

void spill_reg(MIPS_REG reg)
{
    char* key = reg_state[reg].id;
    int offset = get_offset(key);
    DictIter i = find_dict(current_var_info, key);
    if (i == current_var_info->end) {
        perror("variable not found\n");
        exit(-1);
    } else {
        VarInfo* t = (VarInfo*)i->value;
        if (t->dirt == 1) {
            t->dirt = 0;
            emit_sw(reg, FP, offset);
        }
    }
    free_reg(reg);
}

void modify_var(char* key){
    DictIter i = find_dict(current_var_info, key);
    if(i == current_var_info->end){
        perror("variable not found\n");
        exit(-1);
    } else {
         VarInfo* t = (VarInfo*)i->value;
         t->dirt = 1;
    }
}
#include "reg.h"

char* reg_name[REG_NUM] = { "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra" };
MIPS_REG avail_regs[AVAIL_REG_NUM] = { A0, A1, A2, A3, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, S0, S1, S2, S3, S4, S5, S6, S7 };

void reset_reg_desc()
{
    for (int i = 0; i < AVAIL_REG_NUM; i++) {
        MIPS_REG reg = avail_regs[i];
        reg_state[reg].state = IDLE;
        memset(reg_state[reg].id, 0, 32);
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
    // TODO emit lw result x
    return result;
}

MIPS_REG allocate(char* x)
{
    for (int i = 0; i < AVAIL_REG_NUM; i++) {
        MIPS_REG reg = avail_regs[i];
        if (reg_state[reg].state == IDLE) {
            return reg;
        }
    }
    // TODO spill result
}

void free_reg(MIPS_REG reg){
    memset(reg_state[reg].id, 0, 32);
    reg_state[reg].state = IDLE;
}

void spill_reg(MIPS_REG reg){
    // TODO
}
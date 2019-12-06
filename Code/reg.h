#ifndef REG_H
#define REG_H

#include "gene_list.h"

typedef enum MIPS_REG MIPS_REG;
typedef enum REG_STATE REG_STATE;

enum REG_STATE{
    IDLE,OCCUPY
};

enum MIPS_REG {
    ZERO,
    AT,
    V0,
    V1,
    A0,
    A1,
    A2,
    A3,
    T0,
    T1,
    T2,
    T3,
    T4,
    T5,
    T6,
    T7,
    S0,
    S1,
    S2,
    S3,
    S4,
    S5,
    S6,
    S7,
    T8,
    T9,
    K0,
    K1,
    GP,
    SP,
    FP,
    RA
};

MIPS_REG avail_regs[] = {T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, S0, S1, S2, S3, S4, S5, S6, S7 };

REG_STATE reg_state[32];


#endif
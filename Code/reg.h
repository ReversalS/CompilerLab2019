#ifndef REG_H
#define REG_H

/********** include and macro **********/
#include "container.h"
#define REG_NUM 32
#define AVAIL_REG_NUM 22

/********** typedef **********/
typedef enum MIPS_REG MIPS_REG;
typedef enum REG_STATE REG_STATE;
typedef struct RegDesc RegDesc;

/********** structure definition **********/
enum REG_STATE {
    IDLE,
    OCCUPY
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

struct RegDesc {
    REG_STATE state;
    char id[32];
};

/********** global variable **********/
char* reg_name[REG_NUM];
MIPS_REG avail_regs[AVAIL_REG_NUM];
RegDesc reg_state[REG_NUM];

Dict* var_pos;
Dict* var_info;
int current_ic;

/********** function definition **********/
extern void emit_lw(MIPS_REG rt, MIPS_REG base, int offset);
extern void emit_sw(MIPS_REG rt, MIPS_REG base, int offset);

void reset_reg_desc();
MIPS_REG ensure(char* x);
MIPS_REG allocate(char* x);
void free_reg(MIPS_REG reg);
void spill_reg(MIPS_REG reg);
#endif
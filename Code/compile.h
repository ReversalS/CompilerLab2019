#ifndef COMPILE_H
#define COMPILE_H

#include "basic_block.h"
#include "reg.h"

struct FuncDesc{
    Dict var_pos;
};

struct VarPos{
    int offset;
};

FILE* fp;

void compile(Code* code);
void compile_func(Code* code);

void emit_goto(int label);
void emit_call(char* func_name);
void emit_return(MIPS_REG dest);
void emit_addi(MIPS_REG dest, MIPS_REG src, int k);
void emit_add(MIPS_REG result, MIPS_REG op1, MIPS_REG op2);
void emit_sub(MIPS_REG result, MIPS_REG op1, MIPS_REG op2);
void emit_mul(MIPS_REG result, MIPS_REG op1, MIPS_REG op2);
void emit_div(MIPS_REG result, MIPS_REG op1, MIPS_REG op2);
void emit_move(MIPS_REG dest, MIPS_REG src);
void emit_label(int label);
void emit_func(char* func_name);
void emit_lw(MIPS_REG rt, MIPS_REG base, int offset);
void emit_sw(MIPS_REG rt, MIPS_REG base, int offset);
void emit_push(MIPS_REG dest);
void emit_pop(MIPS_REG dest);
void emit_branch(RelopType relop, MIPS_REG op1, MIPS_REG op2, int label);

#endif
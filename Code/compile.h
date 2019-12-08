#ifndef COMPILE_H
#define COMPILE_H

/********** include and macro **********/
#include "basic_block.h"
#include "reg.h"

/********** global variable **********/
FILE* fp;

/********** function definition **********/
void compile(Code* code);
void compile_func(Code* code);
void compile_basic_block(FuncDesc* func_desc, BBList bblist);

void compile_assign(InterCode* ic);
void compile_dec(InterCode* ic, FuncDesc* func_desc);
void compile_bin(InterCode* ic);

void init(char* filename);
void insert_var_pos(Dict* var_pos, Operand* op);
void print_var_pos(void* key, void* value);
MIPS_REG op2reg(Operand* op, int is_addr, MIPS_REG k);
MIPS_REG left2reg(Operand* result, int is_addr);

void emit_label(int label);
void emit_func(char* func_name);

void emit_j(int label);
void emit_jr(MIPS_REG reg);
void emit_jal(char* func_name);
void emit_return(MIPS_REG dest, int sp);

void emit_addi(MIPS_REG dest, MIPS_REG src, int k);
void emit_add(MIPS_REG result, MIPS_REG op1, MIPS_REG op2);
void emit_sub(MIPS_REG result, MIPS_REG op1, MIPS_REG op2);
void emit_mul(MIPS_REG result, MIPS_REG op1, MIPS_REG op2);
void emit_div(MIPS_REG result, MIPS_REG op1, MIPS_REG op2);
void emit_move(MIPS_REG dest, MIPS_REG src);
void emit_lw(MIPS_REG rt, MIPS_REG base, int offset);
void emit_sw(MIPS_REG rt, MIPS_REG base, int offset);
void emit_li(MIPS_REG dest, int imm);
void emit_push(MIPS_REG dest);
void emit_pop(MIPS_REG dest);
void emit_branch(RelopType relop, MIPS_REG op1, MIPS_REG op2, int label);

#endif
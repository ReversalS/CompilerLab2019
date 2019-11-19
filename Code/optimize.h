#ifndef OPTIMIZE_H
#define OPTIMIZE_H

/********** include and macro **********/
#include "ic_list.h"
#define is_const(op) (op->kind == CONST)
#define is_temp(op) (op->kind == TEMP)
#define can_replace(ic) ((ic->kind == ASSIGN_ST) && (ic->code.assign_kind == NORMAL) && is_temp(ic->op1) && is_const(ic->op2))
#define can_simplify(ic) ((ic->kind == BIN_ST) && is_const(ic->op1) && is_const(ic->op2))
/********** function definition **********/
void remove_imm_assign(Code* code);
void remove_useless_assign(Code* code);


#endif
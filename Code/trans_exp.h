#ifndef TRANS_EXP_H
#define TRANS_EXP_H

/********** include and macro **********/
#include "parser_tree.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define is_not(node) (node->symbol_type == T && node->node_type.t_type == OTHER_E && node->node_value.other == NOT_E)
#define is_and(node) (node->symbol_type == T && node->node_type.t_type == OTHER_E && node->node_value.other == AND_E)
#define is_or(node) (node->symbol_type == T && node->node_type.t_type == OTHER_E && node->node_value.other == OR_E)
#define is_relop(node) (node->symbol_type == T && node->node_type.t_type == RELOP_E)
#define CANNOT_TRANSLATE printf("Cannot translate.\n")
#define can_simplify(op1, op2) (op1->kind == CONST && op2->kind == CONST)

/********** function definition **********/
Operand* translate_Exp(Node* root, int need_result);
Operand* translate_Exp_assign(Node* root);
Operand* translate_Exp_logic(Node* root);
Operand* translate_Exp_arith(Node* root);
Operand* translate_Exp_minus(Node* root);
Operand* translate_Exp_func(Node* root);
Operand* translate_Exp_array(Node* root);
Operand* translate_Exp_basic(Node* root);

void translate_Args(Node* root);

int translate_Cond(Node* exp, InterCode* label_true, InterCode* label_false);

int get_elem_size(int type);
RelopType get_relop(enum RELOP relop);
BinType get_bin(enum OTHER type);

#endif
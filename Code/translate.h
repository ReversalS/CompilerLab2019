#ifndef TRANSLATE_H
#define TRANSLATE_H

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

/********** function definition **********/
void translate_Exp(Node* root, Operand* op);
void translate_Exp_assign(Node* root, Operand* op);
void translate_Exp_logic(Node* root, Operand* op);
void translate_Exp_arith(Node* root, Operand* op);
void translate_Exp_minus(Node* root, Operand* op);
void translate_Exp_func(Node* root, Operand* op);
void translate_Exp_array(Node* root, Operand* op);
void translate_Exp_basic(Node* root, Operand* op);

void translate_Args(Node* root);

void translate_Stmt(Node* root);
void translate_Stmt_if(Node* root);
void translate_Stmt_ifel(Node* root);
void translate_Stmt_while(Node* root);


void translate_Cond(Node* exp, InterCode* label_true, InterCode* label_false);




#endif
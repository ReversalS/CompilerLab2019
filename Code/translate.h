#ifndef TRANSLATE_H
#define TRANSLATE_H

/********** include and macro **********/
#include "parser_tree.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********** function definition **********/
void translate_Exp(Node* root, Operand* op);
void translate_Exp_assign(Node* root, Operand* op);
void translate_Exp_logic(Node* root, Operand* op);
void translate_Exp_arith(Node* root, Operand* op);
void translate_Exp_minus(Node* root, Operand* op);
void translate_Exp_func(Node* root, Operand* op);
void translate_Exp_array(Node* root, Operand* op);
void translate_Exp_basic(Node* root, Operand* op);





#endif
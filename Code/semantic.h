#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "parser_tree.h"
#include "attrlist.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* VarDec */
// VarDec: ID
void var_id(Node* root, Node* id);
// VarDec: VarDec LB INT RB
void var_var_lb_int_rb(Node* root, Node* var, Node* int_n);

/* ExtDecList */
// ExtDecList: VarDec
void extdec_var(Node* root, Node* var);
// ExtDecList: VarDec COMMA ExtDecList
void extdec_var_comma_extdec(Node* root, Node* var, Node* extdec);

/* Specifiers */
// Specifer: TYPE
void spec_type(Node* root, Node* type);
// Specifer: StructSpecifier
void spec_struct(Node* root, Node* stru);

#endif
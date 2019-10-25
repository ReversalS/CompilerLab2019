#ifndef SEMANTIC_H
#define SEMANTIC_H

/********** include and macro **********/
#include "errors.h"

// enums.h, attrlist.h
#include "parser_tree.h"

// types.h
#include "symbol_table.h"

/********** function definition **********/

int legal_struct_def(char* name);
Var_Dec* format_vardec(Node* var);

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

/* StructSpecifier */
// TODO:StructSpecifier: STRUCT OptTag LC DefList RC
// StructSpecifier: STRUCT Tag
void struct_sturct_tag(Node* root, Node* tag);

/* Tag */
// Tag: ID
void tag_id(Node* root, Node* id);

/* OptTag */
// OptTag: ID
void opt_id(Node* root, Node* id);

/* Dec */
// Dec: VarDec
void dec_var(Node* root, Node* var);
// Dec: VarDec ASSIGNOP Exp
void dec_var_assignop_exp(Node* root, Node* var, Node* exp);
#endif
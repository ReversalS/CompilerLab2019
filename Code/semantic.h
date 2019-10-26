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
int init_field(AttrList* vardef_list);
unsigned hash_pjw(char* str);
int dup_field(AttrList* vardef_list);
Var_Dec* format_vardec(Node* var);
Var_Def* format_vardef(Node* var, int type_id);

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
void struct_opt_lc_def_rc(Node* root, Node* opt, Node* def);
// StructSpecifier: STRUCT Tag
void struct_struct_tag(Node* root, Node* tag);

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

/* DecList */
// DecList: Dec
void dec_dec(Node* root, Node* dec);
// DecList: Dec COMMA DecList
void dec_dec_comma_dec(Node* root, Node* dec, Node* dec_list);

/* Def */
// Def: Specifier DecList SEMI
void def_spec_dec_semi(Node* root, Node* spec, Node* dec);

/* DefList */
// DefList: Def DefList
void def_def_def(Node* root, Node* def, Node* deflist);

/* Exp */
// Exp: INT
void exp_int(Node* root);
// Exp: FLOAT
void exp_float(Node* root);
#endif
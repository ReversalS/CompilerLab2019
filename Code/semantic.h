#ifndef SEMANTIC_H
#define SEMANTIC_H

/********** include and macro **********/
#include "errors.h"

// enums.h, attrlist.h
#include "parser_tree.h"

// types.h
#include "symbol_table.h"

#define concat2(dest, s1, s2) \
    char* src[2];             \
    src[0] = s1;              \
    src[1] = s2;              \
    str_concat(&dest, src, 2);

#define concat3(dest, s1, s2, s3) \
    char* src[3];                 \
    src[0] = s1;                  \
    src[1] = s2;                  \
    src[2] = s3;                  \
    str_concat(&dest, src, 3);

#define concat4(dest, s1, s2, s3, s4) \
    char* src[4];                     \
    src[0] = s1;                      \
    src[1] = s2;                      \
    src[2] = s3;                      \
    src[3] = s4;                      \
    str_concat(&dest, src, 4);

/********** function definition **********/

int legal_struct_def(char* name);
int init_field(AttrList* vardef_list);
unsigned hash_pjw(char* str);
int dup_field(AttrList* vardef_list);
void str_concat(char** dest, char* src[], int num);
void insert_func(Node* spec, Node* fun);
Var_Dec* format_vardec(Node* var);
Var_Def* format_vardef(Node* var, int type_id);
Para* format_para(Node* var, int type_id);
Para* format_arg(char* id, int type_id, int line);

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
// StructSpecifier: STRUCT OptTag LC DefList RC
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
void exp_int(Node* root, Node* i);
// Exp: FLOAT
void exp_float(Node* root, Node* f);
// Exp: ID
void exp_id(Node* root, Node* id);
// Exp: Exp DOT ID
void exp_exp_dot_id(Node* root, Node* exp, Node* id);
// Exp: Exp LB Exp RB
void exp_exp_lb_exp_rb(Node* root, Node* exp, Node* size);
// Exp: ID LP RP
void exp_id_lp_rp(Node* root, Node* id);
// Exp: ID LP Args RP
void exp_id_lp_args_rp(Node* root, Node* id, Node* args);
// Exp: NOT Exp
void exp_not_exp(Node* root, Node* exp);
// Exp: MINUS Exp
void exp_minus_exp(Node* root, Node* exp);
// Exp: LP Exp RP
void exp_lp_exp_rp(Node* root, Node* exp);
// Exp: Exp DIV Exp
void exp_exp_div_exp(Node* root, Node* left, Node* right);
// Exp: Exp STAR Exp
void exp_exp_star_exp(Node* root, Node* left, Node* right);
// Exp: Exp MINUS Exp
void exp_exp_minus_exp(Node* root, Node* left, Node* right);
// Exp: Exp PLUS Exp
void exp_exp_plus_exp(Node* root, Node* left, Node* right);
// Exp: Exp RELOP Exp
void exp_exp_relop_exp(Node* root, Node* left, Node* rel, Node* right);
// Exp: Exp OR Exp
void exp_exp_or_exp(Node* root, Node* left, Node* right);
// Exp: Exp AND Exp
void exp_exp_and_exp(Node* root, Node* left, Node* right);
// Exp: Exp ASSIGNOP Exp
void exp_exp_assignop_exp(Node* root, Node* left, Node* right);

/* ParamDec */
// ParamDec: Specifier VarDec
void param_spec_var(Node* root, Node* spec, Node* var);

/* VarList */
// VarList: ParamDec COMMA VarList
void var_para_comma_var(Node* root, Node* para, Node* var);
// VarList: ParamDec
void var_para(Node* root, Node* para);

/* FunDec */
// FunDec: ID LP VarList RP
void fun_id_lp_var_rp(Node* root, Node* id, Node* var);
// FunDec: ID LP RP
void fun_id_lp_rp(Node* root, Node* id);

/* ExtDef */
// ExtDef: Specifier ExtDecList SEMI
void extdef_spec_extdec_semi(Node* root, Node* spec, Node* extdec);
// ExtDef: Specifier SEMI
void extdef_spec_semi(Node* root, Node* spec);
// ExtDef: Specifier FunDec CompSt
void extdef_spec_fun_comp(Node* root, Node* spec, Node* fun, Node* comp);

/* ExtdefList */
// ExtDefList: ExtDef ExtDefList
void extdef_extdef_extdef(Node* root, Node* extdef, Node* extdeflist);

/* Program */
// Program: ExtDefList
void program_extdef(Node* root, Node* extdef);

/* Args */
// Args: Exp COMMA Args
void arg_exp_comma_arg(Node* root, Node* exp, Node* arg);
// Args: Exp
void arg_exp(Node* root, Node* exp);

/* Stmt */
// Stmt: Exp SEMI
void stmt_exp_semi(Node* root, Node* exp);
// Stmt: CompSt
void stmt_compst(Node* root, Node* compst);
// Stmt: RETURN Exp SEMI
void stmt_return_exp_semi(Node* root, Node* exp);
// Stmt: IF LP Exp RP Stmt
void stmt_if_lp_exp_rp_stmt(Node* root, Node* exp, Node* stmt);
// Stmt: IF LP Exp RP Stmt ELSE Stmt
void stmt_if_lp_exp_rp_stmt_else_stmt(Node* root, Node* exp, Node* if_stmt, Node* else_stmt);
// Stmt: WHILE LP Exp RP Stmt
void stmt_while_lp_exp_rp_stmt(Node* root, Node* exp, Node* stmt);

/* StmtList */
// StmtList: Stmt StmtList
void stmt_stmt_stmt(Node* root, Node* stmt, Node* stmt_list);

/* CompSt */
// CompSt: LC DefList StmtList RC
void compst_lc_def_stmt_rc(Node* root, Node* def, Node* stmt);
#endif
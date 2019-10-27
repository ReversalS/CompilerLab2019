%{
    /* ... */
    #include "lex.yy.c"
    #include "semantic.h"

    int yyerror(const char*);
    int yylex(void);
%}
%error-verbose
// %define api.value.type { struct Node* }
/* declared token */
%token INT
%token FLOAT
%token ID
%token TYPE
%token AND OR RELOP PLUS MINUS STAR DIV ASSIGNOP NOT
%token LP RP
%token LB RB
%token LC RC
%token COMMA DOT SEMI
%token WHILE RETURN STRUCT IF ELSE

/* priority */
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left DOT
%left LB RB //brackets '[' ']'
%left LP RP //parenthesis '(' ')'
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

/* High-level Definitions */
Program: ExtDefList {
        // FORM_SUBTREE(1, PROGRAM);
        FORM_SUBTREE_1($$,PROGRAM,$1);
        global_root = $$;
        program_extdef($$, $1);
    };
ExtDefList: ExtDef ExtDefList {
        FORM_SUBTREE_2($$,EXTDEFLIST,$1,$2)
        extdef_extdef_extdef($$, $1, $2);
    }
    | /* empty */ {
        $$ = create_EP();
    }
    ;
ExtDef: Specifier ExtDecList SEMI {
        FORM_SUBTREE_3($$,EXTDEF,$1,$2,$3);
        extdef_spec_extdec_semi($$, $1, $2);
    }
    | Specifier SEMI {
        FORM_SUBTREE_2($$,EXTDEF,$1,$2);
        extdef_spec_semi($$, $1);
    }
    | Specifier FunDec CompSt {
        FORM_SUBTREE_3($$,EXTDEF,$1,$2,$3);
        extdef_spec_fun_comp($$, $1, $2, $3);
    }
    | error SEMI {
        $$ = create_EP();
    }
    | Specifier error SEMI {
        $$ = create_EP();
    }
    | Specifier error Specifier FunDec CompSt {
        $$ = create_EP();
    }
    ;
ExtDecList: VarDec  {
        FORM_SUBTREE_1($$,EXTDECLIST,$1);
        extdec_var($$, $1);
        // AttrList* p = $$->attr.var_list;
        // while(p != NULL){
        //     if(p->attr.var_dec.opt_array_size_num > 0){
        //         for(int i = 0; i < p->attr.var_dec.opt_array_size_num; i++){
        //             printf("[%d]", p->attr.var_dec.opt_array_size[i]);
        //         }
        //         printf("\n");
        //     }
        //     p = p->next;
        // }
    }
    | VarDec COMMA ExtDecList   {
        FORM_SUBTREE_3($$,EXTDECLIST,$1,$2,$3);
        extdec_var_comma_extdec($$, $1, $3);
        // AttrList* p = $$->attr.var_list;
        // while(p != NULL){
        //     printf("id: %s\n", p->attr.var_dec.id);
        //     printf("dim: %d\n", p->attr.var_dec.opt_array_size_num);
        //     p = p->next;
        // }
        // printf("========\n");
    }
    | error COMMA ExtDecList {
        $$ = create_EP();
    }
    ;

/* Specifiers */
Specifier: TYPE {
        FORM_SUBTREE_1($$,SPECIFIER,$1);
        spec_type($$, $1);
    }
    | StructSpecifier {
        FORM_SUBTREE_1($$,SPECIFIER,$1);
        spec_struct($$, $1);
    }
    ;
StructSpecifier: STRUCT OptTag LC DefList RC {
        FORM_SUBTREE_5($$,STRUCTSPECIFIER,$1,$2,$3,$4,$5)
        struct_opt_lc_def_rc($$, $2, $4);
    }
    | STRUCT Tag {
        FORM_SUBTREE_2($$,STRUCTSPECIFIER,$1,$2)
        struct_struct_tag($$, $2);
    }
    ;
OptTag: ID {
        FORM_SUBTREE_1($$,OPTTAG,$1)
        opt_id($$, $1);
    }
    | /* empty */ {
        $$ = create_EP();
    }
    ;
Tag: ID {
        FORM_SUBTREE_1($$,TAG,$1)
        tag_id($$, $1);
    }
    ;

/* Declarators */
VarDec: ID  {
        FORM_SUBTREE_1($$,VARDEC,$1)
        var_id($$, $1);
    }
    | VarDec LB INT RB  {
        FORM_SUBTREE_4($$,VARDEC,$1,$2,$3,$4)
        var_var_lb_int_rb($$, $1, $3);
    }
    | VarDec LB error RB {
        $$ = create_EP();
    }
    ;
FunDec: ID LP VarList RP  {
        FORM_SUBTREE_4($$,FUNDEC,$1,$2,$3,$4)
        fun_id_lp_var_rp($$, $1, $3);
        // printf("%s (", $$->attr.id);
        // AttrList* p = $$->attr.para_list;
        // while(p != NULL){
        //     printf(" %s ", p->attr.para.id);
        //     print_type(p->attr.para.type, 0, 0);
        //     p = p->next;
        // }
        // printf(")\n");
    }
    | ID LP RP  {
        FORM_SUBTREE_3($$,FUNDEC,$1,$2,$3)
        fun_id_lp_rp($$, $1);
        // printf("%s (", $$->attr.id);
        // AttrList* p = $$->attr.para_list;
        // while(p != NULL){
        //     printf(" %s ", p->attr.para.id);
        //     print_type(p->attr.para.type, 0, 0);
        //     p = p->next;
        // }
        // printf(")\n");
    }
    | ID LP error RP {
        $$ = create_EP();
    }
    ;
VarList: ParamDec COMMA VarList {
        FORM_SUBTREE_3($$,VARLIST,$1,$2,$3)
        var_para_comma_var($$, $1, $3);
    }
    | ParamDec {
        FORM_SUBTREE_1($$,VARLIST,$1)
        var_para($$, $1);
    }
    | error COMMA VarList {
        $$ = create_EP();
    }
    ;
ParamDec: Specifier VarDec {
        FORM_SUBTREE_2($$,PARAMDEC,$1,$2)
        param_spec_var($$, $1, $2);
    }
    ;

/* Statements */
CompSt: LC DefList StmtList RC {
        FORM_SUBTREE_4($$,COMPST,$1,$2,$3,$4)
        compst_lc_def_stmt_rc($$, $2, $3);
    }
    ;
StmtList: Stmt StmtList {
        FORM_SUBTREE_2($$,STMTLIST,$1,$2)
        stmt_stmt_stmt($$, $1, $2);
    }
    | /* empty */ {
        $$ = create_EP();
    }
    ;
Stmt: Exp SEMI {
        FORM_SUBTREE_2($$,STMT,$1,$2)
        stmt_exp_semi($$, $1);
    }
    | CompSt {
        FORM_SUBTREE_1($$,STMT,$1)
        stmt_compst($$, $1);
    }
    | RETURN Exp SEMI {
        FORM_SUBTREE_3($$,STMT,$1,$2,$3)
        stmt_return_exp_semi($$, $2);
    }
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {
        FORM_SUBTREE_5($$,STMT,$1,$2,$3,$4,$5)
        stmt_if_lp_exp_rp_stmt($$, $3, $5);
    }
    | IF LP Exp RP Stmt ELSE Stmt {
        FORM_SUBTREE_7($$,STMT,$1,$2,$3,$4,$5,$6,$7)
        stmt_if_lp_exp_rp_stmt_else_stmt($$, $3, $5, $7);
    }
    | WHILE LP Exp RP Stmt {
        FORM_SUBTREE_5($$,STMT,$1,$2,$3,$4,$5)
        stmt_while_lp_exp_rp_stmt($$, $3, $5);
    }
    | error SEMI {
        $$ = create_EP();
    }
    | error {
        $$ = create_EP();
    }
    ;

/* Local Definitions */
DefList: Def DefList {
        FORM_SUBTREE_2($$,DEFLIST,$1,$2)
        def_def_def($$, $1, $2);
        // AttrList* p = $$->attr.vardef_list;
        // while(p != NULL){
        //     Var_Def* temp = &p->attr.var_def;
        //     for(int i = 0; i < temp->var_num; i++){
        //         printf("%s: ", temp->id[i]);
        //         print_type(temp->type_id[i], 0, 1);
        //     }
        //     p = p->next;
        //     printf("=========\n");
        // }
    }
    | /* empty */ {
        $$ = create_EP();
    }
    ;
Def: Specifier DecList SEMI {
        FORM_SUBTREE_3($$,DEF,$1,$2,$3)
        def_spec_dec_semi($$, $1, $2);
        // Var_Def* temp = &$$->attr.vardef_list->attr.var_def;
        // for(int i = 0; i < temp->var_num; i++){
        //     printf("%s: ", temp->id[i]);
        //     print_type(temp->type_id[i], 0, 1);
        // }
    }
    | Specifier error SEMI {
        $$ = create_EP();
    }
    ;
DecList: Dec {
        FORM_SUBTREE_1($$,DECLIST,$1)
        dec_dec($$, $1);
    }
    | Dec COMMA DecList {
        FORM_SUBTREE_3($$,DECLIST,$1,$2,$3)
        dec_dec_comma_dec($$, $1, $3);
    }
    | error COMMA DecList {
        $$ = create_EP();
    }
    ;
Dec: VarDec {
        FORM_SUBTREE_1($$,DEC,$1)
        dec_var($$, $1);
    }
    | VarDec ASSIGNOP Exp {
        FORM_SUBTREE_3($$,DEC,$1,$2,$3)
        dec_var_assignop_exp($$, $1, $3);
    }
    ;

/* Expressions */
Exp: Exp ASSIGNOP Exp{
        FORM_SUBTREE_3($$,EXP,$1,$2,$3)
        exp_exp_assignop_exp($$, $1, $3);
    }
    | Exp AND Exp {
        FORM_SUBTREE_3($$,EXP,$1,$2,$3)
        exp_exp_and_exp($$, $1, $3);
    }
    | Exp OR Exp {
        FORM_SUBTREE_3($$,EXP,$1,$2,$3)
        exp_exp_or_exp($$, $1, $3);
    }
    | Exp RELOP Exp {
        FORM_SUBTREE_3($$,EXP,$1,$2,$3)
        exp_exp_relop_exp($$, $1, $2, $3);
    }
    | Exp PLUS Exp {
        FORM_SUBTREE_3($$,EXP,$1,$2,$3)
        exp_exp_plus_exp($$, $1, $3);
    }
    | Exp MINUS Exp {
        FORM_SUBTREE_3($$,EXP,$1,$2,$3)
        exp_exp_minus_exp($$, $1, $3);
    }
    | Exp STAR Exp {
        FORM_SUBTREE_3($$,EXP,$1,$2,$3)
        exp_exp_star_exp($$, $1, $3);
    }
    | Exp DIV Exp {
        FORM_SUBTREE_3($$,EXP,$1,$2,$3)
        exp_exp_div_exp($$, $1, $3);
    }
    | LP Exp RP {
        FORM_SUBTREE_3($$,EXP,$1,$2,$3)
        exp_lp_exp_rp($$, $2);
    }
    | LP error RP {
        $$ = create_EP();
    }
    | MINUS Exp {
        FORM_SUBTREE_2($$,EXP,$1,$2)
        exp_minus_exp($$, $2);
    }
    | NOT Exp {
        FORM_SUBTREE_2($$,EXP,$1,$2)
        exp_not_exp($$, $2);
    }
    | ID LP Args RP {
        FORM_SUBTREE_4($$,EXP,$1,$2,$3,$4)
        exp_id_lp_args_rp($$, $1, $3);
        // AttrList* p = $3->attr.para_list;
        // while(p != NULL){
        //     printf("%s ", p->attr.para.id);
        //     print_type(p->attr.para.type, 0, 1);
        //     p = p->next;
        // }
    }
    | ID LP RP  {
        FORM_SUBTREE_3($$,EXP,$1,$2,$3)
        exp_id_lp_rp($$, $1);
    }
    | ID LP error RP {
        $$ = create_EP();
    }
    | Exp LB Exp RB {
        FORM_SUBTREE_4($$,EXP,$1,$2,$3,$4)
        exp_exp_lb_exp_rb($$, $1, $3);
    }
    | Exp LB error RB {
        $$ = create_EP();
    }
    | Exp DOT ID {
        FORM_SUBTREE_3($$,EXP,$1,$2,$3)
        exp_exp_dot_id($$, $1, $3);
    }
    | ID {
        FORM_SUBTREE_1($$,EXP,$1)
        exp_id($$, $1);
    }
    | INT {
        FORM_SUBTREE_1($$,EXP,$1)
        exp_int($$, $1);
    }
    | FLOAT {
        FORM_SUBTREE_1($$,EXP,$1)
        exp_float($$, $1);
    }
    ;
Args: Exp COMMA Args {
        FORM_SUBTREE_3($$,ARGS,$1,$2,$3)
        arg_exp_comma_arg($$, $1, $3);
    }
    | Exp {
        FORM_SUBTREE_1($$,ARGS,$1)
        arg_exp($$, $1);
    }
    | error COMMA Args {
        $$ = create_EP();
    }
    ;
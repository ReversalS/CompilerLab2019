%{
    /* ... */
    #include "lex.yy.c"
    #include "parser_tree.h"

    int yyerror(const char*);
    int yylex(void);
%}
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
%left LP RP //parenthesis '(' ')'
%left LB RB //brackets '[' ']'
            //curly braces '{' '}'
%left DOT
// %right NEG
%right NOT
%left STAR DIV
%left PLUS MINUS
%left RELOP
%left AND
%left OR
%right ASSIGNOP
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

/* High-level Definitions */
Program: ExtDefList {
        // FORM_SUBTREE(1, PROGRAM);
        FORM_SUBTREE_1($$,PROGRAM,$1);
        global_root = $$;
    };
ExtDefList: ExtDef ExtDefList   { FORM_SUBTREE_2($$,EXTDEFLIST,$1,$2) }
    | /* empty */               {$$ = create_EP();}
    ;
ExtDef: Specifier ExtDecList SEMI   { FORM_SUBTREE_3($$,EXTDEF,$1,$2,$3); }
    | Specifier SEMI    { FORM_SUBTREE_2($$,EXTDEF,$1,$2); }
    | Specifier FunDec CompSt   { FORM_SUBTREE_3($$,EXTDEF,$1,$2,$3); }
    ;
ExtDecList: VarDec  { FORM_SUBTREE_1($$,EXTDECLIST,$1); }
    | VarDec COMMA ExtDecList   { FORM_SUBTREE_3($$,EXTDECLIST,$1,$2,$3); }
    ;

/* Specifiers */
Specifier: TYPE { FORM_SUBTREE_1($$,SPECIFIER,$1); }
    | StructSpecifier   { FORM_SUBTREE_1($$,SPECIFIER,$1); }
    ;
StructSpecifier: STRUCT OptTag LC DefList RC    { FORM_SUBTREE_5($$,STRUCTSPECIFIER,$1,$2,$3,$4,$5) }
    | STRUCT Tag    { FORM_SUBTREE_2($$,STRUCTSPECIFIER,$1,$2) }
    ;
OptTag: ID  { FORM_SUBTREE_1($$,OPTTAG,$1) }
    | /* empty */ {$$ = create_EP();}
    ;
Tag: ID { FORM_SUBTREE_1($$,TAG,$1) }
    ;

/* Declarators */
VarDec: ID  { FORM_SUBTREE_1($$,VARDEC,$1) }
    | VarDec LB INT RB  { FORM_SUBTREE_4($$,VARDEC,$1,$2,$3,$4) }
    ;
FunDec: ID LP VarList RP  { FORM_SUBTREE_4($$,FUNDEC,$1,$2,$3,$4) }
    | ID LP RP  { FORM_SUBTREE_3($$,FUNDEC,$1,$2,$3) }
    ;
VarList: ParamDec COMMA VarList  { FORM_SUBTREE_3($$,VARLIST,$1,$2,$3) }
    | ParamDec  { FORM_SUBTREE_1($$,VARLIST,$1) }
    ;
ParamDec: Specifier VarDec  { FORM_SUBTREE_2($$,PARAMDEC,$1,$2) }
    ;

/* Statements */
CompSt: LC DefList StmtList RC  { FORM_SUBTREE_4($$,COMPST,$1,$2,$3,$4) }
    ;
StmtList: Stmt StmtList  { FORM_SUBTREE_2($$,STMTLIST,$1,$2) }
    | /* empty */        {$$ = create_EP();}
    ;
Stmt: Exp SEMI  { FORM_SUBTREE_2($$,STMT,$1,$2) }
    | CompSt  { FORM_SUBTREE_1($$,STMT,$1) }
    | RETURN Exp SEMI   { FORM_SUBTREE_3($$,STMT,$1,$2,$3) }
    // | IF LP Exp RP Stmt
    // | IF LP Exp RP Stmt ELSE Stmt
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE   { FORM_SUBTREE_5($$,STMT,$1,$2,$3,$4,$5) }
    | IF LP Exp RP Stmt ELSE Stmt   { FORM_SUBTREE_7($$,STMT,$1,$2,$3,$4,$5,$6,$7) }
    | WHILE LP Exp RP Stmt  { FORM_SUBTREE_5($$,STMT,$1,$2,$3,$4,$5) }
    ;

/* Local Definitions */
DefList: Def DefList    { FORM_SUBTREE_2($$,DEFLIST,$1,$2) }
    | /* empty */       {$$ = create_EP();}
    ;
Def: Specifier DecList SEMI { FORM_SUBTREE_3($$,DEF,$1,$2,$3) }
    ;
DecList: Dec    { FORM_SUBTREE_1($$,DECLIST,$1) }
    | Dec COMMA DecList { FORM_SUBTREE_3($$,DECLIST,$1,$2,$3) }
    ;
Dec: VarDec { FORM_SUBTREE_1($$,DEC,$1) }
    | VarDec ASSIGNOP Exp   { FORM_SUBTREE_3($$,DEC,$1,$2,$3) }
    ;

/* Expressions */
Exp: Exp ASSIGNOP Exp   { FORM_SUBTREE_3($$,EXP,$1,$2,$3) }
    | Exp AND Exp   { FORM_SUBTREE_3($$,EXP,$1,$2,$3) }
    | Exp OR Exp    { FORM_SUBTREE_3($$,EXP,$1,$2,$3) }
    | Exp RELOP Exp { FORM_SUBTREE_3($$,EXP,$1,$2,$3) }
    | Exp PLUS Exp  { FORM_SUBTREE_3($$,EXP,$1,$2,$3) }
    | Exp MINUS Exp { FORM_SUBTREE_3($$,EXP,$1,$2,$3) }
    | Exp STAR Exp  { FORM_SUBTREE_3($$,EXP,$1,$2,$3) }
    | Exp DIV Exp   { FORM_SUBTREE_3($$,EXP,$1,$2,$3) }
    | LP Exp RP { FORM_SUBTREE_3($$,EXP,$1,$2,$3) }
    | MINUS Exp { FORM_SUBTREE_2($$,EXP,$1,$2) }
    | NOT Exp   { FORM_SUBTREE_2($$,EXP,$1,$2) }
    | ID LP Args RP { FORM_SUBTREE_4($$,EXP,$1,$2,$3,$4) }
    | ID LP RP  { FORM_SUBTREE_3($$,EXP,$1,$2,$3) }
    | Exp LB Exp RB { FORM_SUBTREE_4($$,EXP,$1,$2,$3,$4) }
    | Exp DOT ID    { FORM_SUBTREE_3($$,EXP,$1,$2,$3) }
    | ID    { FORM_SUBTREE_1($$,EXP,$1) }
    | INT   { FORM_SUBTREE_1($$,EXP,$1) }
    | FLOAT { FORM_SUBTREE_1($$,EXP,$1) }
    ;
Args: Exp COMMA Args    { FORM_SUBTREE_3($$,ARGS,$1,$2,$3) }
    | Exp   { FORM_SUBTREE_1($$,ARGS,$1) }
    ;
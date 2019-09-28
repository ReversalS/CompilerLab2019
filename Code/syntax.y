%{
    /* ... */
    #include "lex.yy.c"
    #include "parser_tree.h"

    int yyerror(const char*);
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
Program: ExtDefList;
ExtDefList: ExtDef ExtDefList
    | /* empty */
    ;
ExtDef: Specifier ExtDecList SEMI
    | Specifier SEMI
    | Specifier FunDec CompSt
    ;
ExtDecList: VarDec
    | VarDec COMMA ExtDecList
    ;

/* Specifiers */
Specifier: TYPE
    | StructSpecifier
    ;
StructSpecifier: STRUCT OptTag LC DefList RC
    | STRUCT Tag
    ;
OptTag: ID
    | /* empty */
    ;
Tag: ID
    ;

/* Declarators */
VarDec: ID
    | VarDec LB INT RB
    ;
FunDec: ID LP VarList RP
    | ID LP RP
    ;
VarList: ParamDec COMMA VarList
    | ParamDec
    ;
ParamDec: Specifier VarDec
    ;

/* Statements */
CompSt: LC DefList StmtList RC
    ;
StmtList: Stmt StmtList
    | /* empty */
    ;
Stmt: Exp SEMI
    | CompSt
    | RETURN Exp SEMI
    // | IF LP Exp RP Stmt
    // | IF LP Exp RP Stmt ELSE Stmt
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt
    ;

/* Local Definitions */
DefList: Def DefList
    | /* empty */
    ;
Def: Specifier DecList SEMI
    ;
DecList: Dec
    | Dec COMMA DecList
    ;
Dec: VarDec
    | VarDec ASSIGNOP Exp
    ;

/* Expressions */
Exp: Exp ASSIGNOP Exp
    | Exp AND Exp
    | Exp OR Exp
    | Exp RELOP Exp
    | Exp PLUS Exp
    | Exp MINUS Exp
    | Exp STAR Exp
    | Exp DIV Exp
    | LP Exp RP
    | MINUS Exp
    | NOT Exp
    | ID LP Args RP
    | ID LP RP
    | Exp LB Exp RB
    | Exp DOT ID
    | ID
    | INT
    | FLOAT
    ;
Args: Exp COMMA Args
    | Exp
    ;
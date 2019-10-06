%option yylineno
%{
    #include<stdio.h>
    #include<string.h>
    #include<stdlib.h>
    #define YYSTYPE struct Node*
    #include"syntax.tab.h"
    #include"parser_tree.h"

    extern int lexical_error;

    // lexeme location
    int yycolumn = 1;
    int first_column = 1;
    int last_column = 1;

    #define YY_USER_ACTION first_column = yycolumn; last_column = yycolumn + yyleng - 1; yycolumn += yyleng;

    union NODE_VALUE nv;
    void output(int lineno, char* type, char* value);

    void unknown_char(char c, int lineno);
    void invalid_id(char* str, int lineno);
%}
%x line_comment
%x block_comment

digit [0-9]
letter [a-zA-Z]
ws [ \t\r]
newline \n
underline _

INT (0)|([1-9]{digit}*)
BADINT {INT}({letter}|{underline})+
ID ({underline}|{letter})({underline}|{letter}|{digit})*
FLOAT {digit}+\.{digit}+
BADFLOAT {FLOAT}({letter}|{underline})+
SEMI ;
COMMA ,
ASSIGNOP =
RELOP >|<|>=|<=|==|!=
PLUS "+"
MINUS "-"
STAR "*"
DIV "/"
AND "&&"
OR "||"
DOT "."
NOT "!"
TYPE (int|float)
LP "("
RP ")"
LB "["
RB "]"
LC "{"
RC "}"
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
WS {ws}+
NEWLINE {newline}

%%

"//"                        {BEGIN(line_comment);}

<line_comment>.             {}

<line_comment>\n            {BEGIN(INITIAL);}

"/*"                        {BEGIN(block_comment);}

<block_comment>[^*]*        {}

<block_comment>"*"+[^*/]*   {}

<block_comment>"*"+"/"      {BEGIN(INITIAL);}

{TYPE} {
    if(strcmp(yytext, "int") == 0){
        nv.type = INT_T;
        yylval = create_T(TYPE_E, nv, yylineno, first_column);
        return TYPE;
    } else{
        nv.type = FLOAT_T;
        yylval = create_T(TYPE_E, nv, yylineno, first_column);
        return TYPE;
    }
}

{STRUCT} {
    nv.keyword = STRUCT_E;
    yylval = create_T(KEYWORD_E, nv, yylineno, first_column);
    return STRUCT;
}

{RETURN} {
    nv.keyword = RETURN_E;
    yylval = create_T(KEYWORD_E, nv, yylineno, first_column);
    return RETURN;
}

{IF} {
    nv.keyword = IF_E;
    yylval = create_T(KEYWORD_E, nv, yylineno, first_column);
    return IF;
}

{ELSE} {
    nv.keyword = ELSE_E;
    yylval = create_T(KEYWORD_E, nv, yylineno, first_column);
    return ELSE;
}

{WHILE} {
    nv.keyword = WHILE_E;
    yylval = create_T(KEYWORD_E, nv, yylineno, first_column);
    return WHILE;
}

{INT} {
    nv.ival = atoi(yytext);
    yylval = create_T(INT_E, nv, yylineno, first_column);
    return INT;
}
    /*
{BADINT}        {invalid_id(yytext, yylineno); lexical_error = 1;}
    */
{FLOAT} {
    nv.fval = atof(yytext);
    yylval = create_T(FLOAT_E, nv, yylineno, first_column);
    return FLOAT;
}
    /*
{BADFLOAT}      {invalid_id(yytext, yylineno); lexical_error = 1;}
    */
{ID} {
    yylval = create_ID(yytext, yylineno, first_column);
    return ID;
}

{SEMI} {
    nv.other = SEMI_E;
    yylval = create_T(OTHER_E, nv, yylineno, first_column);
    return SEMI;
}

{COMMA} {
    nv.other = COMMA_E;
    yylval = create_T(OTHER_E, nv, yylineno, first_column);
    return COMMA;
}

{ASSIGNOP} {
    nv.other = ASSIGNOP_E;
    yylval = create_T(OTHER_E, nv, yylineno, first_column);
    return ASSIGNOP;
}

{RELOP} {
    if(strcmp(yytext, ">") == 0){
        nv.relop = GT_E;
        yylval = create_T(RELOP_E, nv, yylineno, first_column);
        return RELOP;
    } else if (strcmp(yytext, "<") == 0){
        nv.relop = LT_E;
        yylval = create_T(RELOP_E, nv, yylineno, first_column);
        return RELOP;
    } else if (strcmp(yytext, ">=") == 0){
        nv.relop = GE_E;
        yylval = create_T(RELOP_E, nv, yylineno, first_column);
        return RELOP;
    } else if (strcmp(yytext, "<=") == 0){
        nv.relop = LE_E;
        yylval = create_T(RELOP_E, nv, yylineno, first_column);
        return RELOP;
    } else if (strcmp(yytext, "==") == 0){
        nv.relop = EQ_E;
        yylval = create_T(RELOP_E, nv, yylineno, first_column);
        return RELOP;
    } else if (strcmp(yytext, "!=") == 0){
        nv.relop = NE_E;
        yylval = create_T(RELOP_E, nv, yylineno, first_column);
        return RELOP;
    } else{
        // error
    }
}

{PLUS} {
    nv.other = PLUS_E;
    yylval = create_T(OTHER_E, nv, yylineno, first_column);
    return PLUS;
}

{STAR} {
    nv.other = STAR_E;
    yylval = create_T(OTHER_E, nv, yylineno, first_column);
    return STAR;
}

{DIV} {
    nv.other = DIV_E;
    yylval = create_T(OTHER_E, nv, yylineno, first_column);
    return DIV;
}

{AND} {
    nv.other = AND_E;
    yylval = create_T(OTHER_E, nv, yylineno, first_column);
    return AND;
}

{OR} {
    nv.other = OR_E;
    yylval = create_T(OTHER_E, nv, yylineno, first_column);
    return OR;
}

{DOT} {
    nv.other = DOT_E;
    yylval = create_T(OTHER_E, nv, yylineno, first_column);
    return DOT;
}

{NOT} {
    nv.other = NOT_E;
    yylval = create_T(OTHER_E, nv, yylineno, first_column);
    return NOT;
}

{LP} {
    nv.other = LP_E;
    yylval = create_T(OTHER_E, nv, yylineno, first_column);
    return LP;
}

{RP} {
    nv.other = RP_E;
    yylval = create_T(OTHER_E, nv, yylineno, first_column);
    return RP;
}

{LB} {
    nv.other = LB_E;
    yylval = create_T(OTHER_E, nv, yylineno, first_column);
    return LB;
}

{RB} {
    nv.other = RB_E;
    yylval = create_T(OTHER_E, nv, yylineno, first_column);
    return RB;
}

{LC} {
    nv.other = LC_E;
    yylval = create_T(OTHER_E, nv, yylineno, first_column);
    return LC;
}

{RC} {
    nv.other = RC_E;
    yylval = create_T(OTHER_E, nv, yylineno, first_column);
    return RC;
}

{WS}            {}

{NEWLINE}       {yycolumn = 1;}

.               {unknown_char(*yytext, yylineno); lexical_error = 1;}

%%

void output(int lineno, char* type, char* value){
    printf("%d: Type: %s\tValue: %s\n",lineno ,type, value);
}

void unknown_char(char c, int lineno){
    printf("Error type A at Line %d: Mysterious character \"%c\"\n", lineno, c);
}

void invalid_id(char* str, int lineno){
    printf("Error type A at Line %d: invalid identifier \"%s\"\n", lineno, str);
}
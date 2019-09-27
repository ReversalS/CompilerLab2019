%option yylineno
%{
    #include<stdio.h>
    #include<string.h>

    // lexeme location
    int yycolumn = 1;
    #define YY_USER_ACTION
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;


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

{TYPE}          {output(yylineno, "TYPE", yytext);}

{STRUCT}        {output(yylineno, "STRUCT", yytext);}

{RETURN}        {output(yylineno, "RETURN", yytext);}

{IF}            {output(yylineno, "IF", yytext);}

{ELSE}          {output(yylineno, "ELSE", yytext);}

{WHILE}         {output(yylineno, "WHILE", yytext);}

{INT}           {output(yylineno, "INT", yytext);}

{BADINT}        {invalid_id(yytext, yylineno);}

{FLOAT}         {output(yylineno, "FLOAT", yytext);}

{BADFLOAT}      {invalid_id(yytext, yylineno);}

{ID}            {output(yylineno, "ID", yytext);}

{SEMI}          {output(yylineno, "SEMI", yytext);}

{COMMA}         {output(yylineno, "COMMA", yytext);}

{ASSIGNOP}      {output(yylineno, "ASSIGNOP", yytext);}

{RELOP}         {output(yylineno, "RELOP", yytext);}

{PLUS}          {output(yylineno, "PLUS", yytext);}

{STAR}          {output(yylineno, "STAR", yytext);}

{DIV}           {output(yylineno, "DIV", yytext);}

{AND}           {output(yylineno, "AND", yytext);}

{OR}            {output(yylineno, "OR", yytext);}

{DOT}           {output(yylineno, "DOT", yytext);}

{NOT}           {output(yylineno, "NOT", yytext);}

{LP}            {output(yylineno, "LP", yytext);}

{RP}            {output(yylineno, "RP", yytext);}

{LB}            {output(yylineno, "LB", yytext);}

{RB}            {output(yylineno, "RB", yytext);}

{LC}            {output(yylineno, "LC", yytext);}

{RC}            {output(yylineno, "RC", yytext);}

{WS}            {}

{NEWLINE}       {yycolumn = 1;}

.               {unknown_char(*yytext, yylineno);}

%%

void output(int lineno, char* type, char* value){
    printf("%d: Type: %s\tValue: %s\n",lineno ,type, value);
}

void unknown_char(char c, int lineno){
    printf("Error type A at line %d: Mysterious character \"%c\"\n", lineno, c);
}

void invalid_id(char* str, int lineno){
    printf("Error type A at line %d: invalid identifier \"%s\"\n", lineno, str);
}
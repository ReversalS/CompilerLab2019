#ifndef IR_H
#define IR_H

/********** include and macro **********/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OPERAND_LEN 32
#define CODE_LEN 128

/********** typedef **********/
typedef struct InterCode InterCode;
typedef struct Operand Operand;
typedef enum AssignType AssignType;
typedef enum BinType BinType;
typedef enum RelopType RelopType;
typedef enum OperandType OperandType;

/********** structure definition **********/
enum AssignType { NORMAL,
    GET_ADDR,
    DEREF_LEFT,
    DEREF_RIGHT };

enum BinType { PLUS,
    MINUS,
    STAR,
    DIV };

enum RelopType { LT,
    LE,
    EQ,
    NE,
    GT,
    GE };

enum OperandType { VAR,
    TEMP,
    CONST};

struct InterCode {
    enum { LABEL_DEF_ST,
        FUNC_DEF_ST,
        ASSIGN_ST,
        BIN_ST,
        GOTO_ST,
        IF_ST,
        RETURN_ST,
        DEC_ST,
        ARG_ST,
        CALL_FUNC_ST,
        PARAM_ST,
        READ_ST,
        WRITE_ST } kind;
    union {
        int label_id;
        char* func_name;
        struct {
            Operand* left;
            Operand* right;
            AssignType kind;
        } assign;
        struct {
            Operand* result;
            Operand* op1;
            Operand* op2;
            BinType kind;
        } bin;
        int goto_id;
        struct {
            Operand* op1;
            Operand* op2;
            RelopType relop;
            int goto_id;
        } if_statement;
        Operand* return_val;
        struct {
            Operand* op;
            int size;
        } dec_statement;
        Operand* arg;
        struct {
            Operand* return_val;
            char* func_name;
        } call_func_statement;
        Operand* param;
        Operand* read;
        Operand* write;
    } code;
};

struct Operand {
    OperandType kind;
    union {
        char* var_name;
        int temp_id;
        int ival;
    } u;
};

/********** global variable **********/
int label_id;
int temp_id;

/********** function definition **********/
Operand* create_op(OperandType type, void* val);
char* op_to_string(Operand* op);
InterCode* create_ic();
char* ic_to_string(InterCode* ic);
InterCode* create_label();
InterCode* create_func_def(char* name);
InterCode* create_assign(AssignType type, Operand* left, Operand* right);
InterCode* create_bin(BinType type, Operand* result, Operand* op1, Operand* op2);
InterCode* create_goto(int label);
InterCode* create_if(RelopType type, Operand* op1, Operand* op2, int label);
InterCode* create_return(Operand* return_val);
InterCode* create_dec(Operand* op, int size);
InterCode* create_arg(Operand* arg);
InterCode* create_call_func(Operand* return_val, char* func_name);
InterCode* create_param(Operand* param);
InterCode* create_read(Operand* read);
InterCode* create_write(Operand* write);
void deconstruct_op(Operand* op);
void deconstruct_ic(InterCode* ic);
#endif
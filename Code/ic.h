#ifndef IC_H
#define IC_H

/********** include and macro **********/
#include "attrlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OPERAND_LEN 32
#define CODE_LEN 128
#define MAX_TEMP_NUM 2048
#define need_deref(op) (op->is_addr && op->deref)

/********** typedef **********/
typedef struct InterCode InterCode;
typedef struct Operand Operand;
typedef enum AssignType AssignType;
typedef enum BinType BinType;
typedef enum RelopType RelopType;
typedef enum OperandType OperandType;

/********** structure definition **********/
enum AssignType { NORMAL,
    GET_ADDR };

enum BinType { PLUS_IC,
    MINUS_IC,
    STAR_IC,
    DIV_IC };

enum RelopType { LT_IC,
    LE_IC,
    EQ_IC,
    NE_IC,
    GT_IC,
    GE_IC };

enum OperandType { VAR,
    TEMP,
    CONST };

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
    Operand* op1;
    Operand* op2;
    Operand* result;
    struct {
        int op1;
        int op2;
        int result;
    } is_addr;
    union {
        int label_id;
        char* func_name;
        int dec_size;
        AssignType assign_kind;
        BinType bin_kind;
        RelopType relop;
        struct {
            int label_id;
            RelopType relop;
        } if_stmt;
    } code;
};

struct Operand {
    OperandType kind;
    union {
        char* var_name;
        int temp_id;
        int ival;
    } u;
    int is_addr;
};

/********** global variable **********/
int label_id;
int temp_id;

/********** function definition **********/
Operand* create_op(OperandType type, char* val, int is_addr);
Operand* create_const(int val);

InterCode* create_ic();
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

char* op_to_string(Operand* op, int is_addr);
char* ic_to_string(InterCode* ic);

int bin_result(int a, int b, BinType type);
int relop_result(int a, int b, RelopType type);
#endif
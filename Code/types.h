#ifndef TYPES_H
#define TYPES_H

#define TYPE_SET_SIZE 1024

typedef struct Type Type;
typedef struct VarList VarList;
typedef enum BOOL BOOL;

int type_num;

enum BASIC_TYPE{
    INT_BASIC,
    FLOAT_BASIC,
};

enum BOOL {
    FALSE = 0, TRUE = 1
};

struct Type{
    enum {BASIC, ARRAY, STRUCTURE, FUNCTION} kind;
    union {
        enum BASIC_TYPE basic;
        struct {
            int elem;
            int size;
        } array;
        struct {
            char* name;
            VarList* fields;
        } structure;
        struct {
            char* name;
            int return_type;
            VarList* parameters;
        } function;
    } u;
};

struct VarList{
    char* name;
    int type;
    VarList* next;
};

Type type_set[TYPE_SET_SIZE];

void init_type_set();
void deconstruct_varlist(VarList* p);
void deconstruct_set();

// for developer
BOOL EQ(Type* t1, Type* t2);

// for user
BOOL eq(int t1, int t2);

int exists(Type* t);

int construct_basic(enum BASIC_TYPE type);
int construct_array(int base, int size);
int construct_struct(char* name);
int construct_function(char* name, int return_type);
int add_member(int base, char* name, int type);
// int add_var(VarList* p, char* name, int type);

void print_type(int type, int level, int newline);
void print_array(int type, int level);
void print_var(VarList* p, int level);

#endif
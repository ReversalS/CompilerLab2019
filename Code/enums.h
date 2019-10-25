#ifndef ENUMS_H
#define ENUMS_H

/********** typedef **********/
typedef struct Location Location;
// typedef enum SYMBOL_TYPE SYMBOL_TYPE;
// typedef enum NT_TYPE NT_TYPE;
// typedef enum T_TYPE T_TYPE;
// typedef enum RELOP RELOP;
// typedef enum TYPE TYPE;
// typedef enum OTHER OTHER;
// typedef union NODE_TYPE NODE_TYPE;
// typedef union NODE_VALUE NODE_VALUE;


/********** structure definition **********/
// HIGH LEVEL ENUM
// 标记节点是非终结符（NT）或终结符（T）或空串（\epsilon）
enum SYMBOL_TYPE {
    NT,
    T,
    EPSILON
};

// LOW LEVEL ENUM
// 非终结符(NonTerminal)类型（根据文法）
enum NT_TYPE {
    PROGRAM,
    EXTDEFLIST,
    EXTDEF,
    SPECIFIER,
    FUNDEC,
    COMPST,
    VARDEC,
    EXTDECLIST,
    STRUCTSPECIFIER,
    OPTTAG,
    DEFLIST,
    TAG,
    VARLIST,
    PARAMDEC,
    STMTLIST,
    STMT,
    EXP,
    DEF,
    DECLIST,
    DEC,
    ARGS
};

// 终结符(Terminal)类型，包括 int，float，id，类型标识符，关系符，关键字，其他
enum T_TYPE {
    INT_E,
    FLOAT_E,
    ID_E,
    TYPE_E,
    RELOP_E,
    KEYWORD_E,
    OTHER_E
};

enum RELOP {
    GT_E,
    LT_E,
    GE_E,
    LE_E,
    EQ_E,
    NE_E
};

enum TYPE {
    INT_T,
    FLOAT_T
};

enum OTHER {
    SEMI_E,
    COMMA_E,
    ASSIGNOP_E,
    PLUS_E,
    MINUS_E,
    STAR_E,
    DIV_E,
    AND_E,
    OR_E,
    NOT_E,
    DOT_E,
    LP_E,
    RP_E,
    LB_E,
    RB_E,
    LC_E,
    RC_E,
};

enum KEYWORD {
    STRUCT_E,
    RETURN_E,
    IF_E,
    ELSE_E,
    WHILE_E
};

// union 用于存储节点具体的类型，其取哪个分量根据最高级的 SYMBOL_TYPE 而定，分量中存储具体的枚举变量（NT or T），ep 分量用于空串，默认为 -1
union NODE_TYPE {
    enum NT_TYPE nt_type;
    enum T_TYPE t_type;
    int ep;
};

// union 用于存储节点具体的值，其取哪个分量根据 NODE_TYPE 而定。当节点为终结符时这个变量才有意义
union NODE_VALUE {
    enum RELOP relop; // 当节点为关系符时的值
    enum TYPE type; // 节点为类型声明时的值
    enum OTHER other; // 节点为其他终结符时的值
    enum KEYWORD keyword; // 节点为关键字时的值
    int ival; // 节点为 int 型常量时常量字面的值
    float fval; // 节点为 float 型常量时常量字面的值
    char* id;
    int nt; // 当节点为非终结符时的值，默认 -1
    int ep; // 当节点为空串时的值，默认为 -1
};

// 用于存储节点在源代码中的开始位置
struct Location {
    int line;
    int column;
};

#endif
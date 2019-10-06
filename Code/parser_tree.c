#include "parser_tree.h"

int lexical_error = 0;
struct Node* global_root = NULL;

char* nt_type_dict[] = {
    "Program",
    "ExtDefList",
    "ExtDef",
    "Specifier",
    "FunDec",
    "CompSt",
    "VarDec",
    "ExtDecList",
    "StructSpecifier",
    "OptTag",
    "DefList",
    "Tag",
    "VarList",
    "ParamDec",
    "StmtList",
    "Stmt",
    "Exp",
    "Def",
    "DecList",
    "Dec",
    "Args"
};

char* relop_dict[] = {
    "GT",
    "LT",
    "GE",
    "LE",
    "EQ",
    "NE"
};

char* type_dict[] = {
    "int",
    "float"
};

char* other_dict[] = {
    "SEMI",
    "COMMA",
    "ASSIGNOP",
    "PLUS",
    "MINUS",
    "STAR",
    "DIV",
    "AND",
    "OR",
    "NOT",
    "DOT",
    "LP",
    "RP",
    "LB",
    "RB",
    "LC",
    "RC"
};

char* keyword_dict[] = {
    "STRUCT",
    "RETURN",
    "IF",
    "ELSE",
    "WHILE"
};

// 创建节点，不会被用户显式调用
struct Node* create_node(enum SYMBOL_TYPE st, union NODE_TYPE nt, union NODE_VALUE nv, struct location loc)
{
    struct Node* p = (struct Node*)malloc(sizeof(struct Node));
    if (p == NULL) {
        perror("malloc failed.\n");
        abort();
    }
    p->symbol_type = st;
    p->node_type = nt;
    p->node_value = nv;
    p->loc.line = loc.line;
    p->loc.column = loc.column;
    p->child_num = -1;
    p->children = NULL;
    return p;
}

// 创建空串
struct Node* create_EP()
{
    enum SYMBOL_TYPE st;
    union NODE_TYPE nt;
    union NODE_VALUE nv;
    struct location loc;
    st = EPSILON;
    nt.ep = -1;
    nv.ep = -1;
    loc.line = -1;
    loc.column = -1;
    return create_node(st, nt, nv, loc);
}

// 创建非终结符
struct Node* create_NT(enum NT_TYPE nt_type)
{
    enum SYMBOL_TYPE st;
    union NODE_TYPE nt;
    union NODE_VALUE nv;
    struct location loc;
    st = NT;
    nt.nt_type = nt_type;
    nv.nt = -1;
    loc.line = -1;
    loc.column = -1;
    return create_node(st, nt, nv, loc);
}

// 创建终结符（非 id）
struct Node* create_T(enum T_TYPE t_type, union NODE_VALUE nv, int line, int column)
{
    enum SYMBOL_TYPE st;
    union NODE_TYPE nt;
    struct location loc;
    st = T;
    nt.t_type = t_type;
    loc.line = line;
    loc.column = column;
    return create_node(st, nt, nv, loc);
}

// 创建 id
struct Node* create_ID(char* str, int line, int column)
{
    enum SYMBOL_TYPE st;
    union NODE_TYPE nt;
    union NODE_VALUE nv;
    struct location loc;
    st = T;
    nt.t_type = ID_E;
    nv.index = add_entry(str, line, column);
    loc.line = line;
    loc.column = column;
    return create_node(st, nt, nv, loc);
}

// 为节点的子数组分配空间
// TODO: 添加再分配功能？Free 原有 malloc 新空间
void allo_child(int num, struct Node* p)
{
    p->child_num = num;
    p->children = (struct Node**)malloc(num * sizeof(struct Node*));
    if (p->children == NULL) {
        perror("malloc failed.\n");
        abort();
    }
    memset(p->children, 0, p->child_num * sizeof(struct Node*));
}

// parent的行号为第一个子节点的行号
void set_attr(struct Node* parent, struct Node* child)
{
    parent->loc.line = child->loc.line;
}

// 析构语法树，归还内存
void deconstruct(struct Node* p)
{
    if (p->child_num == -1) {
        // no children
        free(p);
        return;
    } else {
        for (int i = 0; i < p->child_num; i++) {
            deconstruct(p->children[i]);
        }
        free(p->children);
        free(p);
        return;
    }
}

// 递归打印
void print_tree(struct Node* p, int level)
{
    if(p->symbol_type == EPSILON){
        return ;
    }
    char str[MAX_ID_LENGTH];
    memset(str, 0, MAX_ID_LENGTH * sizeof(char));
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    switch (p->symbol_type) {
    case NT:
        printf("%s (%d)\n", nt_type_dict[(int)p->node_type.nt_type], p->loc.line);
        break;
    case T:
        switch (p->node_type.t_type) {
        case INT_E:
            printf("INT: %d\n", p->node_value.ival);
            break;
        case FLOAT_E:
            printf("FLOAT: %f\n", p->node_value.fval);
            break;
        case ID_E:
            get_id(p->node_value.index, str);
            printf("ID: %s\n", str);
            memset(str, 0, MAX_ID_LENGTH * sizeof(char));
            break;
        case TYPE_E:
            printf("TYPE: %s\n", type_dict[(int)p->node_value.type]);
            break;
        case RELOP_E:
            printf("%s\n", relop_dict[(int)p->node_value.relop]);
            break;
        case KEYWORD_E:
            printf("%s\n", keyword_dict[(int)p->node_value.keyword]);
            break;
        case OTHER_E:
            printf("%s\n", other_dict[(int)p->node_value.other]);
            break;
        }
        break;
    case EPSILON:;
        break;
    default:
        break;
    }
    if (p->child_num < 0) {
        return;
    } else {
        for (int i = 0; i < p->child_num; i++) {
            print_tree(p->children[i], level + 1);
        }
    }
}
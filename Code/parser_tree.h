#ifndef PARSER_TREE_H
#define PARSER_TREE_H

/********** include and macro **********/
#include "attrlist.h"
#include "enums.h"
#include "ic_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********** typedef **********/
typedef struct Node Node;

/********** structure definition **********/
struct Node {

    // lexical
    enum SYMBOL_TYPE symbol_type; // 终结符/非终结符
    union NODE_TYPE node_type; // 具体的类型
    union NODE_VALUE node_value; // 词法单元的值
    struct Location loc; //出现位置
    int child_num; // 子节点数量
    struct Node** children; // 子节点数组
    union {
        enum { EXP_ASSIGN,
            EXP_LOGIC,
            EXP_ARITH,
            EXP_LP,
            EXP_MINUS,
            EXP_FUNC,
            EXP_ARRAY,
            EXP_STRUCT,
            EXP_BASIC } exp;
    } body;
    Code code;



    // semantic attrs
    struct {
        char* id;
        int type_id;
        union {
            int ival;
            float fval;
        } val;
        int is_left;
        AttrList* opt_array;
        AttrList* var_list;
        AttrList* vardef_list;
        AttrList* para_list;
    } attr;
};

/********** global variable **********/
Node* global_root;
int lexical_error;

/********** function definition **********/
// 为节点的子数组分配空间
void allo_child(int num, Node* p);
// parent的行号为第一个子节点的行号
void set_attr(Node* parent, Node* child);
// 析构语法树，归还内存
void deconstruct(Node* p);
// 递归打印
void print_tree(Node* p, int level);

// 创建节点，不会被用户显式调用
Node* create_node(enum SYMBOL_TYPE st, union NODE_TYPE nt, union NODE_VALUE nv, Location loc);
// 创建空串
Node* create_EP();
// 创建非终结符
Node* create_NT(enum NT_TYPE nt_type);
// 创建终结符（非 id）
Node* create_T(enum T_TYPE t_type, union NODE_VALUE nv, int line, int column);
// 创建 id
Node* create_ID(char* str, int line, int column);

/* macros that help to efficiently build trees */

#define FORM_SUBTREE_0(root, root_type) \
    root = create_NT(root_type);        \
    allo_child(0, root);

#define FORM_SUBTREE_1(root, root_type, child0) \
    root = create_NT(root_type);                \
    allo_child(1, root);                        \
    root->children[0] = child0;                 \
    set_attr(root, child0);

#define FORM_SUBTREE_2(root, root_type, child0, child1) \
    root = create_NT(root_type);                        \
    allo_child(2, root);                                \
    root->children[0] = child0;                         \
    set_attr(root, child0);                             \
    root->children[1] = child1;

#define FORM_SUBTREE_3(root, root_type, child0, child1, child2) \
    root = create_NT(root_type);                                \
    allo_child(3, root);                                        \
    root->children[0] = child0;                                 \
    set_attr(root, child0);                                     \
    root->children[1] = child1;                                 \
    root->children[2] = child2;

#define FORM_SUBTREE_4(root, root_type, child0, child1, child2, child3) \
    root = create_NT(root_type);                                        \
    allo_child(4, root);                                                \
    root->children[0] = child0;                                         \
    set_attr(root, child0);                                             \
    root->children[1] = child1;                                         \
    root->children[2] = child2;                                         \
    root->children[3] = child3;

#define FORM_SUBTREE_5(root, root_type, child0, child1, child2, child3, child4) \
    root = create_NT(root_type);                                                \
    allo_child(5, root);                                                        \
    root->children[0] = child0;                                                 \
    set_attr(root, child0);                                                     \
    root->children[1] = child1;                                                 \
    root->children[2] = child2;                                                 \
    root->children[3] = child3;                                                 \
    root->children[4] = child4;

#define FORM_SUBTREE_6(root, root_type, child0, child1, child2, child3, child4, child5) \
    root = create_NT(root_type);                                                        \
    allo_child(6, root);                                                                \
    root->children[0] = child0;                                                         \
    set_attr(root, child0);                                                             \
    root->children[1] = child1;                                                         \
    root->children[2] = child2;                                                         \
    root->children[3] = child3;                                                         \
    root->children[4] = child4;                                                         \
    root->children[5] = child5;

#define FORM_SUBTREE_7(root, root_type, child0, child1, child2, child3, child4, child5, child6) \
    root = create_NT(root_type);                                                                \
    allo_child(7, root);                                                                        \
    root->children[0] = child0;                                                                 \
    set_attr(root, child0);                                                                     \
    root->children[1] = child1;                                                                 \
    root->children[2] = child2;                                                                 \
    root->children[3] = child3;                                                                 \
    root->children[4] = child4;                                                                 \
    root->children[5] = child5;                                                                 \
    root->children[6] = child6;

#endif // PARSER_TREE_H

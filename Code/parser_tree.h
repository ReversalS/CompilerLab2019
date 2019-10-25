#ifndef PARSER_TREE_H
#define PARSER_TREE_H

#include "enums.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node;

struct AttrList;
typedef struct AttrList AttrList;
void deconstruct_attrlist(AttrList* p);

struct Node {
    // lexical
    enum SYMBOL_TYPE symbol_type; // 终结符/非终结符
    union NODE_TYPE node_type; // 具体的类型
    union NODE_VALUE node_value; // 词法单元的值
    struct Location loc; //出现位置
    int child_num; // 子节点数量
    struct Node** children; // 子节点数组

    // semantic attrs
    struct {
        char* id;
		int type_id;
		AttrList* opt_array;
		AttrList* var_list;
    } attr;
};

// a global root for parser tree

Node* global_root;
int lexical_error;
extern int nest_depth;

// bison?
// 为节点的子数组分配空间
// TODO: 添加再分配功能？Free 原有 malloc 新空间
void allo_child(int num, Node* p);
// parent的行号为第一个子节点的行号
void set_attr(Node* parent, Node* child);
// 析构语法树，归还内存
void deconstruct(Node* p);
// 递归打印
void print_tree(Node* p, int level);

/*
example:

EXP
    EXP
        ID
    PLUS
    EXP
        ID


    // EXP -> ID
    // FLEX: conNodes and return pointers p5, p4
    Node* p4 = create_ID("i", 4, 9);
    Node* p5 = create_NT(EXP);

    // BISON: build the parser tree Bottom up
    allo_child(1, p5);
    p5->children[0] = p4;
    set_attr(p5, p4);

    // PLUS
    nv.other = PLUS;
    Node* p6 = create_T(OTHER, nv, 4, 11);

    // EXP -> INT
    nv.ival = 1;
    Node* p7 = create_T(INT, nv, 4, 13);
    Node* p8 = create_NT(EXP);
    allo_child(1, p8);
    p8->children[0] = p7;
    set_attr(p8, p7);

    // EXP -> EXP PLUS EXP
    Node* p9 = create_NT(EXP);
    allo_child(3, p9);
    p9->children[0] = p5;
    p9->children[1] = p6;
    p9->children[2] = p8;
    set_attr(p9, p7);
*/

// flex
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

	#define FORM_SUBTREE_0(root, root_type)\
		root = create_NT(root_type);\
		allo_child(0, root);\

	#define FORM_SUBTREE_1(root, root_type, child0)\
		root = create_NT(root_type);\
		allo_child(1, root);\
		root->children[0] = child0;\
		set_attr(root, child0);\

	#define FORM_SUBTREE_2(root, root_type, child0, child1)\
		root = create_NT(root_type);\
		allo_child(2, root);\
		root->children[0] = child0;\
		set_attr(root, child0);\
		root->children[1] = child1;\

	#define FORM_SUBTREE_3(root, root_type, child0, child1, child2)\
		root = create_NT(root_type);\
		allo_child(3, root);\
		root->children[0] = child0;\
		set_attr(root, child0);\
		root->children[1] = child1;\
		root->children[2] = child2;\

	#define FORM_SUBTREE_4(root, root_type, child0, child1, child2, child3)\
		root = create_NT(root_type);\
		allo_child(4, root);\
		root->children[0] = child0;\
		set_attr(root, child0);\
		root->children[1] = child1;\
		root->children[2] = child2;\
		root->children[3] = child3;\

	#define FORM_SUBTREE_5(root, root_type, child0, child1, child2, child3, child4)\
		root = create_NT(root_type);\
		allo_child(5, root);\
		root->children[0] = child0;\
		set_attr(root, child0);\
		root->children[1] = child1;\
		root->children[2] = child2;\
		root->children[3] = child3;\
		root->children[4] = child4;\

	#define FORM_SUBTREE_6(root, root_type, child0, child1, child2, child3, child4, child5)\
		root = create_NT(root_type);\
		allo_child(6, root);\
		root->children[0] = child0;\
		set_attr(root, child0);\
		root->children[1] = child1;\
		root->children[2] = child2;\
		root->children[3] = child3;\
		root->children[4] = child4;\
		root->children[5] = child5;\

	#define FORM_SUBTREE_7(root, root_type, child0, child1, child2, child3, child4, child5, child6)\
		root = create_NT(root_type);\
		allo_child(7, root);\
		root->children[0] = child0;\
		set_attr(root, child0);\
		root->children[1] = child1;\
		root->children[2] = child2;\
		root->children[3] = child3;\
		root->children[4] = child4;\
		root->children[5] = child5;\
		root->children[6] = child6;\


#endif // PARSER_TREE_H

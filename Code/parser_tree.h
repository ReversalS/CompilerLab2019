#ifndef PARSER_TREE_H
#define PARSER_TREE_H

#include "enums.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// a global root for parser tree
struct Node* global_root;

// bison?
// 为节点的子数组分配空间
// TODO: 添加再分配功能？Free 原有 malloc 新空间
void allo_child(int num, struct Node* p);
// parent的行号为第一个子节点的行号
void set_attr(struct Node* parent, struct Node* child);
// 析构语法树，归还内存
void deconstruct(struct Node* p);
// 递归打印
void print_tree(struct Node* p, int level);

/*
example:

EXP
    EXP
        ID
    PLUS
    EXP
        ID


    // EXP -> ID
    // FLEX: construct Nodes and return pointers p5, p4
    struct Node* p4 = create_ID("i", 4, 9);
    struct Node* p5 = create_NT(EXP);

    // BISON: build the parser tree Bottom up
    allo_child(1, p5);
    p5->children[0] = p4;
    set_attr(p5, p4);

    // PLUS
    nv.other = PLUS;
    struct Node* p6 = create_T(OTHER, nv, 4, 11);

    // EXP -> INT
    nv.ival = 1;
    struct Node* p7 = create_T(INT, nv, 4, 13);
    struct Node* p8 = create_NT(EXP);
    allo_child(1, p8);
    p8->children[0] = p7;
    set_attr(p8, p7);

    // EXP -> EXP PLUS EXP
    struct Node* p9 = create_NT(EXP);
    allo_child(3, p9);
    p9->children[0] = p5;
    p9->children[1] = p6;
    p9->children[2] = p8;
    set_attr(p9, p7);
*/

// flex
// 创建节点，不会被用户显式调用
struct Node* create_node(enum SYMBOL_TYPE st, union NODE_TYPE nt, union NODE_VALUE nv, struct location loc);
// 创建空串
struct Node* create_EP();
// 创建非终结符
struct Node* create_NT(enum NT_TYPE nt_type);
// 创建终结符（非 id）
struct Node* create_T(enum T_TYPE t_type, union NODE_VALUE nv, int line, int column);
// 创建 id
struct Node* create_ID(char* str, int line, int column);


/* macros that help to efficiently build trees */
    /* Not Possible since Macro-expansion will not be performed in Bison Actions */
    // #define FORM_SUBTREE(n, root_type)\
    //     $$ = create_NT(root_type);\
    //     allo_child(n, $$);\
    //     for (int i = 1; i <= n; i++)\
    //         root->children[i-1] = $##i;\
    //         set_attr($$, $##i);

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
		set_attr(root, child1);\

	#define FORM_SUBTREE_3(root, root_type, child0, child1, child2)\
		root = create_NT(root_type);\
		allo_child(3, root);\
		root->children[0] = child0;\
		set_attr(root, child0);\
		root->children[1] = child1;\
		set_attr(root, child1);\
		root->children[2] = child2;\
		set_attr(root, child2);\

	#define FORM_SUBTREE_4(root, root_type, child0, child1, child2, child3)\
		root = create_NT(root_type);\
		allo_child(4, root);\
		root->children[0] = child0;\
		set_attr(root, child0);\
		root->children[1] = child1;\
		set_attr(root, child1);\
		root->children[2] = child2;\
		set_attr(root, child2);\
		root->children[3] = child3;\
		set_attr(root, child3);\

	#define FORM_SUBTREE_5(root, root_type, child0, child1, child2, child3, child4)\
		root = create_NT(root_type);\
		allo_child(5, root);\
		root->children[0] = child0;\
		set_attr(root, child0);\
		root->children[1] = child1;\
		set_attr(root, child1);\
		root->children[2] = child2;\
		set_attr(root, child2);\
		root->children[3] = child3;\
		set_attr(root, child3);\
		root->children[4] = child4;\
		set_attr(root, child4);\

	#define FORM_SUBTREE_6(root, root_type, child0, child1, child2, child3, child4, child5)\
		root = create_NT(root_type);\
		allo_child(6, root);\
		root->children[0] = child0;\
		set_attr(root, child0);\
		root->children[1] = child1;\
		set_attr(root, child1);\
		root->children[2] = child2;\
		set_attr(root, child2);\
		root->children[3] = child3;\
		set_attr(root, child3);\
		root->children[4] = child4;\
		set_attr(root, child4);\
		root->children[5] = child5;\
		set_attr(root, child5);\

	#define FORM_SUBTREE_7(root, root_type, child0, child1, child2, child3, child4, child5, child6)\
		root = create_NT(root_type);\
		allo_child(7, root);\
		root->children[0] = child0;\
		set_attr(root, child0);\
		root->children[1] = child1;\
		set_attr(root, child1);\
		root->children[2] = child2;\
		set_attr(root, child2);\
		root->children[3] = child3;\
		set_attr(root, child3);\
		root->children[4] = child4;\
		set_attr(root, child4);\
		root->children[5] = child5;\
		set_attr(root, child5);\
		root->children[6] = child6;\
		set_attr(root, child6);\



#endif // PARSER_TREE_H

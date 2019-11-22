#include "trans_stmt.h"

extern void translate_StmtList(Node* root);
extern void translate_DefList(Node* root);

void translate_Stmt(Node* root)
{
    Operand* ret;
    switch (root->body.stmt) {
    case STMT_EXP:
        translate_Exp(root->children[0], 0);
        root->code = root->children[0]->code;
        break;
    case STMT_COMP:
        translate_Stmt_comp(root->children[0]);
        root->code = root->children[0]->code;
        break;
    case STMT_RT:
        ret = translate_Exp(root->children[1], 1);
        InterCode* ic = create_return(ret);
        ic->is_addr.op1 = ret->is_addr;
        APPEND2(root, root->children[1], create_list_node(ic));
        break;
    case STMT_IF:
        translate_Stmt_if(root);
        break;
    case STMT_IFEL:
        translate_Stmt_ifel(root);
        break;
    case STMT_WHILE:
        translate_Stmt_while(root);
        break;
    }
}

void translate_Stmt_comp(Node* root)
{
    translate_DefList(root->children[1]);
    translate_StmtList(root->children[2]);
    concatenate_code(&root->code, &root->children[1]->code, &root->children[2]->code);
}

void translate_Stmt_if(Node* root)
{
    InterCode* label_true = create_label();
    InterCode* label_false = create_label();
    int ret = translate_Cond(root->children[2], label_true, label_false);
    switch (ret) {
    case 0:
        translate_Stmt(root->children[4]);
        APPEND2(root, root->children[2], create_list_node(label_true));
        CONCAT(root, root, root->children[4]);
        APPEND1(root, create_list_node(label_false));
        break;
    case 1:
        deconstruct_ic(label_true);
        deconstruct_ic(label_false);
        root->code = root->children[4]->code;
        break;
    case -1:
        deconstruct_ic(label_true);
        deconstruct_ic(label_false);
        break;
    }
}

void translate_Stmt_ifel(Node* root)
{
    InterCode* l1 = create_label();
    InterCode* l2 = create_label();
    InterCode* l3 = create_label();
    int ret = translate_Cond(root->children[2], l1, l2);
    switch (ret) {
    case 0:
        translate_Stmt(root->children[4]);
        translate_Stmt(root->children[6]);
        APPEND1(root->children[2], create_list_node(l1));
        APPEND1(root->children[4], create_list_node(create_goto(l3->code.label_id)));
        APPEND1(root->children[6], create_list_node(l3));
        CONCAT(root, root->children[2], root->children[4]);
        APPEND1(root, create_list_node(l2));
        CONCAT(root, root, root->children[6]);
        break;
    case 1:
        deconstruct_ic(l1);
        deconstruct_ic(l2);
        deconstruct_ic(l3);
        translate_Stmt(root->children[4]);
        root->code = root->children[4]->code;
        break;
    case -1:
        deconstruct_ic(l1);
        deconstruct_ic(l2);
        deconstruct_ic(l3);
        translate_Stmt(root->children[6]);
        root->code = root->children[6]->code;
        break;
    }
}

void translate_Stmt_while(Node* root)
{
    InterCode* l1 = create_label();
    InterCode* l2 = create_label();
    InterCode* l3 = create_label();
    int ret = translate_Cond(root->children[2], l2, l3);
    switch (ret) {
    case 0:
        translate_Stmt(root->children[4]);
        INSERT1(root->children[2], create_list_node(l1));
        INSERT1(root->children[4], create_list_node(l2));
        CONCAT(root, root->children[2], root->children[4]);
        APPEND1(root, create_list_node(create_goto(l1->code.label_id)));
        APPEND1(root, create_list_node(l3));
        break;
    case 1:
        translate_Stmt(root->children[4]);
        INSERT1(root->children[4], create_list_node(l1));
        APPEND2(root, root->children[4], create_list_node(create_goto(l1->code.label_id)));
        deconstruct_ic(l2);
        deconstruct_ic(l3);
        break;
    case -1:
        deconstruct_ic(l1);
        deconstruct_ic(l2);
        deconstruct_ic(l3);
        break;
    }
}
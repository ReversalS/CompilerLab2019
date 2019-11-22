#include "trans_program.h"

void translate_Program(Node* root)
{
    translate_ExtDefList(root->children[0]);
    root->code = root->children[0]->code;
}

void translate_ExtDefList(Node* root)
{
    if (root->symbol_type != EPSILON) {
        if (root->children[1]->symbol_type != EPSILON) {
            translate_ExtDef(root->children[0]);
            translate_ExtDefList(root->children[1]);
            CONCAT(root, root->children[0], root->children[1]);
        } else {
            translate_ExtDef(root->children[0]);
            root->code = root->children[0]->code;
        }
    }
}

void translate_ExtDef(Node* root)
{
    if (root->child_num == 3 && root->children[1]->node_type.nt_type == FUNDEC) {
        translate_ExtDef_func(root);
    } else {
        CANNOT_TRANSLATE;
        exit(-1);
    }
}

void translate_ExtDef_func(Node* root)
{
    translate_FunDec(root->children[1]);
    translate_Stmt_comp(root->children[2]);
    CONCAT(root, root->children[1], root->children[2]);
}

void translate_FunDec(Node* root)
{
    CREATE(root, create_list_node(create_func_def(root->children[0]->node_value.id)));
    if (root->child_num == 4) {
        AttrList* p = root->children[2]->attr.para_list;
        while (p != NULL) {
            if(kind(p->attr.para.type) == ARRAY){
                APPEND1(root, create_list_node(create_param(create_op(VAR, p->attr.para.id, 1))));
            } else {
                APPEND1(root, create_list_node(create_param(create_op(VAR, p->attr.para.id, 0))));
            }
            p = p->next;
        }
    }
}

void translate_StmtList(Node* root)
{
    if (root->symbol_type != EPSILON) {
        if (root->children[1]->symbol_type != EPSILON) {
            translate_Stmt(root->children[0]);
            translate_StmtList(root->children[1]);
            CONCAT(root, root->children[0], root->children[1]);
        } else {
            translate_Stmt(root->children[0]);
            root->code = root->children[0]->code;
        }
    }
}

void translate_DefList(Node* root)
{
    if (root->symbol_type != EPSILON) {
        if (root->children[1]->symbol_type != EPSILON) {
            translate_Def(root->children[0]);
            translate_DefList(root->children[1]);
            CONCAT(root, root->children[0], root->children[1]);
        } else {
            translate_Def(root->children[0]);
            root->code = root->children[0]->code;
        }
    }
}

void translate_Def(Node* root)
{
    translate_DecList(root->children[1]);
    root->code = root->children[1]->code;
}

void translate_DecList(Node* root)
{
    if (root->child_num == 3) {
        translate_Dec(root->children[0]);
        translate_DecList(root->children[2]);
        CONCAT(root, root->children[0], root->children[2]);
    } else {
        translate_Dec(root->children[0]);
        root->code = root->children[0]->code;
    }
}

void translate_Dec(Node* root)
{
    if (root->child_num == 3) {
        Operand* temp = translate_Exp(root->children[2], 1);
        Operand* var = create_op(VAR, root->children[0]->attr.id, 0);
        InterCode* ic = create_assign(NORMAL, var, temp);
        ic->is_addr.op2 = temp->is_addr;
        APPEND2(root, root->children[2], create_list_node(ic));
    } else {
        if(root->children[0]->child_num > 1){
            AttrList* p = root->children[0]->attr.opt_array;
            Operand* temp = create_op(TEMP, NULL, 0);
            int size = 4;
            while(p != NULL){
                size *= p->attr.array_size;
                p = p->next;
            }
            CREATE(root, create_list_node(create_dec(temp, size)));
            APPEND1(root, create_list_node(create_assign(GET_ADDR, create_op(VAR, root->children[0]->attr.id, 1), temp)));
        }
    }
}
#include "semantic.h"

void var_id(Node* root, Node* id)
{
    copy_str(&root->attr.id, id->node_value.id);
}

void var_var_lb_int_rb(Node* root, Node* var, Node* int_n)
{
    copy_str(&root->attr.id, var->attr.id);
    copy_attrlist(&root->attr.opt_array, var->attr.opt_array);
    insert(&root->attr.opt_array, ARRAY_SIZE, (void*)(long)int_n->node_value.ival);
}

void extdec_var(Node* root, Node* var)
{
    Var_Dec temp;
    int i = 0;
    memset(&temp, 0, sizeof(Var_Dec));
    copy_str(&temp.id, var->attr.id);
    if (var->attr.opt_array != NULL) {
        AttrList* p = var->attr.opt_array;
        while (p != NULL) {
            i++;
            p = p->next;
        }
        temp.opt_array_size_num = i;
        temp.opt_array_size = (int*)malloc(i);
        p = var->attr.opt_array;
        i = 0;
        while (p != NULL) {
            temp.opt_array_size[i] = p->attr.array_size;
            i++;
            p = p->next;
        }
    }
    insert(&root->attr.var_list, VAR_DEC, &temp);
    free(temp.id);
    free(temp.opt_array_size);
}

void extdec_var_comma_extdec(Node* root, Node* var, Node* extdec)
{
    copy_attrlist(&root->attr.var_list, extdec->attr.var_list);
    extdec_var(root, var);
}

void spec_type(Node* root, Node* type){
    if(type->node_value.type == INT_T){
        root->attr.type_id = construct_basic(INT_BASIC);
    } else {
        root->attr.type_id = construct_basic(FLOAT_BASIC);
    }
}
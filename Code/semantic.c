#include "semantic.h"

int legal_struct_def(char* name){
    int type;
    if(!existSymbol(name)){
        return -1;
    } else {
        type = getSymbolType(name);
        if(type_set[type].kind == STRUCTURE && strcmp(type_set[type].u.structure.name, name) == 0){
            return type;
        } else {
            return -1;
        }
    }
}

Var_Dec* format_vardec(Node* var){
    Var_Dec* temp = (Var_Dec*)malloc(sizeof(Var_Dec));
    int i = 0;
    memset(temp, 0, sizeof(Var_Dec));
    copy_str(&temp->id, var->attr.id);
    if (var->attr.opt_array != NULL) {
        AttrList* p = var->attr.opt_array;
        while (p != NULL) {
            i++;
            p = p->next;
        }
        temp->opt_array_size_num = i;
        temp->opt_array_size = (int*)malloc(i);
        p = var->attr.opt_array;
        i = 0;
        while (p != NULL) {
            temp->opt_array_size[i] = p->attr.array_size;
            i++;
            p = p->next;
        }
    }
    return temp;
}

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
    Var_Dec* temp = format_vardec(var);
    insert(&root->attr.var_list, VAR_DEC, temp);
    free(temp->id);
    free(temp->opt_array_size);
    free(temp);
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

void spec_struct(Node* root, Node* stru){
    root->attr.type_id = stru->attr.type_id;
}

void struct_sturct_tag(Node* root, Node* tag){
    root->attr.type_id = legal_struct_def(tag->attr.id);
}

void tag_id(Node* root, Node* id){
    copy_str(&root->attr.id, id->node_value.id);
}

void opt_id(Node* root, Node* id){
    copy_str(&root->attr.id, id->node_value.id);
}

void dec_var(Node* root, Node* var){
    Var_Dec* temp = format_vardec(var);
    insert(&root->attr.var_list, VAR_DEC, temp);
    free(temp->id);
    free(temp->opt_array_size);
    free(temp);
}

void dec_var_assignop_exp(Node* root, Node* var, Node* exp){
    // TODO: type check

    Var_Dec* temp = format_vardec(var);
    insert(&root->attr.var_list, VAR_DEC, temp);
    free(temp->id);
    free(temp->opt_array_size);
    free(temp);
}
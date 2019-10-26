#include "semantic.h"

int legal_struct_def(char* name)
{
    int type;
    if (getSymbol(name, 1) == NULL) {
        return -1;
    } else {
        type = getSymbol(name, 1)->type_id;
        if(type < 0 || type >= type_num){
            return -1;
        }
        if (type_set[type].kind == STRUCTURE && strcmp(type_set[type].u.structure.name, name) == 0) {
            return type;
        } else {
            return -1;
        }
    }
}

int init_field(AttrList* vardef_list)
{
    AttrList* p = vardef_list;
    while (p != NULL) {
        for (int i = 0; i < p->attr.var_def.var_num; i++) {
            if (p->attr.var_def.init[i] == 1) {
                print_error(15, p->attr.var_def.line, p->attr.var_def.id[i], NULL);
            }
        }
        p = p->next;
    }
}

int dup_field(AttrList* vardef_list)
{
    AttrList* p = vardef_list;
    unsigned char* temp_hash = (unsigned char*)malloc(sizeof(unsigned char) * 997);
    unsigned hash = 0;
    memset(temp_hash, 0, sizeof(unsigned char) * 997);
    while (p != NULL) {
        for (int i = 0; i < p->attr.var_def.var_num; i++) {
            hash = hash_pjw(p->attr.var_def.id[i]) % 997;
            temp_hash[hash]++;
            if (temp_hash[hash] > 1) {
                // duplicate
                print_error(15, p->attr.var_def.line, p->attr.var_def.id[i], NULL);
            }
        }
        p = p->next;
    }
    free(temp_hash);
}

unsigned hash_pjw(char* str)
{
    unsigned seed = 131;
    unsigned hash = 0;
    while (*str) {
        hash = hash * seed + (*str++);
    }
    return (hash & 0x7fffffff);
}

Var_Dec* format_vardec(Node* var)
{
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
    temp->opt_init_type_id = -1;
    return temp;
}

Var_Def* format_vardef(Node* var, int type_id)
{
    Var_Def* temp = (Var_Def*)malloc(sizeof(Var_Def));
    int i = 0;
    int type1 = type_id;
    memset(temp, 0, sizeof(Var_Def));
    AttrList* p = var->attr.var_list;
    while (p != NULL) {
        i++;
        p = p->next;
    }
    temp->var_num = i;
    temp->id = (char**)malloc(sizeof(char*) * temp->var_num);
    temp->type_id = (int*)malloc(sizeof(int) * temp->var_num);
    temp->init = (int*)malloc(sizeof(int) * temp->var_num);
    temp->line = var->loc.line;
    p = var->attr.var_list;
    for (int j = 0; j < temp->var_num; j++) {
        copy_str(&temp->id[j], p->attr.var_dec.id);
        if (p->attr.var_dec.opt_array_size_num > 0) {
            for (int k = 0; k < p->attr.var_dec.opt_array_size_num; k++) {
                type1 = construct_array(type1, p->attr.var_dec.opt_array_size[k]);
            }
            temp->type_id[j] = type1;
            type1 = type_id;
        } else {
            temp->type_id[j] = type_id;
        }
        if (p->attr.var_dec.opt_init_type_id != -1) {
            temp->init[j] = 1;
        }
        if (p->attr.var_dec.opt_init_type_id != -1 && temp->type_id[j] != p->attr.var_dec.opt_init_type_id) {
            print_error(5, var->loc.line, NULL, NULL);
        }
        p = p->next;
    }
    return temp;
}

Para* format_para(Node* var, int type_id){
    Para* temp = (Para*)malloc(sizeof(Para));
    AttrList* p = NULL;
    int temp_type = type_id;
    memset(temp, 0, sizeof(Para));
    copy_str(&temp->id, var->attr.id);
    if(var->attr.opt_array != NULL){
        p = var->attr.opt_array;
        while(p != NULL){
            temp_type = construct_array(temp_type, p->attr.array_size);
            p = p->next;
        }
        temp->type = temp_type;
    } else {
        temp->type = type_id;
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

void spec_type(Node* root, Node* type)
{
    if (type->node_value.type == INT_T) {
        root->attr.type_id = construct_basic(INT_BASIC);
    } else {
        root->attr.type_id = construct_basic(FLOAT_BASIC);
    }
}

void spec_struct(Node* root, Node* stru)
{
    root->attr.type_id = stru->attr.type_id;

}

void struct_opt_lc_def_rc(Node* root, Node* opt, Node* def)
{
    int ret = -1;
    int index = -1;
    AttrList* p;
    if (opt->attr.id != NULL) {
        index = insertSymbol(opt->attr.id, 1, -1, root->loc.line, root->loc.column, 0);
        if (index == -1) {
            // redefine
            print_error(16, opt->loc.line, opt->attr.id, NULL);
            dup_field(def->attr.vardef_list);
            init_field(def->attr.vardef_list);
            root->attr.type_id = -1;
        } else {
            ret = construct_struct(opt->attr.id);
            dup_field(def->attr.vardef_list);
            init_field(def->attr.vardef_list);
            if (ret == -1) {
                // redefine
                // impossible
                printf("WTF\n");
            }
            p = def->attr.vardef_list;
            while (p != NULL) {
                for (int i = 0; i < p->attr.var_def.var_num; i++) {
                    add_member(ret, p->attr.var_def.id[i], p->attr.var_def.type_id[i]);
                }
                p = p->next;
            }
            updateSymbolType(index, ret);
            root->attr.type_id = ret;
        }
    } else {
        char* name = get_random_name();
        ret = construct_struct(name);
        dup_field(def->attr.vardef_list);
        init_field(def->attr.vardef_list);
        p = def->attr.vardef_list;
        while (p != NULL) {
            for (int i = 0; i < p->attr.var_def.var_num; i++) {
                add_member(ret, p->attr.var_def.id[i], p->attr.var_def.type_id[i]);
            }
            p = p->next;
        }
        insertSymbol(name, 1, ret, root->loc.line, root->loc.column, 0);
        root->attr.type_id = ret;
    }
}

void struct_struct_tag(Node* root, Node* tag)
{
    root->attr.type_id = legal_struct_def(tag->attr.id);
    if(root->attr.type_id == -1){
        print_error(17, root->loc.line, tag->attr.id, NULL);
    }
}

void tag_id(Node* root, Node* id)
{
    copy_str(&root->attr.id, id->node_value.id);
}

void opt_id(Node* root, Node* id)
{
    copy_str(&root->attr.id, id->node_value.id);
}

void dec_var(Node* root, Node* var)
{
    Var_Dec* temp = format_vardec(var);
    insert(&root->attr.var_list, VAR_DEC, temp);
    free(temp->id);
    free(temp->opt_array_size);
    free(temp);
}

void dec_var_assignop_exp(Node* root, Node* var, Node* exp)
{
    // TODO: type check

    Var_Dec* temp = format_vardec(var);
    temp->opt_init_type_id = exp->attr.type_id;

    insert(&root->attr.var_list, VAR_DEC, temp);
    free(temp->id);
    free(temp->opt_array_size);
    free(temp);
}

void dec_dec(Node* root, Node* dec)
{
    insert(&root->attr.var_list, VAR_DEC, (void*)&dec->attr.var_list->attr.var_dec);
}

void dec_dec_comma_dec(Node* root, Node* dec, Node* dec_list)
{
    copy_attrlist(&root->attr.var_list, dec_list->attr.var_list);
    dec_dec(root, dec);
}

void def_spec_dec_semi(Node* root, Node* spec, Node* dec)
{
    Var_Def* temp = format_vardef(dec, spec->attr.type_id);
    insert(&root->attr.vardef_list, VAR_DEF, temp);
    free(temp->type_id);
    for (int i = 0; i < temp->var_num; i++) {
        free(temp->id[i]);
    }
    free(temp->id);
    free(temp);
}

void def_def_def(Node* root, Node* def, Node* deflist)
{
    if (deflist->symbol_type != EPSILON) {
        copy_attrlist(&root->attr.vardef_list, deflist->attr.vardef_list);
    }
    insert(&root->attr.vardef_list, VAR_DEF, &def->attr.vardef_list->attr.var_def);
}

void exp_int(Node* root)
{
    root->attr.type_id = construct_basic(INT_BASIC);
}

void exp_float(Node* root)
{
    root->attr.type_id = construct_basic(FLOAT_BASIC);
}

void param_spec_var(Node* root, Node* spec, Node* var){
    Para* temp = format_para(var, spec->attr.type_id);
    insert(&root->attr.para_list, PARA, temp);
    free(temp->id);
    free(temp);
}
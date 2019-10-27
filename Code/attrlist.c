#include "attrlist.h"

void deconstruct_attrlist(AttrList* p)
{
    if (p == NULL) {
        return;
    } else {
        // free attr
        // TODO:
        switch (p->kind) {
        case VAR_DEC:
            free(p->attr.var_dec.id);
            free(p->attr.var_dec.opt_array_size);
            break;
        case VAR_DEF:
            free(p->attr.var_def.type_id);
            free(p->attr.var_def.init);
            for (int i = 0; i < p->attr.var_def.var_num; i++) {
                free(p->attr.var_def.id[i]);
            }
            free(p->attr.var_def.id);
            break;
        case PARA:
            free(p->attr.para.id);
            break;
        }
        deconstruct_attrlist(p->next);
        // free p
        free(p);
        return;
    }
}

void copy_str(char** dest, char* src)
{
    if(src == NULL){
        return ;
    }
    (*dest) = (char*)malloc(strlen(src) + 1);
    strcpy((*dest), src);
}

void copy_attrlist(AttrList** dest, AttrList* src)
{
    if (src == NULL) {
        (*dest) = NULL;
        return;
    } else {
        (*dest) = (AttrList*)malloc(sizeof(AttrList));
        memset((*dest), 0, sizeof(AttrList));
        (*dest)->kind = src->kind;
        (*dest)->next = NULL;
        // TODO:
        switch (src->kind) {
        case ARRAY_SIZE:
            update((*dest), ARRAY_SIZE, (void*)(long)src->attr.array_size);
            break;
        case VAR_DEC:
            update((*dest), VAR_DEC, (void*)&src->attr.var_dec);
            break;
        case VAR_DEF:
            update((*dest), VAR_DEF, (void*)&src->attr.var_def);
            break;
        case PARA:
            update((*dest), PARA, (void*)&src->attr.para);
        }
        copy_attrlist(&(*dest)->next, src->next);
    }
}

void insert(AttrList** p, ATTR_TYPE type, void* val)
{
    AttrList* q = NULL;
    if ((*p) == NULL) {
        (*p) = (AttrList*)malloc(sizeof(AttrList));
        memset((*p), 0, sizeof(AttrList));
        q = (*p);
    } else {
        q = (*p);
        (*p) = (AttrList*)malloc(sizeof(AttrList));
        memset((*p), 0, sizeof(AttrList));
        (*p)->next = q;
        q = (*p);
    }
    update(q, type, val);
}

void append(AttrList** p, ATTR_TYPE type, void* val)
{
    AttrList* q = NULL;
    if ((*p) == NULL) {
        (*p) = (AttrList*)malloc(sizeof(AttrList));
        memset((*p), 0, sizeof(AttrList));
        q = (*p);
    } else {
        q = (*p);
        while (q->next != NULL) {
            q = q->next;
        }
        q->next = (AttrList*)malloc(sizeof(AttrList));
        memset(q->next, 0, sizeof(AttrList));
        q = q->next;
    }
    update(q, type, val);
}

void update(AttrList* p, ATTR_TYPE type, void* val)
{
    Var_Dec* q = NULL;
    Var_Def* r = NULL;
    Para* s = NULL;
    // TODO:
    switch (type) {
    case ARRAY_SIZE:
        p->kind = ARRAY_SIZE;
        p->attr.array_size = (long)val;
        break;
    case VAR_DEC:
        q = (Var_Dec*)val;
        p->kind = VAR_DEC;
        copy_str(&p->attr.var_dec.id, q->id);
        if (q->opt_array_size_num > 0) {
            p->attr.var_dec.opt_array_size = (int*)malloc(q->opt_array_size_num);
            for (int i = 0; i < q->opt_array_size_num; i++) {
                p->attr.var_dec.opt_array_size[i] = q->opt_array_size[i];
            }
        }
        p->attr.var_dec.opt_array_size_num = q->opt_array_size_num;
        p->attr.var_dec.opt_init_type_id = q->opt_init_type_id;
        break;
    case VAR_DEF:
        r = (Var_Def*)val;
        p->kind = VAR_DEF;
        p->attr.var_def.var_num = r->var_num;
        p->attr.var_def.id = (char**)malloc(sizeof(char*) * r->var_num);
        p->attr.var_def.type_id = (int*)malloc(sizeof(int) * r->var_num);
        p->attr.var_def.init = (int*)malloc(sizeof(int) * r->var_num);
        for (int i = 0; i < r->var_num; i++) {
            copy_str(&p->attr.var_def.id[i], r->id[i]);
            p->attr.var_def.type_id[i] = r->type_id[i];
            p->attr.var_def.init[i] = r->init[i];
        }
        p->attr.var_def.line = r->line;
        break;
    case PARA:
        p->kind = PARA;
        s = (Para*)val;
        p->attr.para.type = s->type;
        copy_str(&p->attr.para.id, s->id);
        break;
    }
}
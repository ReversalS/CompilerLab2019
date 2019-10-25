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
        }
        deconstruct_attrlist(p->next);
        // free p
        free(p);
        return;
    }
}

void copy_str(char** dest, char* src)
{
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
            (*dest)->attr.array_size = src->attr.array_size;
            break;
        case VAR_DEC:
            copy_str(&(*dest)->attr.var_dec.id, src->attr.var_dec.id);
            if (src->attr.var_dec.opt_array_size_num > 0) {
                (*dest)->attr.var_dec.opt_array_size = (int*)malloc(src->attr.var_dec.opt_array_size_num);
                for (int i = 0; i < src->attr.var_dec.opt_array_size_num; i++) {
                    (*dest)->attr.var_dec.opt_array_size[i] = src->attr.var_dec.opt_array_size[i];
                }
            }
            (*dest)->attr.var_dec.opt_array_size_num = src->attr.var_dec.opt_array_size_num;
            break;
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
        break;
    }
}
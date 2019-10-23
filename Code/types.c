#include "types.h"

int eq(Type* t1, Type* t2)
{
    if (t1->kind != t2->kind) {
        return FALSE;
    } else {
        switch (t1->kind) {
        case BASIC:
            if (t1->u.basic == t2->u.basic) {
                return TRUE;
            } else {
                return FALSE;
            }
            break;
        case ARRAY:
            return eq(t1->u.array.elem, t2->u.array.elem);
            break;
        case STRUCTURE:
            if (strcmp(t1->u.structure.name, t2->u.structure.name) == 0) {
                return TRUE;
            } else {
                return FALSE;
            }
            break;
        }
    }
}

void deconstruct_type(Type* p)
{
    if (p->kind == BASIC) {
        free(p);
        return;
    } else {
        switch (p->kind) {
        case ARRAY:
            deconstruct_type(p->u.array.elem);
            free(p);
            break;
        case STRUCTURE:
            free(p->u.structure.name);
            deconstruct_field(p->u.structure.fields);
            free(p);
            break;
        }
    }
}

void deconstruct_field(FieldList* p)
{
    if (p == NULL) {
        return;
    } else {
        deconstruct_field(p->next);
        free(p->name);
        deconstruct_type(p->type);
        free(p);
        return;
    }
}

void copy_type(Type** dest, Type* src)
{
    if ((*dest) != NULL) {
        deconstruct_type((*dest));
        (*dest) = NULL;
    }
    (*dest) = (Type*)malloc(sizeof(Type));
    switch (src->kind) {
    case BASIC:
        (*dest)->kind = BASIC;
        (*dest)->u.basic = src->u.basic;
        break;
    case ARRAY:
        (*dest)->kind = ARRAY;
        (*dest)->u.array.size = src->u.array.size;
        (*dest)->u.array.elem = NULL;
        copy_type(&(*dest)->u.array.elem, src->u.array.elem);
        break;
    case STRUCTURE:
        (*dest)->kind = STRUCTURE;
        (*dest)->u.structure.name = NULL;
        copy_name(&(*dest)->u.structure.name, src->u.structure.name);
        (*dest)->u.structure.fields = NULL;
        copy_field(&(*dest)->u.structure.fields, src->u.structure.fields);
        break;
    }
}

void copy_field(FieldList** dest, FieldList* src)
{
    if ((*dest) != NULL) {
        deconstruct_field((*dest));
        (*dest) = NULL;
    }
    if (src == NULL) {
        (*dest) = src;
        return;
    } else {
        (*dest) = (FieldList*)malloc(sizeof(FieldList));
        (*dest)->name = NULL;
        copy_name(&(*dest)->name, src->name);
        (*dest)->type = NULL;
        copy_type(&(*dest)->type, src->type);
        (*dest)->next = NULL;
        copy_field(&(*dest)->next, src->next);
    }
}

void copy_name(char** dest, char* src)
{
    if ((*dest) != NULL) {
        free((*dest));
        (*dest) = NULL;
    }
    (*dest) = (char*)malloc(strlen(src) + 1);
    strcpy((*dest), src);
}

Type* construct_basic(enum B_TYPE type)
{
    Type* p = (Type*)malloc(sizeof(Type));
    p->kind = BASIC;
    p->u.basic = type;
    return p;
}

Type* construct_array(Type* base, int size)
{
    Type* p = (Type*)malloc(sizeof(Type));
    p->kind = ARRAY;
    p->u.array.size = size;
    p->u.array.elem = NULL;
    copy_type(&p->u.array.elem, base);
    return p;
}

Type* construct_struct(char* name)
{
    Type* p = (Type*)malloc(sizeof(Type));
    p->kind = STRUCTURE;
    p->u.structure.name = NULL;
    copy_name(&p->u.structure.name, name);
    p->u.structure.fields = NULL;
    return p;
}

void add_field(Type* base, char* name, Type* type)
{
    if (base == NULL || base->kind != STRUCTURE) {
        printf("Wrong Type.\n");
        return;
    }
    FieldList** p = &base->u.structure.fields;
    if ((*p) == NULL) {
        (*p) = (FieldList*)malloc(sizeof(FieldList));
        (*p)->name = NULL;
        copy_name(&(*p)->name, name);
        (*p)->type = NULL;
        copy_type(&(*p)->type, type);
        (*p)->next = NULL;
    } else {
        FieldList* q = (*p);
        while (q->next != NULL) {
            q = q->next;
        }
        q->next = (FieldList*)malloc(sizeof(FieldList));
        q->next->name = NULL;
        copy_name(&q->next->name, name);
        q->next->type = NULL;
        copy_type(&q->next->type, type);
        q->next->next = NULL;
    }
}

void print_type(Type* p, int level, int newline)
{
    switch (p->kind) {
    case BASIC:
        if (p->u.basic == INT_B) {
            printf("int");
        } else {
            printf("float");
        }
        break;
    case ARRAY:
        print_array(p, level);
        break;
    case STRUCTURE:
        printf("struct %s {\n", p->u.structure.name);
        print_field(p->u.structure.fields, level + 1);
        for (int j = 0; j < level; j++) {
            printf("  ");
        }
        printf("}");
    }
    if (newline != 0) {
        printf("\n");
    }
}

void print_array(Type* p, int level)
{
    Type* q = p->u.array.elem;
    while (q->kind == ARRAY) {
        q = q->u.array.elem;
    }
    print_type(q, level, 0);
    q = p;
    while (q->kind == ARRAY) {
        printf("[%d]", q->u.array.size);
        q = q->u.array.elem;
    }
}

void print_field(FieldList* p, int level)
{
    if (p == NULL) {
        return;
    } else {
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        print_type(p->type, level, 1);
        print_field(p->next, level);
    }
}
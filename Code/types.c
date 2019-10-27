#include "types.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void init_type_set()
{
    type_num = 0;
    random_index = 0;
    memset(type_set, 0, sizeof(Type) * TYPE_SET_SIZE);
}

void deconstruct_varlist(VarList* p)
{
    if (p == NULL) {
        return;
    } else {
        deconstruct_varlist(p->next);
        free(p->name);
        free(p);
        return;
    }
}

void deconstruct_set()
{
    for (int i = 0; i < type_num; i++) {
        if (type_set[i].kind == STRUCTURE) {
            free(type_set[i].u.structure.name);
            deconstruct_varlist(type_set[i].u.structure.fields);
        } else if (type_set[i].kind == FUNCTION) {
            free(type_set[i].u.function.name);
            deconstruct_varlist(type_set[i].u.function.parameters);
        } else {
            ;
        }
    }
}

BOOL EQ(Type* t1, Type* t2)
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
            if (t1->u.array.elem == t2->u.array.elem && t1->u.array.size == t2->u.array.size) {
                return TRUE;
            } else {
                return FALSE;
            }
            break;
        case STRUCTURE:
            if (strcmp(t1->u.structure.name, t2->u.structure.name) == 0) {
                return TRUE;
            } else {
                return FALSE;
            }
            break;
        case FUNCTION:
            if (strcmp(t1->u.function.name, t2->u.function.name) == 0) {
                return TRUE;
            } else {
                return FALSE;
            }
            break;
        }
    }
}

BOOL eq(int t1, int t2)
{
    if (!valid_type(t1) || !valid_type(t2)) {
        return FALSE;
    } else {
        if (type_set[t1].kind != type_set[t2].kind) {
            return FALSE;
        } else {
            switch (type_set[t1].kind) {
            case BASIC:
                if (type_set[t1].u.basic == type_set[t2].u.basic) {
                    return TRUE;
                } else {
                    return FALSE;
                }
                break;
            case ARRAY:
                if (eq(type_set[t1].u.array.elem, type_set[t2].u.array.elem)) {
                    return TRUE;
                } else {
                    return FALSE;
                }
                break;
            case STRUCTURE:
                if (strcmp(type_set[t1].u.structure.name, type_set[t2].u.structure.name) == 0) {
                    return TRUE;
                } else {
                    return FALSE;
                }
                break;
            case FUNCTION:
                if (strcmp(type_set[t1].u.function.name, type_set[t2].u.function.name) == 0) {
                    return TRUE;
                } else {
                    return FALSE;
                }
                break;
            }
        }
    }
}

int exists(Type* t)
{
    for (int i = 0; i < type_num; i++) {
        if (EQ(t, &type_set[i])) {
            return i;
        }
    }
    return -1;
}

int construct_basic(enum BASIC_TYPE type)
{
    Type* p = (Type*)malloc(sizeof(Type));
    int i = -1;
    p->kind = BASIC;
    p->u.basic = type;
    i = exists(p);
    free(p);
    if (i == -1) {
        type_set[type_num].kind = BASIC;
        type_set[type_num].u.basic = type;
        i = type_num;
        type_num++;
    }
    return i;
}

int construct_array(int base, int size)
{
    Type* p = (Type*)malloc(sizeof(Type));
    int i = -1;
    p->kind = ARRAY;
    p->u.array.elem = base;
    p->u.array.size = size;
    i = exists(p);
    if (i == -1) {
        type_set[type_num].kind = ARRAY;
        type_set[type_num].u.array.elem = base;
        type_set[type_num].u.array.size = size;
        i = type_num;
        type_num++;
    }
    return i;
}

int construct_struct(char* name)
{
    Type* p = (Type*)malloc(sizeof(Type));
    int i = -1;
    p->kind = STRUCTURE;
    p->u.structure.name = (char*)malloc(strlen(name) + 1);
    strcpy(p->u.structure.name, name);
    i = exists(p);
    free(p->u.structure.name);
    free(p);
    if (i == -1) {
        type_set[type_num].kind = STRUCTURE;
        type_set[type_num].u.structure.name = (char*)malloc(strlen(name) + 1);
        strcpy(type_set[type_num].u.structure.name, name);
        type_set[type_num].u.structure.fields = NULL;
        i = type_num;
        type_num++;
        return i;
    } else {
        return -1;
    }
}

int construct_function(char* name, int return_type)
{
    Type* p = (Type*)malloc(sizeof(Type));
    int i = -1;
    p->kind = FUNCTION;
    p->u.function.name = (char*)malloc(strlen(name) + 1);
    strcpy(p->u.function.name, name);
    i = exists(p);
    free(p->u.function.name);
    free(p);
    if (i == -1) {
        type_set[type_num].kind = FUNCTION;
        type_set[type_num].u.function.name = (char*)malloc(strlen(name) + 1);
        strcpy(type_set[type_num].u.function.name, name);
        type_set[type_num].u.function.parameters = NULL;
        type_set[type_num].u.function.return_type = return_type;
        i = type_num;
        type_num++;
        return i;
    } else {
        return -1;
    }
}

int add_member(int base, char* name, int type)
{
    if (!valid_type(base)) {
        return -1;
    } else if (type_set[base].kind == STRUCTURE) {
        if (type_set[base].u.structure.fields == NULL) {
            type_set[base].u.structure.fields = (VarList*)malloc(sizeof(VarList));
            type_set[base].u.structure.fields->type = type;
            type_set[base].u.structure.fields->name = (char*)malloc(strlen(name) + 1);
            strcpy(type_set[base].u.structure.fields->name, name);
            type_set[base].u.structure.fields->next = NULL;
            return 0;
        } else {
            VarList* p = type_set[base].u.structure.fields;
            while (p->next != NULL) {
                if (strcmp(name, p->name) == 0) {
                    return -1;
                }
                p = p->next;
            }
            if (strcmp(name, p->name) == 0) {
                return -1;
            } else {
                p->next = (VarList*)malloc(sizeof(VarList));
                p->next->type = type;
                p->next->name = (char*)malloc(strlen(name) + 1);
                strcpy(p->next->name, name);
                p->next->next = NULL;
                return 0;
            }
        }
    } else if (type_set[base].kind == FUNCTION) {
        if (type_set[base].u.function.parameters == NULL) {
            type_set[base].u.function.parameters = (VarList*)malloc(sizeof(VarList));
            type_set[base].u.function.parameters->type = type;
            type_set[base].u.function.parameters->name = (char*)malloc(strlen(name) + 1);
            strcpy(type_set[base].u.function.parameters->name, name);
            type_set[base].u.function.parameters->next = NULL;
            return 0;
        } else {
            VarList* p = type_set[base].u.function.parameters;
            while (p->next != NULL) {
                if (strcmp(name, p->name) == 0) {
                    return -1;
                }
                p = p->next;
            }
            if (strcmp(name, p->name) == 0) {
                return -1;
            } else {
                p->next = (VarList*)malloc(sizeof(VarList));
                p->next->type = type;
                p->next->name = (char*)malloc(strlen(name) + 1);
                strcpy(p->next->name, name);
                p->next->next = NULL;
                return 0;
            }
        }
    } else {
        return -1;
    }
}

void print_type(int type, int level, int newline)
{
    if(!valid_type(type)){
        return ;
    }
    switch (type_set[type].kind) {
    case BASIC:
        if (type_set[type].u.basic == INT_BASIC) {
            printf("int");
        } else {
            printf("float");
        }
        break;
    case ARRAY:
        print_array(type, level);
        break;
    case STRUCTURE:
        printf("struct %s {\n", type_set[type].u.structure.name);
        print_var(type_set[type].u.structure.fields, level + 1);
        for (int j = 0; j < level; j++) {
            printf("  ");
        }
        printf("}");
        break;
    case FUNCTION:
        print_type(type_set[type].u.function.return_type, level, 0);
        printf(" %s (\n", type_set[type].u.function.name);
        print_var(type_set[type].u.function.parameters, level + 1);
        for (int j = 0; j < level; j++) {
            printf("  ");
        }
        printf(")");
        break;
    }
    if (newline != 0) {
        printf("\n");
    }
}

void print_array(int type, int level)
{
    int q = type_set[type].u.array.elem;
    while (type_set[q].kind == ARRAY) {
        q = type_set[q].u.array.elem;
    }
    print_type(q, level, 0);
    q = type;
    while (type_set[q].kind == ARRAY) {
        printf("[%d]", type_set[q].u.array.size);
        q = type_set[q].u.array.elem;
    }
}

void print_var(VarList* p, int level)
{
    if (p == NULL) {
        return;
    } else {
        for (int i = 0; i < level; i++) {
            printf("  ");
        }
        print_type(p->type, level, 1);
        print_var(p->next, level);
    }
}

char* random_names[RANDOM_NAME_NUM] = {
    "Qq5CsLwBcW",
    "IXDvkP13TY",
    "bujjAPSc6X",
    "rup6THRDyQ",
    "HVIf9LkAQS",
    "XXv8vaAQds",
    "4D3JVNJ9KT",
    "JciK1GG8Iz",
    "byvjaGc6LX",
    "prMZ0tsBVj",
    "4CAj5g68JU",
    "ncLoGCkr7e",
    "6EP2NxWETQ",
    "FMrGgYAbZV",
    "dMS3uU0PXs",
    "nOSa7LD5W7",
    "2F7S4Ok2at",
    "AAmeUJbznR",
    "A9LZ08QIRq",
    "MNaAS5ZnGb",
    "jknRkm9r2O",
    "qj67JsMrE3",
    "2b6lrXLryU",
    "XGMxYev8Qm",
    "51YnZIJv7I",
    "qJIWtNMJFT",
    "VgcoSQIbzF",
    "vwfmQOmmJV",
    "yLRzxtd71E",
    "YMhe4t3odH",
    "PyVXHeeJZP",
    "GvGGfK7IPJ",
    "kKv33pnF7W",
    "8mjvY5COLa",
    "phJJfLDgvG",
    "x0hb3t3i9H",
    "EjnmpwA8OB",
    "i7MNjDbi2X",
    "rSyTONazh4",
    "8q9tbQiJCI",
    "xK9sN3Z6jD",
    "44AnOBQ87e",
    "pmk51OVy8I",
    "AcpdDo9jVM",
    "kTpUIqg1CR",
    "iTBXGKZbj6",
    "uzgFKXrXWv",
    "3oNStbin4I",
    "daGjmdoxhb",
    "2Z8v5yTEFI",
    "puH1ZjrEok",
    "64XObRu7WQ",
    "h4trgoNjA1",
    "ggG0m4UIUl",
    "GyTBMTb2NS",
    "siz3xkPSEU",
    "JlFiHSgX9m",
    "BCGaDMEtY3",
    "thVcgMqAVV",
    "YMBUzfODLE",
    "KQhiic7NDW",
    "ga1sF9TH6h",
    "Xj3Espiv30",
    "jYd7xPnucg",
};

char* get_random_name(){
    if(random_index < RANDOM_NAME_NUM){
        random_index++;
        return random_names[random_index-1];
    } else {
        return NULL;
    }
}
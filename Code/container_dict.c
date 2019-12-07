#include "container.h"

char* string(char* str){
    if( str == NULL){
        return NULL;
    } else {
        char* temp = (char*)malloc(strlen(str) + 1);
        strcpy(temp, str);
        return temp;
    }
}

char* string_t(int id){
    char* temp = (char*)malloc(32);
    memset(temp, 0, 32);
    sprintf(temp, "_t%d", id);
    return temp;
}

int str_eq(void* str1, void* str2){
    if(strcmp((char*)str1, (char*)str2) == 0){
        return 1;
    } else {
        return 0;
    }
}

void construct_dict(Dict* dict, int (*eq)(void*, void*), void (*print)(void*, void*))
{
    memset(dict, 0, sizeof(Dict));
    dict->head.prev = dict->head.next = &dict->head;
    dict->begin = dict->end = &dict->head;
    dict->eq = eq;
    dict->print = print;
}

void insert_item(Dict* dict, void* key, void* value)
{
    if (find_dict(dict, key) == dict->end) {
        Item* temp = (Item*)malloc(sizeof(Item));
        memset(temp, 0, sizeof(Item));
        temp->key = key;
        temp->value = value;

        DictIter i = dict->end->prev;
        i->next = temp;
        temp->prev = i;
        temp->next = dict->end;
        dict->end->prev = temp;
        dict->size++;
        dict->begin = dict->head.next;
    }
}

DictIter find_dict(Dict* dict, void* key)
{
    if(key == NULL){
        return dict->end;
    }
    for (DictIter i = dict->begin; i != dict->end; i = i->next) {
        if (dict->eq(key, i->key)) {
            return i;
        }
    }
    return dict->end;
}

void remove_item(Dict* dict, void* key)
{
    DictIter i = find_dict(dict, key);
    if (i != dict->end) {
        DictIter prev = i->prev;
        DictIter next = i->next;
        prev->next = next;
        next->prev = prev;
        dict->size--;
        free(i->key);
        free(i->value);
        free(i);
        dict->begin = dict->head.next;
    }
}

void deconstruct_dict(Dict* dict)
{
    DictIter i = dict->begin;
    while (i != dict->end) {
        DictIter temp = i;
        i = i->next;
        free(temp->key);
        free(temp->value);
        free(temp);
    }
    memset(dict, 0, sizeof(Dict));
    dict->head.prev = dict->head.next = &dict->head;
    dict->begin = dict->end = &dict->head;
}

void print_dict(Dict* dict)
{
    if (dict->print == NULL) {
        printf("Provide print function\n");
        return;
    }
    for (DictIter i = dict->begin; i != dict->end; i = i->next) {
        dict->print(i->key, i->value);
    }
}

#include "container.h"

void construct_list(List* list, int (*eq)(void*, void*), void (*print)(void*))
{
    memset(list, 0, sizeof(List));
    list->head.prev = &list->head;
    list->head.next = &list->head;
    list->begin = &list->head;
    list->end = &list->head;
    list->eq = eq;
    list->print = print;
}

void insert_data(List* list, void* data)
{
    Cell* temp = (Cell*)malloc(sizeof(Cell));
    memset(temp, 0, sizeof(Cell));
    temp->data = data;

    ListIter i = list->end->prev;
    i->next = temp;
    temp->prev = i;
    temp->next = list->end;
    list->end->prev = temp;
    list->size++;
    list->begin = list->head.next;
}

ListIter find_list(List* list, void* data)
{
    if (data == NULL) {
        return list->end;
    }
    for (ListIter i = list->begin; i != list->end; i = i->next) {
        if (list->eq(i->data, data)) {
            return i;
        }
    }
    return list->end;
}

void insert_data_u(List* list, void* data)
{
    if (find_list(list, data) == list->end) {
        insert_data(list, data);
    }
}

void remove_data(List* list, void* data)
{
    ListIter i = find_list(list, data);
    if (i != list->end) {
        ListIter prev = i->prev;
        ListIter next = i->next;
        prev->next = next;
        next->prev = prev;
        list->size--;
        free(i->data);
        free(i);
        list->begin = list->head.next;
    }
}

void deconstruct_list(List* list)
{
    ListIter i = list->begin;
    while (i != list->end) {
        ListIter temp = i;
        i = i->next;
        free(temp->data);
        free(temp);
    }
    memset(list, 0, sizeof(List));
    list->head.next = list->head.prev = &list->head;
    list->begin = list->end = &list->head;
}

void print_list(List* list)
{
    if (list->print == NULL) {
        printf("Provide print function\n");
        return;
    }
    for (ListIter i = list->begin; i != list->end; i = i->next) {
        list->print(i->data);
    }
}
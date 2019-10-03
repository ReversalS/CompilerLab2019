#include "symbol_table.h"

// 以下是符号表部分

struct symbol_table Symbol_table;

// 初始化
void init_st()
{
    memset(Symbol_table.table, 0, MAX_ST_ENTRY * sizeof(struct item));
    Symbol_table.index = 0;
    // printf("Init symbol table.");
}

// 添加条目，返回 index，若表中已有则返回已有条目的 index
// TODO: 后期考虑调整为搜索更有效率的结构，以及添加 scope
int add_entry(char* id, int first_line, int first_column)
{
    int i = 0;
    for (i = 0; i < Symbol_table.index; i++) {
        if (strcmp(Symbol_table.table[i].id, id) == 0) {
            return i;
        }
    }
    strcpy(Symbol_table.table[i].id, id);
    Symbol_table.table[i].first_column = first_column;
    Symbol_table.table[i].first_line = first_line;
    Symbol_table.index++;
    return i;
}

// TODO: 鲁棒性，错误处理
// 根据 index 将标识符复制到 dest
void get_id(int index, char* dest)
{
    if (index < Symbol_table.index) {
        strcpy(dest, Symbol_table.table[index].id);
    } else {
        dest = NULL;
    }
}

// 根据 index 返回首次出现的行
int get_line(int index)
{
    if (index < Symbol_table.index) {
        return Symbol_table.table[index].first_line;
    } else {
        return -1;
    }
}

// 根据 index 返回首次出现的列
int get_column(int index)
{
    if (index < Symbol_table.index) {
        return Symbol_table.table[index].first_column;
    } else {
        return -1;
    }
}
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ST_ENTRY 256
#define MAX_ID_LENGTH 32

// 表项，包含了首次出现位置以及标识符的内容
struct item {
    int first_line;
    int first_column;
    char id[MAX_ID_LENGTH];
};

// 表。index用于记录最大下标+1
struct symbol_table {
    struct item table[MAX_ST_ENTRY];
    int index;
};

extern struct symbol_table Symbol_table;

// 初始化
void init_st();
// 添加条目，返回 index，若表中已有则返回已有条目的 index
int add_entry(char* id, int first_line, int first_column);
// 根据 index 将标识符复制到 dest
void get_id(int index, char* dest);
// 根据 index 返回首次出现的行
int get_line(int index);
// 根据 index 返回首次出现的列
int get_column(int index);

#endif
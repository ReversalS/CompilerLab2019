#include "symbol_table.h"

#ifdef USE_RBTREE
/* when encounters '{' */
// int pushSymbolStack() {
//     symbol_stack[stackTop] = RB_ROOT;
//     stackTop++;
// }

// void destructTree(struct rb_root root);
// /* when encounters '}' */
// int popSymbolStack() {
//     stackTop--;
//     destructTree(symbol_stack[stackTop]);
//     RB_EMPTY_ROOT(&symbol_stack[stackTop]);   //actuall {NULL, }
// }
#else

void initSymbolTable() {
    symbolCount = 0;
    stackTop = 0;
    memset((void*)symbol_table, 0, sizeof(symbol_table));
    memset((void*)symbol_stack, 0, sizeof(symbol_stack));
    pushSymbolStack();    //add it here or do it manually outside.
}

void printSymbolStack() {
    printf("---------------\n");
    for (int i = 0; i < stackTop; i++) {
        printList(&symbol_stack[i]);
    }
    printf("---------------\n");
}

int pushSymbolStack() {
    if (stackTop >= STACK_DEPTH)    //overflow
        return -1;
    constructList(&symbol_stack[stackTop]);
    stackTop++;
    return 0;
}

int popSymbolStack() {
    stackTop--;
    if (stackTop < 0)   //underflow
        return -1;
    int count = destructList(&symbol_stack[stackTop]);
    symbolCount -= count;   //release temporary symbols' memory, since they are always added later to the symbol table like a stack
    return 0;
}

/* recursively search symbol stack */
Symbol* getSymbol(char* name) {
    //search data structure and get id
    int stackPointer = stackTop - 1;
    int id = -1;
    while (id == -1 && stackPointer >= 0) {
        int id = searchNode(&symbol_stack[stackPointer], name);
        stackPointer--;
    }
    return &symbol_table[id];
}

/* - First find if there exists previous definitions.
* Since in a give stack frame there should be only one definition,
* when errors happen is quite obivious.
* - Additionally, global variable and local variable can be distinguished
* by current stack depth.
 */
int insertSymbol(char* name, int type_id, int lineno, int column, int attribute_id) {
    FILL_SYMBOL(symbol_table[symbolCount], name, type_id, lineno, column, attribute_id) 
    //insert into data structure
    int ret = insertNode(&symbol_stack[stackTop-1], name, symbolCount);
    if (ret == -1)
        return -1;
    else    //now actually allocate symbol table item
        symbolCount++;
    return 0;
}

#endif
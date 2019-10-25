#include "symbol_table.h"

void initSymbolTable() {
    symbolCount = 0;
    stackTop = 0;
    memset((void*)symbol_table, 0, sizeof(symbol_table));
    memset((void*)symbol_stack, 0, sizeof(symbol_stack));
    pushSymbolStack();    //add it here or do it manually outside.
}

#ifdef USE_RBTREE

void constructRBTree(struct rb_root* root) {
    root->rb_node = NULL;
    //or RB_EMPTY_ROOT(root)
}

erb_node *searchNode(struct rb_root* root, char* key) {
    struct rb_node *rbnode = root->rb_node;
    while (rbnode != NULL) {
        erb_node* enode = container_of(rbnode, erb_node, rb_node);
        int compare_result = strcmp(key, enode->key);
        if (compare_result < 0)
            rbnode = rbnode->rb_left;
        else if (compare_result > 0)
            rbnode = rbnode->rb_right;
        else
            return enode;
    }
    return NULL;
}

int insertNode(struct rb_root* root, char* key, int index) {
    erb_node* enode;    //new node
    struct rb_node** tnode = &(root->rb_node);
    struct rb_node* parent = NULL;
    while (*tnode) {
        erb_node* onode = container_of(*tnode, erb_node, rb_node);
        int compare_result = strcmp(key, onode->key);
        if (compare_result < 0)
            tnode = &((*tnode)->rb_left);
        else if (compare_result > 0)
            tnode = &((*tnode)->rb_right);
        else
            return -1;
    }

    if ((enode=malloc(sizeof(erb_node))) == NULL)
        return -1;
    strncpy(enode->key, key, 32);
    truncate31(enode->key); //prevent overflow
    enode->index = index;

    rb_link_node(&enode->rb_node, parent, tnode);
    rb_insert_color(&enode->rb_node, root);

    return 0;
}

void deleteNode(struct rb_root* root, char* key) {
    erb_node* enode = searchNode(root, key);
    if (enode) {
        rb_erase(&enode->rb_node, root);
        myfree(enode);
    }
}

static void printSubTree(struct rb_node* node) {
    if (node->rb_left)
        printSubTree(node->rb_left);
    printf(":%s:", container_of(node, erb_node, rb_node)->key);
    if (node->rb_right)
        printSubTree(node->rb_right);
}

/* in-order traverse, namely sorted keys */
void printRBTree(struct rb_root* root) {
    printSubTree(root->rb_node);
    printf("\n");
}

static void deconstructSubTree(struct rb_node* node) {
    if (node->rb_left)
        deconstructSubTree(node->rb_left);
    if (node->rb_right)
        deconstructSubTree(node->rb_right);
    myfree(container_of(node, erb_node, rb_node));
}

void destructRBTree(struct rb_root* root) {
    //note that root cannot be freed (sad)
    if (root->rb_node->rb_left)
        deconstructSubTree(root->rb_node->rb_left);
    if (root->rb_node->rb_right)
        deconstructSubTree(root->rb_node->rb_right);
}

/* when encounters '{' */
int pushSymbolStack() {
    if (stackTop >= STACK_DEPTH)    //overflow
        return -1;
    constructRBTree(&symbol_stack[stackTop].root);
    symbol_stack[stackTop].nodeCount = 0;
    stackTop++;
    return 0;
}

/* when encounters '}' */
int popSymbolStack() {
    stackTop--;
    if (stackTop < 0)
        return -1;
    destructRBTree(&symbol_stack[stackTop].root);
    RB_EMPTY_ROOT(&symbol_stack[stackTop].root);   //actuall {NULL, }
    symbolCount -= symbol_stack[stackTop].nodeCount;
    symbol_stack[stackTop].nodeCount = 0;
}

void printSymbolStack() {
    printf("---------------------------------\n");
    for (int i = 0; i < stackTop; i++)
        printRBTree(&symbol_stack[i].root);
    printf("---------------------------------\n");
}

Symbol* getSymbol(char* name) {
    //search data structure and get id
    int stackPointer = stackTop - 1;
    int id = -1;
    while (id == -1 && stackPointer >= 0) {
        int id = searchNode(&symbol_stack[stackPointer].root, name)->index;
        stackPointer--;
    }
    return &symbol_table[id];
}

int insertSymbol(char* name, int type_id, int lineno, int column, int attribute_id) {
    FILL_SYMBOL(symbol_table[symbolCount], name, type_id, lineno, column, attribute_id) 
    int ret = insertNode(&(symbol_stack[stackTop-1].root), name, symbolCount);
    if (ret == -1)
        return -1;
    else
        symbolCount++;
    return 0;
}

#else

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
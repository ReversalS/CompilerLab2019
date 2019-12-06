#include "compile.h"

void compile(Code* code)
{
    enum { INIT,
        FUNC,
        PARAM } state;
    ListNode* p = code->start;
    Code c;
    while (p != NULL && p->prev != code->end) {
        switch (state) {
        case INIT:
            if (p->ic->kind == FUNC_DEF_ST) {
                state = FUNC;
            }
            break;
        case FUNC:
            if (p->ic->kind != PARAM_ST) {
                c.start = p;
                state = PARAM;
            }
            break;
        case PARAM:
            if (p->next == NULL || p->next->ic->kind == FUNC_DEF_ST) {
                c.end = p;
                BBList temp = split(&c);
                state = INIT;
            }
            break;
        }
        p = p->next;
    }
}

void init(char* filename)
{
    fp = fopen(filename, "w");
    if (!fp) {
        perror(filename);
    }
    fprintf(fp, ".data\n");
    fprintf(fp, "_prompt: .asciiz \"Enter an integer: \"\n");
    fprintf(fp, "_ret: asciiz \"\\n\"\n");
    fprintf(fp, ".global main\n");
    fprintf(fp, ".text\n");
    fprintf(fp, "read:\n");
    fprintf(fp, "\tli $v0, 4\n");
    fprintf(fp, "\tla $a0, _prompt\n");
    fprintf(fp, "\tsyscall\n");
    fprintf(fp, "\tli $v0, 5\n");
    fprintf(fp, "\tsyscall\n");
    fprintf(fp, "\tjr $ra\n");
    fprintf(fp, "write:\n");
    fprintf(fp, "\tli $v0, 1\n");
    fprintf(fp, "\tsyscall\n");
    fprintf(fp, "\tli $v0, 4\n");
    fprintf(fp, "\tla $a0, _ret\n");
    fprintf(fp, "\tsyscall\n");
    fprintf(fp, "\tmove $v0, $0\n");
    fprintf(fp, "\tjr $ra\n");
}

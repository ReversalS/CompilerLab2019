#include "compile.h"

void compile(Code* code)
{
    enum { INIT,
        FUNC } state;
    state = INIT;
    ListNode* p = code->start;
    Code c;
    while (p != NULL && p->prev != code->end) {
        switch (state) {
        case INIT:
            if (p->ic->kind == FUNC_DEF_ST) {
                c.start = p;
                state = FUNC;
            }
            break;
        case FUNC:
            if (p->next == NULL || p->next->ic->kind == FUNC_DEF_ST) {
                c.end = p;
                compile_func(&c);
                state = INIT;
            }
            break;
        }
        p = p->next;
    }
}

void compile_func(Code* code)
{
    // TODO func desc
    ListNode* p = code->start;
    Code temp;
    temp.end = code->end;
    int flag = 0;
    while (p != NULL & p->prev != code->end) {
        switch (p->ic->kind) {
        case FUNC_DEF_ST:
            // TODO
            printf("func name: %s\n", p->ic->code.func_name);
            break;
        case PARAM_ST:
            // TODO
            printf("param: %s\n", p->ic->op1->u.var_name);
            break;
        default:
            flag = 1;
            temp.start = p;
            break;
        }
        if (flag) {
            break;
        }
        p = p->next;
    }
    BBList bblist = split(&temp);
    while (bblist != NULL) {
        printf("CODE:\n");
        print_code(stdout, &bblist->code);
        printf("USAGE:\n");
        init_var_info(&bblist->code, &bblist->var_info, &bblist->ic_num);
        printf("******\n");
        bblist = bblist->next;
    }
    printf("==========\n");
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

void emit_lw(MIPS_REG rt, MIPS_REG base, int offset)
{
    fprintf(fp, "\tlw %s, %d(%s)\n", reg_name[rt], offset, reg_name[base]);
}

void emit_sw(MIPS_REG rt, MIPS_REG base, int offset)
{
    fprintf(fp, "\tsw %s, %d(%s)\n", reg_name[rt], offset, reg_name[base]);
}
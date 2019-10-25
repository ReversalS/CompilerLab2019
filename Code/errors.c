#include "errors.h"
#include <stdio.h>

void print_error(int type, int lineno, char* opt_msg, char* opt_msg_2)
{
    if (type < 1 || type > 17) {
        printf("Unknown error.\n");
        return;
    }
    printf("Error type %d at Line %d: ", type, lineno);
    switch (type) {
    case 1:
        printf("Undefined variable \"%s\".\n", opt_msg);
        break;
    case 2:
        printf("Undefined function \"%s\".\n", opt_msg);
        break;
    case 3:
        printf("Redefined variable \"%s\".\n", opt_msg);
        break;
    case 4:
        printf("Redefined function \"%s\".\n", opt_msg);
        break;
    case 5:
        printf("Type mismatched for assignment.\n");
        break;
    case 6:
        printf("The left-hand side of an assignment must be a variable.\n");
        break;
    case 7:
        printf("Type mismatched for operands.\n");
        break;
    case 8:
        printf("Type mismatched for return.\n");
        break;
    case 9:
        printf("Function \"%s\" is not applicable for arguments \"%s\".\n", opt_msg, opt_msg_2);
        break;
    case 10:
        printf("\"%s\" is not an array.\n", opt_msg);
        break;
    case 11:
        printf("\"%s\" is not a function.\n", opt_msg);
        break;
    case 12:
        printf("\"%s\" is not an integer.\n", opt_msg);
        break;
    case 13:
        printf("Illegal use of \"%s\".\n", opt_msg);
        break;
    case 14:
        printf("Non-existent field \"%s\".\n", opt_msg);
        break;
    case 15:
        printf("Redefined field \"%s\".\n", opt_msg);
        break;
    case 16:
        printf("Duplicated name \"%s\".\n", opt_msg);
        break;
    case 17:
        printf("UNdefined structure \"%s\".\n", opt_msg);
        break;
    }
    return;
}
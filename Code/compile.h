#ifndef COMPILE_H
#define COMPILE_H

#include "basic_block.h"

FILE* fp;

void compile(Code* code);
void compile_func(Code* code);

#endif
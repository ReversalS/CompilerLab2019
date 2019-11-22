#ifndef TRANS_PROGRAM_H
#define TRANS_PROGRAM_H

/********** include and macro **********/
#include "trans_stmt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********** function definition **********/
void translate_Program(Node* root);
void translate_ExtDefList(Node* root);
void translate_ExtDef(Node* root);
void translate_ExtDef_func(Node* root);
void translate_FunDec(Node* root);
void translate_StmtList(Node* root);
void translate_DefList(Node* root);
void translate_Def(Node* root);
void translate_DecList(Node* root);
void translate_Dec(Node* root);

#endif
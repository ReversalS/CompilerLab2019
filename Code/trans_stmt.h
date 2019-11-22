#ifndef TRANS_STMT_H
#define TRANS_STMT_H

/********** include and macro **********/
#include "trans_exp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********** function definition **********/
void translate_Stmt(Node* root);
void translate_Stmt_comp(Node* root);
void translate_Stmt_if(Node* root);
void translate_Stmt_ifel(Node* root);
void translate_Stmt_while(Node* root);
#endif
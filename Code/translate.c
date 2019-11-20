#include "translate.h"

int get_elem_size(int type)
{
    int size = 1;
    int temp = value(type).array.elem;
    while (kind(temp) == ARRAY) {
        size *= value(temp).array.size;
        temp = value(temp).array.elem;
    }
    size *= 4;
    return size;
}

RelopType get_relop(enum RELOP relop)
{
    switch (relop) {
    case GT_E:
        return GT_IC;
        break;
    case GE_E:
        return GE_IC;
        break;
    case EQ_E:
        return EQ_IC;
        break;
    case NE_E:
        return NE_IC;
        break;
    case LE_E:
        return LE_IC;
        break;
    case LT_E:
        return LT_IC;
        break;
    }
}

void translate_Exp(Node* root, Operand* op)
{
    switch (root->body.exp) {
    case EXP_ASSIGN:
        translate_Exp_assign(root, op);
        break;
    case EXP_LOGIC:
        translate_Exp_logic(root, op);
        break;
    case EXP_ARITH:
        translate_Exp_arith(root, op);
        break;
    case EXP_LP:
        translate_Exp(root->children[1], op);
        root->code = root->children[1]->code;
        break;
    case EXP_MINUS:
        translate_Exp_minus(root, op);
        break;
    case EXP_FUNC:
        translate_Exp_func(root, op);
        break;
    case EXP_ARRAY:
        translate_Exp_array(root, op);
        break;
    case EXP_STRUCT:
        // nothing
        break;
    case EXP_BASIC:
        translate_Exp_basic(root, op);
        break;
    }
}

void translate_Exp_assign(Node* root, Operand* op)
{
    Operand* left;
    Operand* right = create_op(TEMP, NULL);
    translate_Exp(root->children[2], right);
    switch (root->children[0]->body.exp) {
    case EXP_BASIC:
        left = create_op(VAR, root->children[0]->attr.id);
        if (root->children[2]->body.exp == EXP_ARRAY) {
            append_ic(&root->code, &root->children[2]->code, create_list_node(create_assign(DEREF_RIGHT, left, right)));
        } else {
            append_ic(&root->code, &root->children[2]->code, create_list_node(create_assign(NORMAL, left, right)));
        }
        break;
    case EXP_ARRAY:
        left = create_op(TEMP, NULL);
        translate_Exp(root->children[0], left);
        Code code2;
        concatenate_code(&code2, &root->children[0]->code, &root->children[2]->code);
        if (root->children[2]->body.exp == EXP_ARRAY) {
            Operand* temp = create_op(TEMP, NULL);
            append_ic(&root->code, &code2, create_list_node(create_assign(DEREF_RIGHT, temp, right)));
            append_ic(&root->code, &root->code, create_list_node(create_assign(DEREF_LEFT, left, temp)));
        } else {
            append_ic(&root->code, &code2, create_list_node(create_assign(DEREF_LEFT, left, right)));
        }
        break;
    case EXP_STRUCT:
        // nothing
        break;
    }
    if (op != NULL) {
        if (root->children[2]->body.exp == EXP_ARRAY) {
            append_ic(&root->code, &root->code, create_list_node(create_assign(DEREF_RIGHT, op, right)));
        } else {
            append_ic(&root->code, &root->code, create_list_node(create_assign(NORMAL, op, right)));
        }
    }
}

void translate_Exp_logic(Node* root, Operand* op)
{
    if (op != NULL) {
        InterCode* label_true = create_label();
        InterCode* label_false = create_label();
        translate_Cond(root, label_true, label_false);
        insert_ic(&root->code, &root->code, create_list_node(create_assign(NORMAL, op, create_const(0))));
        append_ic(&root->code, &root->code, create_list_node(label_true));
        append_ic(&root->code, &root->code, create_list_node(create_assign(NORMAL, op, create_const(1))));
        append_ic(&root->code, &root->code, create_list_node(label_false));
    }
}

void translate_Exp_arith(Node* root, Operand* op)
{
    if (op != NULL) {
        Operand* op1 = create_op(TEMP, NULL);
        Operand* op2 = create_op(TEMP, NULL);
        translate_Exp(root->children[0], op1);
        translate_Exp(root->children[2], op2);
        concatenate_code(&root->code, &root->children[0]->code, &root->children[2]->code);
        InterCode* temp;
        switch (root->children[1]->node_value.other) {
        case PLUS_E:
            temp = create_bin(PLUS_IC, op, op1, op2);
            break;
        case MINUS_E:
            temp = create_bin(MINUS_IC, op, op1, op2);
            break;
        case STAR_E:
            temp = create_bin(STAR_IC, op, op1, op2);
            break;
        case DIV_E:
            temp = create_bin(DIV_IC, op, op1, op2);
            break;
        }
        append_ic(&root->code, &root->code, create_list_node(temp));
    }
}

void translate_Exp_minus(Node* root, Operand* op)
{
    int val;
    char* name;
    if (op != NULL) {
        if (root->children[1]->body.exp = EXP_BASIC) {
            switch (root->children[1]->children[0]->node_type.t_type) {
            case INT_E:
                val = root->children[1]->children[0]->node_value.ival;
                root->code.start = root->code.end = create_list_node(create_assign(NORMAL, op, create_const(-val)));
                break;
            case FLOAT_E:
                // nothing
                break;
            case ID_E:
                name = root->children[1]->attr.id;
                root->code.start = root->code.end = create_list_node(create_bin(MINUS_IC, op, create_const(0), create_op(VAR, name)));
                break;
            }
        } else {
            Operand* temp = create_op(TEMP, NULL);
            translate_Exp(root->children[1], temp);
            append_ic(&root->code, &root->children[1]->code, create_list_node(create_bin(MINUS_IC, op, create_const(0), temp)));
        }
    }
}

void translate_Exp_func(Node* root, Operand* op)
{
    if(root->child_num == 3){
        if(strcmp(root->children[0]->node_value.id, "read") == 0){
            root->code.start = root->code.end = create_list_node(create_read(op));
        } else {
            root->code.start = root->code.end = create_list_node(create_call_func(op, root->children[0]->node_value.id));
        }
    } else {
        translate_Args(root->children[2]);
        if(strcmp(root->children[0]->node_value.id, "write") == 0){
            append_ic(&root->code, &root->children[2]->code, create_list_node(create_write((Operand*)root->children[2]->attr.opt_array->attr.array_size)));
        } else {
            AttrList* p = root->children[2]->attr.opt_array;
            root->code = root->children[2]->code;
            while(p != NULL){
                append_ic(&root->code, &root->code, create_list_node(create_arg((Operand*)p->attr.array_size)));
                p = p->next;
            }
            append_ic(&root->code, &root->code, create_list_node(create_call_func(op, root->children[0]->node_value.id)));
        }
    }
}

void translate_Exp_array(Node* root, Operand* op)
{
    if (op == NULL) {
        return;
    }
    if (root->children[0]->body.exp == EXP_BASIC) {
        Operand* base = create_op(TEMP, NULL);
        Operand* offset = create_op(TEMP, NULL);
        Operand* id = create_op(VAR, root->children[0]->attr.id);
        Operand* size = create_const(get_elem_size(root->children[0]->attr.type_id));
        Operand* temp_result = create_op(TEMP, NULL);
        translate_Exp(root->children[2], offset);
        Code temp;
        insert_ic(&temp, &root->children[2]->code, create_list_node(create_assign(GET_ADDR, base, id)));
        append_ic(&root->code, &temp, create_list_node(create_bin(STAR_IC, temp_result, offset, size)));
        append_ic(&root->code, &root->code, create_list_node(create_bin(PLUS_IC, op, base, temp_result)));
    } else {
        Operand* base = create_op(TEMP, NULL);
        Operand* offset = create_op(TEMP, NULL);
        Operand* size = create_const(get_elem_size(root->children[0]->attr.type_id));
        Operand* temp_result = create_op(TEMP, NULL);
        translate_Exp_array(root->children[0], base);
        translate_Exp(root->children[2], offset);
        concatenate_code(&root->code, &root->children[0]->code, &root->children[2]->code);
        append_ic(&root->code, &root->code, create_list_node(create_bin(STAR_IC, temp_result, offset, size)));
        append_ic(&root->code, &root->code, create_list_node(create_bin(PLUS_IC, op, base, temp_result)));
    }
}

void translate_Exp_basic(Node* root, Operand* op)
{
    if (op != NULL) {
        Operand* right;
        switch (root->children[0]->node_type.t_type) {
        case INT_E:
            right = create_const(root->children[0]->node_value.ival);
            break;
        case FLOAT_E:
            // nothing
            break;
        case ID_E:
            right = create_op(VAR, root->children[0]->node_value.id);
            break;
        }
        root->code.start = root->code.end = create_list_node(create_assign(NORMAL, op, right));
    }
}

void translate_Args(Node* root){
    if(root->child_num == 1){
        Operand* temp = create_op(TEMP, NULL);
        translate_Exp(root->children[0], temp);
        root->code = root->children[0]->code;
        insert(&root->attr.opt_array, ARRAY_SIZE, (void*)temp);
    } else {
        Operand* temp = create_op(TEMP, NULL);
        translate_Exp(root->children[0], temp);
        translate_Args(root->children[2]);
        copy_attrlist(&root->attr.opt_array, root->children[2]->attr.opt_array);
        append(&root->attr.opt_array, ARRAY_SIZE, (void*)temp);
        concatenate_code(&root->code, &root->children[0]->code, &root->children[2]->code);
    }
}

void translate_Stmt(Node* root)
{
    Operand* ret;
    switch (root->body.stmt) {
    case STMT_EXP:
        translate_Exp(root->children[0], NULL);
        root->code = root->children[0]->code;
        break;
    case STMT_COMP:
        // TODO
        break;
    case STMT_RT:
        ret = create_op(TEMP, NULL);
        translate_Exp(root->children[1], ret);
        append_ic(&root->code, &root->children[1]->code, create_list_node(create_return(ret)));
        break;
    case STMT_IF:
        translate_Stmt_if(root);
        break;
    case STMT_IFEL:
        translate_Stmt_ifel(root);
        break;
    case STMT_WHILE:
        translate_Stmt_while(root);
        break;
    }
}

void translate_Stmt_if(Node* root){
    InterCode* label_true = create_label();
    InterCode* label_false = create_label();
    translate_Cond(root->children[2], label_true, label_false);
    translate_Stmt(root->children[4]);
    append_ic(&root->code, &root->children[2]->code, create_list_node(label_true));
    concatenate_code(&root->code, &root->code, &root->children[4]->code);
    append_ic(&root->code, &root->code, create_list_node(label_false));
}

void translate_Stmt_ifel(Node* root){
    InterCode* l1 = create_label();
    InterCode* l2 = create_label();
    InterCode* l3 = create_label();
    translate_Cond(root->children[2], l1, l2);
    translate_Stmt(root->children[4]);
    translate_Stmt(root->children[6]);
    append_ic(&root->children[2]->code, &root->children[2]->code, create_list_node(l1));
    append_ic(&root->children[4]->code, &root->children[4]->code, create_list_node(create_goto(l3->code.label_id)));
    append_ic(&root->children[6]->code, &root->children[6]->code, create_list_node(l3));
    concatenate_code(&root->code, &root->children[2]->code, &root->children[4]->code);
    append_ic(&root->code, &root->code, create_list_node(l2));
    concatenate_code(&root->code, &root->code,&root->children[6]->code);
}

void translate_Stmt_while(Node* root){
    InterCode* l1 = create_label();
    InterCode* l2 = create_label();
    InterCode* l3 = create_label();
    translate_Cond(root->children[2], l2, l3);
    translate_Stmt(root->children[4]);
    insert_ic(&root->children[2]->code, &root->children[2]->code, create_list_node(l1));
    insert_ic(&root->children[4]->code, &root->children[4]->code, create_list_node(l2));
    concatenate_code(&root->code,&root->children[2]->code, &root->children[4]->code);
    append_ic(&root->code, &root->code, create_list_node(create_goto(l1->code.label_id)));
    append_ic(&root->code, &root->code, create_list_node(l3));
}

void translate_Cond(Node* exp, InterCode* label_true, InterCode* label_false)
{
    if (exp->child_num == 2 && is_not(exp->children[0])) {
        translate_Cond(exp->children[1], label_false, label_true);
        exp->code = exp->children[1]->code;
    } else if (exp->child_num == 3 && is_relop(exp->children[1])) {
        Operand* left = create_op(TEMP, NULL);
        Operand* right = create_op(TEMP, NULL);
        translate_Exp(exp->children[0], left);
        translate_Exp(exp->children[2], right);
        concatenate_code(&exp->code, &exp->children[0]->code, &exp->children[2]->code);
        append_ic(&exp->code, &exp->code, create_list_node(create_if(get_relop(exp->children[1]->node_value.relop), left, right, label_true->code.label_id)));
        append_ic(&exp->code, &exp->code, create_list_node(create_goto(label_false->code.label_id)));
    } else if (exp->child_num == 3 && is_and(exp->children[1])) {
        InterCode* label = create_label();
        translate_Cond(exp->children[0], label, label_false);
        translate_Cond(exp->children[2], label_true, label_false);
        append_ic(&exp->code, &exp->children[0]->code, create_list_node(label));
        concatenate_code(&exp->code, &exp->code, &exp->children[2]->code);
    } else if (exp->child_num == 3 && is_or(exp->children[1])) {
        InterCode* label = create_label();
        translate_Cond(exp->children[0], label_true, label);
        translate_Cond(exp->children[2], label_true, label_false);
        append_ic(&exp->code, &exp->children[0]->code, create_list_node(label));
        concatenate_code(&exp->code, &exp->code, &exp->children[2]->code);
    } else {
        Operand* temp = create_op(TEMP, NULL);
        translate_Exp(exp, temp);
        append_ic(&exp->code, &exp->code, create_list_node(create_if(NE_IC, temp, create_const(0), label_true->code.label_id)));
        append_ic(&exp->code, &exp->code, create_list_node(create_goto(label_false->code.label_id)));
    }
}
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
        append_ic(&root->code, &root->children[2]->code, create_list_node(create_assign(NORMAL, left, right)));
        break;
    case EXP_ARRAY:
        left = create_op(TEMP, NULL);
        translate_Exp(root->children[0], left);
        Code code2;
        concatenate_code(&code2, &root->children[0]->code, &root->children[2]->code);
        append_ic(&root->code, &code2, create_list_node(create_assign(DEREF_LEFT, left, right)));
        break;
    case EXP_STRUCT:
        // nothing
        break;
    }
    if (op != NULL) {
        append_ic(&root->code, &root->code, create_list_node(create_assign(NORMAL, op, right)));
    }
}

void translate_Exp_logic(Node* root, Operand* op)
{
    // TODO
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
    // TODO
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
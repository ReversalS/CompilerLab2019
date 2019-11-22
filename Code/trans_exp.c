#include "trans_exp.h"

Operand* translate_Exp(Node* root, int need_result)
{
    Operand* temp;
    switch (root->body.exp) {
    case EXP_ASSIGN:
        return translate_Exp_assign(root);
        break;
    case EXP_LOGIC:
        if (need_result) {
            return translate_Exp_logic(root);
        } else {
            return NULL;
        }
        break;
    case EXP_ARITH:
        if (need_result) {
            return translate_Exp_arith(root);
        } else {
            return NULL;
        }
        break;
    case EXP_LP:
        temp = translate_Exp(root->children[1], need_result);
        root->code = root->children[1]->code;
        return temp;
        break;
    case EXP_MINUS:
        if (need_result) {
            return translate_Exp_minus(root);
        } else {
            return NULL;
        }
        break;
    case EXP_FUNC:
        return translate_Exp_func(root);
        break;
    case EXP_ARRAY:
        if (need_result) {
            return translate_Exp_array(root);
        } else {
            return NULL;
        }
        break;
    case EXP_STRUCT:
        CANNOT_TRANSLATE;
        exit(-1);
        break;
    case EXP_BASIC:
        if (need_result) {
            return translate_Exp_basic(root);
        } else {
            return NULL;
        }
        break;
    }
}

Operand* translate_Exp_assign(Node* root)
{
    Operand* left = translate_Exp(root->children[0], 1);
    Operand* right = translate_Exp(root->children[2], 1);
    CONCAT(root, root->children[0], root->children[2]);
    InterCode* ic = create_assign(NORMAL, left, right);
    ic->is_addr.op1 = left->is_addr;
    ic->is_addr.op2 = right->is_addr;
    APPEND1(root, create_list_node(ic));
    return right;
}

Operand* translate_Exp_logic(Node* root)
{
    Operand* temp = create_op(TEMP, NULL, 0);
    InterCode* label_true = create_label();
    InterCode* label_false = create_label();
    int ret = translate_Cond(root, label_true, label_false);
    switch (ret) {
    case 0:
        INSERT1(root, create_list_node(create_assign(NORMAL, temp, create_const(0))));
        APPEND1(root, create_list_node(label_true));
        APPEND1(root, create_list_node(create_assign(NORMAL, temp, create_const(1))));
        APPEND1(root, create_list_node(label_false));
        return temp;
        break;
    case 1:
        deconstruct_op(temp);
        deconstruct_ic(label_true);
        deconstruct_ic(label_false);
        return create_const(1);
        break;
    case -1:
        deconstruct_op(temp);
        deconstruct_ic(label_true);
        deconstruct_ic(label_false);
        return create_const(0);
        break;
    }
}

Operand* translate_Exp_arith(Node* root)
{
    Operand* op1 = translate_Exp(root->children[0], 1);
    Operand* op2 = translate_Exp(root->children[2], 1);
    CONCAT(root, root->children[0], root->children[2]);
    if (can_simplify(op1, op2)) {
        int res = bin_result(op1->u.ival, op2->u.ival, get_bin(root->children[1]->node_value.other));
        deconstruct_op(op1);
        deconstruct_op(op2);
        return create_const(res);
    } else {
        Operand* temp = create_op(TEMP, NULL, 0);
        InterCode* ic = create_bin(get_bin(root->children[1]->node_value.other), temp, op1, op2);
        ic->is_addr.op1 = op1->is_addr;
        ic->is_addr.op2 = op2->is_addr;
        APPEND1(root, create_list_node(ic));
        return temp;
    }
}

Operand* translate_Exp_minus(Node* root)
{
    Operand* temp = translate_Exp(root->children[1], 1);
    root->code = root->children[1]->code;
    if (temp->kind == CONST) {
        int val = temp->u.ival;
        deconstruct_op(temp);
        return create_const(-val);
    } else {
        Operand* temp1 = create_op(TEMP, NULL, 0);
        InterCode* ic = create_bin(MINUS_IC, temp1, create_const(0), temp);
        ic->is_addr.op2 = temp->is_addr;
        APPEND1(root, create_list_node(ic));
        return temp1;
    }
}

Operand* translate_Exp_func(Node* root)
{
    if (root->child_num == 3) {
        Operand* temp = create_op(TEMP, NULL, 0);
        if (strcmp(root->children[0]->node_value.id, "read") == 0) {
            CREATE(root, create_list_node(create_read(temp)));
        } else {
            CREATE(root, create_list_node(create_call_func(temp, root->children[0]->node_value.id)));
        }
        return temp;
    } else {
        translate_Args(root->children[2]);
        if (strcmp(root->children[0]->node_value.id, "write") == 0) {
            Operand* temp =(Operand*)root->children[2]->attr.opt_array->attr.array_size;
            InterCode* ic = create_write(temp);
            if(temp->is_addr && kind(root->children[2]->attr.para_list->attr.para.type) == INT_BASIC){
                ic->is_addr.op1 = 1;
            }
            APPEND2(root, root->children[2], create_list_node(ic));
            return create_const(0);
        } else {
            AttrList* p = root->children[2]->attr.opt_array;
            AttrList* q = root->children[2]->attr.para_list;
            Operand* temp = create_op(TEMP, NULL, 0);
            CREATE(root, create_list_node(create_call_func(temp, root->children[0]->node_value.id)));
            while (p != NULL) {
                Operand* temp = (Operand*)p->attr.array_size;
                InterCode* ic = create_arg(temp);
                if (temp->is_addr && kind(q->attr.para.type) == INT_BASIC) {
                    ic->is_addr.op1 = 1;
                }
                INSERT1(root, create_list_node(ic));
                p = p->next;
                q = q->next;
            }
            CONCAT(root, root->children[2], root);
            return temp;
        }
    }
}

Operand* translate_Exp_array(Node* root)
{
    if (root->body.exp == EXP_BASIC) {
        return create_op(VAR, root->attr.id, 1);
    } else {
        Operand* base = translate_Exp_array(root->children[0]);
        Operand* offset = translate_Exp(root->children[2], 1);
        Operand* size = create_const(get_elem_size(root->children[0]->attr.type_id));
        CONCAT(root, root->children[0], root->children[2]);
        Operand *t1, *t2;
        if (can_simplify(offset, size)) {
            int res = bin_result(offset->u.ival, size->u.ival, STAR_IC);
            deconstruct_op(offset);
            deconstruct_op(size);
            t1 = create_const(res);
        } else {
            t1 = create_op(TEMP, NULL, 0);
            InterCode* ic = create_bin(STAR_IC, t1, offset, size);
            ic->is_addr.op1 = offset->is_addr;
            APPEND1(root, create_list_node(ic));
        }
        t2 = create_op(TEMP, NULL, 1);
        APPEND1(root, create_list_node(create_bin(PLUS_IC, t2, base, t1)));
        return t2;
    }
}

Operand* translate_Exp_basic(Node* root)
{
    switch (root->children[0]->node_type.t_type) {
    case INT_E:
        return create_const(root->attr.val.ival);
        break;
    case FLOAT_E:
        CANNOT_TRANSLATE;
        exit(-1);
        break;
    case ID_E:
        if (kind(root->attr.type_id) == ARRAY) {
            return create_op(VAR, root->attr.id, 1);
        } else {
            return create_op(VAR, root->attr.id, 0);
        }
        break;
    }
}

void translate_Args(Node* root)
{
    if (root->child_num == 1) {
        Operand* temp = translate_Exp(root->children[0], 1);
        root->code = root->children[0]->code;
        insert(&root->attr.opt_array, ARRAY_SIZE, (void*)temp);
    } else {
        Operand* temp = translate_Exp(root->children[0], 1);
        translate_Args(root->children[2]);
        copy_attrlist(&root->attr.opt_array, root->children[2]->attr.opt_array);
        insert(&root->attr.opt_array, ARRAY_SIZE, (void*)temp);
        CONCAT(root, root->children[0], root->children[2]);
    }
}

int translate_Cond(Node* exp, InterCode* label_true, InterCode* label_false)
{
    int ret, ret2;
    if (exp->child_num == 2 && is_not(exp->children[0])) {
        ret = translate_Cond(exp->children[1], label_false, label_true);
        exp->code = exp->children[1]->code;
        return -ret;
    } else if (exp->child_num == 3 && is_relop(exp->children[1])) {
        Operand* left = translate_Exp(exp->children[0], 1);
        Operand* right = translate_Exp(exp->children[2], 1);
        CONCAT(exp, exp->children[0], exp->children[2]);
        if (can_simplify(left, right)) {
            int res = relop_result(left->u.ival, right->u.ival, get_relop(exp->children[1]->node_value.relop));
            deconstruct_op(left);
            deconstruct_op(right);
            deconstruct_code(&exp->code);
            if (res) {
                return 1;
            } else {
                return -1;
            }
        } else {
            InterCode* ic = create_if(get_relop(exp->children[1]->node_value.relop), left, right, label_true->code.label_id);
            ic->is_addr.op1 = left->is_addr;
            ic->is_addr.op2 = right->is_addr;
            APPEND1(exp, create_list_node(ic));
            APPEND1(exp, create_list_node(create_goto(label_false->code.label_id)));
            return 0;
        }
    } else if (exp->child_num == 3 && is_and(exp->children[1])) {
        InterCode* label = create_label();
        ret = translate_Cond(exp->children[0], label, label_false);
        ret2 = translate_Cond(exp->children[2], label_true, label_false);
        if (ret == 1 && ret2 == 1) {
            deconstruct_ic(label);
            return 1;
        } else if (ret == -1 || ret2 == -1) {
            deconstruct_ic(label);
            return -1;
        } else {
            APPEND2(exp, exp->children[0], create_list_node(label));
            CONCAT(exp, exp, exp->children[2]);
            return 0;
        }
    } else if (exp->child_num == 3 && is_or(exp->children[1])) {
        InterCode* label = create_label();
        ret = translate_Cond(exp->children[0], label_true, label);
        ret2 = translate_Cond(exp->children[2], label_true, label_false);
        if (ret == 1 || ret2 == 1) {
            deconstruct_ic(label);
            return 1;
        } else if (ret == -1 && ret2 == -1) {
            deconstruct_ic(label);
            return -1;
        } else {
            APPEND2(exp, exp->children[0], create_list_node(label));
            CONCAT(exp, exp, exp->children[2]);
            return 0;
        }
    } else {
        Operand* temp = translate_Exp(exp, 1);
        if (temp->kind == CONST) {
            if (temp->u.ival) {
                deconstruct_op(temp);
                return 1;
            } else {
                deconstruct_op(temp);
                return -1;
            }
        } else {
            InterCode* ic = create_if(NE_IC, temp, create_const(0), label_true->code.label_id);
            ic->is_addr.op1 = temp->is_addr;
            APPEND1(exp, create_list_node(ic));
            APPEND1(exp, create_list_node(create_goto(label_false->code.label_id)));
            return 0;
        }
    }
}

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

BinType get_bin(enum OTHER type)
{
    switch (type) {
    case PLUS_E:
        return PLUS_IC;
        break;
    case MINUS_E:
        return MINUS_IC;
        break;
    case STAR_E:
        return STAR_IC;
        break;
    case DIV_E:
        return DIV_IC;
        break;
    default:
        perror("IMPOSSIBLE\n");
        abort();
    }
}
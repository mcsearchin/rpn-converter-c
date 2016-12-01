#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rpn-converter.h"

#define null_char '\0'
#define min_char_int_value 97
#define max_char_int_value 122

typedef struct Operation Operation;

typedef struct Side {
    char operand;
    Operation *operation;
} Side;

struct Operation {
    char operator;
    Side *left;
    Side *right;
};

const char supported_operators[] = { '^', '/', '*', '-', '+' };

static char *substring(const char *string, int start, int length) {
    char *substring = malloc(sizeof(length + 1));
    strncpy(substring, &string[start], length);
    substring[length] = null_char;

    return substring;
}

static int get_operator_precedence(const char operator) {
    int index, supported_operators_length = sizeof(supported_operators);
    for (index = 0; index < supported_operators_length; index++) {
        if (supported_operators[index] == operator) {
            return index;
        }
    }

    return -1;
}

static int get_weakest_operator_index(const char *infix) {
    int infix_index, infix_length = strlen(infix);
    int operator_precedence, weakest_operator_precedence = -1, weakest_operator_index = -1;

    for (infix_index = 0; infix_index < infix_length; infix_index++) {
        operator_precedence = get_operator_precedence(infix[infix_index]);
        if (operator_precedence > weakest_operator_precedence) {
            weakest_operator_precedence = operator_precedence;
            weakest_operator_index = infix_index;
        }
    }

    return weakest_operator_index;
}

static bool is_valid_operand(const char operand) {
    return operand >= min_char_int_value && operand <= max_char_int_value;
}

static Side *initialize_operation_side() {
    Side *side = (Side *) malloc(sizeof(Side));
    side->operand = null_char;
    side->operation = NULL;
    return side;
}

static Operation *initialize_operation() {
    Operation *operation = (Operation *) malloc(sizeof(Operation));
    operation->operator = null_char;
    operation->left = initialize_operation_side();
    operation->right = initialize_operation_side();
    return operation;
}

static void clean_up_operation(Operation *operation) {
    if (operation) {
        clean_up_operation(operation->left->operation);
        free(operation->left);
        clean_up_operation(operation->right->operation);
        free(operation->right);
        free(operation);
    }
}

static rpn_conversion_status populate_operation_side_from_infix(const char *infix, Side *side);

static rpn_conversion_status populate_operation_from_infix(const char *infix, Operation *operation) {
    rpn_conversion_status status = SUCCESS;
    int weakest_operator_index = get_weakest_operator_index(infix);

    if (weakest_operator_index < 0) {
        return INVALID_SYNTAX;
    }

    operation->operator = infix[weakest_operator_index];

    char *left_side = substring(infix, 0, weakest_operator_index);
    status = populate_operation_side_from_infix(left_side, operation->left);
    free(left_side);
    if (SUCCESS != status) {
        return status;
    }

    const char *right_side = &infix[weakest_operator_index + 1];
    status = populate_operation_side_from_infix(right_side, operation->right);
    
    return status;
}

static rpn_conversion_status populate_operation_side_from_infix(const char *infix, Side *side) {
    rpn_conversion_status status = SUCCESS;

    if (strlen(infix) > 1) {
        side->operation = initialize_operation();
        status = populate_operation_from_infix(infix, side->operation);
    } else if (is_valid_operand(infix[0])) {
        side->operand = infix[0];
    } else {
        return INVALID_SYNTAX;
    }

    return status;
}

static void set_and_increment(char value, char *string, int *index) {
    string[*index] = value;
    (*index)++;        
}

static void set_and_decrement(char value, char *string, int *index) {
    string[*index] = value;
    (*index)--;      
}

static void populate_rpn_from_operation(const Operation *operation, char *rpn, int *start_index, int *end_index) {
    if (!operation->left->operation && !operation->right->operation) {
        set_and_increment(operation->left->operand, rpn, start_index);
        set_and_increment(operation->right->operand, rpn, start_index);
        set_and_increment(operation->operator, rpn, start_index);
    } else {
        set_and_decrement(operation->operator, rpn, end_index);

        if (operation->left->operation) {
            set_and_decrement(operation->right->operand, rpn, end_index);
            populate_rpn_from_operation(operation->left->operation, rpn, start_index, end_index);
        } else {
            set_and_increment(operation->left->operand, rpn, start_index);
            populate_rpn_from_operation(operation->right->operation, rpn, start_index, end_index);
        }
    }
}

rpn_conversion_status to_rpn(const char *infix, char *rpn) {
    Operation *operation = initialize_operation();
    rpn_conversion_status status = populate_operation_from_infix(infix, operation);

    if (SUCCESS != status) {
        clean_up_operation(operation);
        return status;
    }

    int start_index = 0, end_index = strlen(infix) - 1;
    populate_rpn_from_operation(operation, rpn, &start_index, &end_index);

    clean_up_operation(operation);

    return status;
}

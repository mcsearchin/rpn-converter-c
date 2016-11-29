#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rpn-converter.h"

#define null_char '\0'
#define min_char_int_value 97
#define max_char_int_value 122

typedef struct Operation Operation;
struct Operation {
    char operator;
    char left_operand;
    char right_operand;
    Operation *left_sub_operation;
    Operation *right_sub_operation;
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

static bool is_supported_operator(const char operator) {
    return get_operator_precedence(operator) >= 0;
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

static Operation *initialize_operation() {
    Operation *operation = (Operation *) malloc(sizeof(Operation));
    operation->operator = null_char;
    operation->left_operand = null_char;
    operation->right_operand = null_char;
    operation->left_sub_operation = NULL;
    operation->right_sub_operation = NULL;
    return operation;
}

static void clean_up_operation(Operation *operation) {
    if (operation) {
        clean_up_operation(operation->left_sub_operation);
        clean_up_operation(operation->right_sub_operation);
        free(operation);
    }
}

static rpn_conversion_status populate_operation_from_infix(const char *infix, Operation *operation) {
    int weakest_operator_index = get_weakest_operator_index(infix);

    if (!is_supported_operator(infix[weakest_operator_index])) {
        return INVALID_CHARACTER;
    }

    operation->operator = infix[weakest_operator_index];

    char *left_side = substring(infix, 0, weakest_operator_index);
    if (strlen(left_side) > 1) {
        operation->left_sub_operation = initialize_operation();
        populate_operation_from_infix(left_side, operation->left_sub_operation);
    } else if (is_valid_operand(left_side[0])) {
        operation->left_operand = left_side[0];
    } else {
        return INVALID_CHARACTER;
    }
    free(left_side);

    const char *right_side = &infix[weakest_operator_index + 1];
    if (strlen(right_side) > 1) {
        operation->right_sub_operation = initialize_operation();
        populate_operation_from_infix(right_side, operation->right_sub_operation);
    } else if (is_valid_operand(right_side[0])) {
        operation->right_operand = right_side[0];
    } else {
        return INVALID_CHARACTER;
    }
    
    return SUCCESS;
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
    if (!operation->left_sub_operation && !operation->right_sub_operation) {
        set_and_increment(operation->left_operand, rpn, start_index);
        set_and_increment(operation->right_operand, rpn, start_index);
        set_and_increment(operation->operator, rpn, start_index);
    } else if (operation->left_sub_operation) {
        set_and_decrement(operation->operator, rpn, end_index);
        set_and_decrement(operation->right_operand, rpn, end_index);
        populate_rpn_from_operation(operation->left_sub_operation, rpn, start_index, end_index);
    } else if (operation->right_sub_operation) {
        set_and_decrement(operation->operator, rpn, end_index);
        set_and_increment(operation->left_operand, rpn, start_index);
        populate_rpn_from_operation(operation->right_sub_operation, rpn, start_index, end_index);
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

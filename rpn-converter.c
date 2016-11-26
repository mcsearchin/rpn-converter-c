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

static int get_weakest_operator_precedence(const char *infix) {
    int infix_index, infix_length = strlen(infix);
    int operator_precedence, weakest_operator_precedence = -1;

    for (infix_index = 0; infix_index < infix_length; infix_index++) {
        operator_precedence = get_operator_precedence(infix[infix_index]);
        if (operator_precedence > weakest_operator_precedence) {
            weakest_operator_precedence = operator_precedence;
        }
    }

    return weakest_operator_precedence;
}

static bool is_valid_operand(const char operand) {
    return operand >= min_char_int_value && operand <= max_char_int_value;
}

static void clean_up_operation(Operation *operation) {
    if (operation) {
        clean_up_operation(operation->left_sub_operation);
        clean_up_operation(operation->right_sub_operation);
        free(operation);
    }
}

static Operation *initialize_operation(char operator) {
    Operation *operation = (Operation *) malloc(sizeof(Operation));

    operation->operator = operator;
    operation->left_operand = null_char;
    operation->right_operand = null_char;
    operation->left_sub_operation = NULL;
    operation->right_sub_operation = NULL;
    return operation;
}

static rpn_conversion_status build_operation_from_infix(const char *infix, Operation **operation) {
    // printf("populate operation from : %s\n", infix);
    *operation = initialize_operation(infix[1]);

    (*operation)->left_operand = infix[0];

    const char *right_side = &infix[2];
    // printf("right side : %s\n", right_side);
    if (strlen(right_side) > 1) {
        // Operation right_sub_operation = { null_char, null_char, null_char, NULL, NULL };
        // operation->right_sub_operation = &right_sub_operation;
        build_operation_from_infix(right_side, &(*operation)->right_sub_operation);
        // printf("right side operator : %c, left_operand : %c, right_operand : %c\n", 
        //     right_sub_operation.operator, right_sub_operation.left_operand, right_sub_operation.right_operand);
    } else {
        (*operation)->right_operand = right_side[0];
    }
    
    if (!is_supported_operator((*operation)->operator) || 
        !is_valid_operand((*operation)->left_operand) || 
        (null_char != (*operation)->right_operand && !is_valid_operand((*operation)->right_operand))) {
        return INVALID_CHARACTER;
    }

    return SUCCESS;
}

static void populate_rpn_from_operation(const Operation *operation, char *rpn, int *start_index, int *end_index) {
    rpn[*start_index] = operation->left_operand;
    (*start_index)++;
    // printf("operation.operator : %c, operation.left_operand : %c, operation.right_operand : %c, : rpn %s\n", 
        // operation->operator, operation->left_operand, operation->right_operand, rpn);
    if (NULL != operation->right_sub_operation) {
        // printf("right_sub_operation operator : %c, left_operand : %c\n", 
            // operation->right_sub_operation->operator, operation->right_sub_operation->left_operand);
        populate_rpn_from_operation(operation->right_sub_operation, rpn, start_index, end_index);
    } else {
        rpn[*start_index] = operation->right_operand;
        (*start_index)++;
    }

    rpn[*end_index] = operation->operator;
    (*end_index)--;
}

rpn_conversion_status to_rpn(const char *infix, char *rpn) {
    Operation *operation = NULL;
    rpn_conversion_status status = build_operation_from_infix(infix, &operation);
    // printf("root operation operator : %c, left_operand : %c, right_operand : %c\n", 
    //     operation.operator, operation.left_operand, operation.right_operand);
    if (SUCCESS != status) {
        clean_up_operation(operation);
        return status;
    }

    int start_index = 0, end_index = strlen(infix) - 1;

    populate_rpn_from_operation(operation, rpn, &start_index, &end_index);

    clean_up_operation(operation);

    return status;
}

rpn_conversion_status to_rpn_lame(const char *infix, char *rpn) {
    int infix_index, infix_length = strlen(infix);
    char infix_char;
    int ascending_index = 0, descending_index = infix_length - 1;
    char current_operator = null_char;
    int weakest_operator_precedence = get_weakest_operator_precedence(infix);
    
    for (infix_index = 0; infix_index < infix_length; infix_index++) {
        infix_char = infix[infix_index];

        if (is_valid_operand(infix_char)) {
            rpn[ascending_index] = infix_char;
            ascending_index++;
            if (null_char != current_operator) {
                rpn[ascending_index] = current_operator;
                ascending_index++;
                current_operator = null_char;
            }
        } else if (is_supported_operator(infix_char)) {
            if (get_operator_precedence(infix_char) < weakest_operator_precedence) {
                current_operator = infix_char;
            } else {
                rpn[descending_index] = infix_char;
                descending_index--;
                weakest_operator_precedence = get_weakest_operator_precedence(&infix[infix_index + 1]);
            }
        } else {
            rpn[0] = null_char;
            return INVALID_CHARACTER;
        }

        printf("infix : %s, rpn : %s\n", infix, rpn);
    }

    return SUCCESS;
}
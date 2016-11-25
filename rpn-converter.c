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
    Operation *left_operation;
    Operation *right_operation;
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

rpn_conversion_status to_rpn(const char *infix, char *rpn) {
    Operation operation = { null_char, null_char, null_char, NULL, NULL };
    operation.operator = infix[1];
    operation.left_operand = infix[0];
    operation.right_operand = infix[2];
    if (!is_supported_operator(operation.operator)) {
        return INVALID_CHARACTER;
    }

    rpn[2] = operation.operator;
    rpn[0] = operation.left_operand;
    rpn[1] = operation.right_operand;

    return SUCCESS;
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
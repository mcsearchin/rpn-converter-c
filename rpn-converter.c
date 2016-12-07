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

static bool is_supported_operator(const char operator) {
    return get_operator_precedence(operator) >= 0;
}

static int get_surrounding_parentheses_count(const char *infix) {
    int count = 0;

    int start = 0, end = strlen(infix) - 1;
    bool outer_parentheses_match = false;
    do {
        outer_parentheses_match = '(' == infix[start] && ')' == infix[end];
        if (outer_parentheses_match) {
            count++;
            start++;
            end--;
        }
    } while (outer_parentheses_match);

    if (count > 0) {
        int index;
        for (index = start; index <= end; index++) {
            if (')' == infix[index]) {
                count--;
            }
        }
    }

    return count;
}

static int get_weakest_operator_index_for_infix(const char *infix) {
    int index, weakest_operator_index = -1;
    int operator_precedence, weakest_operator_precedence = -1;
    int surrounding_parentheses_count = get_surrounding_parentheses_count(infix);
    int end_index = strlen(infix) - surrounding_parentheses_count - 1;
    int start_index = surrounding_parentheses_count;

    bool parentheses = false;
    for (index = end_index; index >= start_index; index--) {
        if (')' == infix[index]) {
            parentheses = true;
        } else if ('(' == infix[index]) {
            parentheses = false;
        } else if (!parentheses) {
            operator_precedence = get_operator_precedence(infix[index]);
            if (operator_precedence > weakest_operator_precedence) {
                weakest_operator_precedence = operator_precedence;
                weakest_operator_index = index;
            }
        }
    }

    return weakest_operator_index;
}

static bool contains_operator(const char *string) {
    int index, length = strlen(string);
    for (index = 0; index < length; index++) {
        if (is_supported_operator(string[index])) {
            return true;
        }
    }
    return false;
}

static bool is_valid_operand(const char operand) {
    return operand >= min_char_int_value && operand <= max_char_int_value;
}

static char get_single_operand(const char *string) {
    char operand = null_char;
    bool operand_found = 0;
    int index, length = strlen(string);
    for (index = 0; index < length; index++) {
        if (is_valid_operand(string[index])) {
            if (!operand_found) {
                operand = string[index];
                operand_found = true;
            } else {
                operand = null_char;
                break;
            }
        }
    }

    return operand;
}

static bool infix_contains_invalid_character(const char *infix) {
    int index, length = strlen(infix);
    char character;
    for (index = 0; index < length; index++) {
        character = infix[index];
        if (!is_valid_operand(character) && !is_supported_operator(character) &&
            '(' != character && ')' != character) {
            return true;
        }
    }
    return false;
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
    int weakest_operator_index = get_weakest_operator_index_for_infix(infix);

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

    if (contains_operator(infix)) {
        side->operation = initialize_operation();
        status = populate_operation_from_infix(infix, side->operation);
    } else {
        char operand = get_single_operand(infix);

        if (null_char != operand) {
            side->operand = operand;
        } else {
            status = INVALID_SYNTAX;
        }
    }

    return status;
}

static void set_and_decrement(char value, char *string, int *index) {
    string[*index] = value;
    (*index)--;      
}

static void populate_rpn_from_operation(const Operation *operation, char *rpn, int *end_index) {
    if (!operation->right->operation && !operation->left->operation) {
        set_and_decrement(operation->operator, rpn, end_index);
        set_and_decrement(operation->right->operand, rpn, end_index);
        set_and_decrement(operation->left->operand, rpn, end_index);
    } else {
        set_and_decrement(operation->operator, rpn, end_index);

        if (operation->right->operation && operation->left->operation) {
            populate_rpn_from_operation(operation->right->operation, rpn, end_index);
            populate_rpn_from_operation(operation->left->operation, rpn, end_index);
        } else if (operation->right->operation) {
            populate_rpn_from_operation(operation->right->operation, rpn, end_index);
            set_and_decrement(operation->left->operand, rpn, end_index);
        } else {
            set_and_decrement(operation->right->operand, rpn, end_index);
            populate_rpn_from_operation(operation->left->operation, rpn, end_index);
        }
    }
}

static int get_rpn_length_for_infix(const char *infix) {
    int rpn_length = 0;
    int index, infix_length = strlen(infix);
    for (index = 0; index < infix_length; index++) {
        if (is_valid_operand(infix[index]) || is_supported_operator(infix[index])) {
            rpn_length++;
        }
    }

    return rpn_length;
}

rpn_conversion_status to_rpn(const char *infix, char *rpn) {
    if (infix_contains_invalid_character(infix)) {
        return INVALID_SYNTAX;
    }

    Operation *operation = initialize_operation();
    rpn_conversion_status status = populate_operation_from_infix(infix, operation);

    if (SUCCESS != status) {
        clean_up_operation(operation);
        return status;
    }

    int end_index = get_rpn_length_for_infix(infix) - 1;
    populate_rpn_from_operation(operation, rpn, &end_index);

    clean_up_operation(operation);

    return status;
}

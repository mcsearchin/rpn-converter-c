#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rpn-converter.h"

#define min_char_int_value 97
#define max_char_int_value 122
#define null_char '\0'

const char supported_operators[] = { '+', '-', '*', '/', '^' };

static bool is_supported_operator(const char operator) {
    int index, supported_operators_length = sizeof(supported_operators);
    for (index = 0; index < supported_operators_length; index++) {
        if (supported_operators[index] == operator) {
            return true;
        }
    }

    return false;
}

static bool is_valid_operand(const char operand) {
    return operand >= min_char_int_value && operand <= max_char_int_value;
}

rpn_conversion_status to_rpn(const char *infix, char *rpn) {
    int infix_index, infix_length = strlen(infix);
    char infix_char;
    int ascending_index = 0, descending_index = infix_length - 1;
    char current_operator;
    
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
            if ('-' == infix_char) {
                current_operator = infix_char;
            } else {
                rpn[descending_index] = infix_char;
                descending_index--;
            }
        } else {
            rpn[0] = null_char;
            return INVALID_CHARACTER;
        }
    }

    return SUCCESS;
}
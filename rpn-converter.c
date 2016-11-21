#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rpn-converter.h"

const char supported_operators[] = { '+', '-', '*', '/', '^' };

static bool is_supported_operator(const char operator) {
    int index, supported_operators_length = sizeof(supported_operators);
    for (index = 0; index < supported_operators_length; index++) {
        if (supported_operators[index] == operator) {
            return true;
        }
    }

    fprintf(stderr, "Unsupported operator: '%c'\n", operator);
    return false;
}

rpn_conversion_status to_rpn(const char *infix, char *rpn) {
    if (!is_supported_operator(infix[1])) {
        return INVALID_CHARACTER;
    }
    rpn[0] = infix[0];
    rpn[1] = infix[2];
    rpn[2] = infix[1];
    rpn[3] = '\0';

    return SUCCESS;
}
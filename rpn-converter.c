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
    strcpy(rpn, "ab");
    char operator[2];
    memcpy(operator, &infix[1], 1);
    operator[1] = '\0';
    strcat(rpn, operator);

    return SUCCESS;
}
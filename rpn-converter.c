#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rpn-converter.h"

const char valid_operators[] = { '+', '-', '*', '/' };

static bool is_valid_operator(const char operator) {
    int index, valid_operators_length = sizeof(valid_operators);
    for (index = 0; index < valid_operators_length; index++) {
        if (valid_operators[index] == operator) {
            return true;
        }
    }

    return false;
}

rpn_conversion_status to_rpn(const char *infix, char *rpn) {
    if (!is_valid_operator(infix[1])) {
        return INVALID_CHARACTER;
    }
    strcpy(rpn, "ab");
    char operator[2];
    memcpy(operator, &infix[1], 1);
    operator[1] = '\0';
    strcat(rpn, operator);

    return SUCCESS;
}
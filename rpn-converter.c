#include <stdlib.h>
#include <string.h>

#include "rpn-converter.h"

void to_rpn(const char *infix, char *rpn) {
    strcpy(rpn, "ab");
    char operator[2];
    memcpy(operator, &infix[1], 1);
    operator[1] = '\0';
    strcat(rpn, operator);
}
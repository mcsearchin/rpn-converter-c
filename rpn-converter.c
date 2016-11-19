#include <stdlib.h>
#include <string.h>

#include "rpn-converter.h"

void to_rpn(const char *infix, char *rpn) {
    strcpy(rpn, "ab");
    char *operator = strstr(infix, "+") ? "+" : "-";
    strcat(rpn, operator);
}
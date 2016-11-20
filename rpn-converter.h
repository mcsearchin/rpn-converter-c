#ifndef RPN_CONVERTER
#define RPN_CONVERTER

typedef enum { SUCCESS, INVALID_CHARACTER } rpn_conversion_status;

rpn_conversion_status to_rpn(const char *infix, char *rpn);

#endif

/*
 * DO NOT EDIT THIS FILE. Generated by checkmk.
 * Edit the original source file "rpn-converter-test.check" instead.
 */

#include <check.h>

#line 1 "rpn-converter-test.check"
#include <stdlib.h>

#include "rpn-converter.h"

char *infix;
char *rpn;

void tear_down() {
    free(rpn);
}


START_TEST(can_convert_addition)
{
#line 15
    infix = "a+b";
    rpn = malloc((strlen(infix) + 1) * sizeof(char));

    to_rpn(infix, rpn);

    ck_assert_str_eq(rpn, "ab+");

}
END_TEST

START_TEST(can_convert_subtraction)
{
#line 23
    infix = "a-b";
    rpn = malloc((strlen(infix) + 1) * sizeof(char));

    to_rpn(infix, rpn);

    ck_assert_str_eq(rpn, "ab-");

}
END_TEST

START_TEST(can_convert_multiplication)
{
#line 31
    infix = "a*b";
    rpn = malloc((strlen(infix) + 1) * sizeof(char));

    to_rpn(infix, rpn);

    ck_assert_str_eq(rpn, "ab*");

}
END_TEST

int main(void)
{
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("to_rpn");
    SRunner *sr = srunner_create(s1);
    int nf;

    /* User-specified pre-run code */
#line 39
    tcase_add_checked_fixture(tc1_1, NULL, tear_down);

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, can_convert_addition);
    tcase_add_test(tc1_1, can_convert_subtraction);
    tcase_add_test(tc1_1, can_convert_multiplication);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}

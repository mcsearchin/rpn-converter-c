/*
 * DO NOT EDIT THIS FILE. Generated by checkmk.
 * Edit the original source file "rpn-converter-test.check" instead.
 */

#include <check.h>

#line 1 "rpn-converter-test.check"
#include <stdlib.h>

#include "rpn-converter.h"

START_TEST(can_convert_infix_addition_to_rpn)
{
#line 6
	char *infix = "a+b";
	char *rpn = malloc((strlen(infix) + 1) * sizeof(char));

	to_rpn(infix, rpn);

	ck_assert_str_eq(rpn, "ab+");
}
END_TEST

int main(void)
{
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, can_convert_infix_addition_to_rpn);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}

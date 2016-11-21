/*
 * DO NOT EDIT THIS FILE. Generated by checkmk.
 * Edit the original source file "rpn-converter-test.check" instead.
 */

#include <check.h>

#line 1 "rpn-converter-test.check"
#include "rpn-converter.h"

char rpn_result[100];


START_TEST(can_convert_addition)
{
#line 8
    ck_assert_int_eq(to_rpn("a+b", rpn_result), SUCCESS);

    ck_assert_str_eq(rpn_result, "ab+");

}
END_TEST

START_TEST(can_convert_subtraction)
{
#line 13
    ck_assert_int_eq(to_rpn("a-b", rpn_result), SUCCESS);

    ck_assert_str_eq(rpn_result, "ab-");

}
END_TEST

START_TEST(can_convert_multiplication)
{
#line 18
    ck_assert_int_eq(to_rpn("a*b", rpn_result), SUCCESS);

    ck_assert_str_eq(rpn_result, "ab*");

}
END_TEST

START_TEST(can_convert_division)
{
#line 23
    ck_assert_int_eq(to_rpn("a/b", rpn_result), SUCCESS);

    ck_assert_str_eq(rpn_result, "ab/");

}
END_TEST

START_TEST(can_convert_exponent)
{
#line 28
    ck_assert_int_eq(to_rpn("a^b", rpn_result), SUCCESS);

    ck_assert_str_eq(rpn_result, "ab^");

}
END_TEST

START_TEST(when_an_unsupported_operator_is_passed_it_returns_invalid_character_status)
{
#line 33
    ck_assert_int_eq(to_rpn("a&b", rpn_result), INVALID_CHARACTER);

    ck_assert_str_eq(rpn_result, "");

}
END_TEST

START_TEST(can_convert_with_any_operands_that_are_lowercase_letters)
{
#line 38
    ck_assert_int_eq(to_rpn("a+z", rpn_result), SUCCESS);

    ck_assert_str_eq(rpn_result, "az+");

}
END_TEST

START_TEST(when_a_non_lowercase_letter_second_operand_is_passed_it_returns_invalid_character_status)
{
#line 43
    ck_assert_int_eq(to_rpn("a+{", rpn_result), INVALID_CHARACTER);

    ck_assert_str_eq(rpn_result, "");

}
END_TEST

START_TEST(when_a_non_lowercase_letter_first_operand_is_passed_it_returns_invalid_character_status)
{
#line 48
    ck_assert_int_eq(to_rpn("`-z", rpn_result), INVALID_CHARACTER);

    ck_assert_str_eq(rpn_result, "");
}
END_TEST

int main(void)
{
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("to_rpn");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, can_convert_addition);
    tcase_add_test(tc1_1, can_convert_subtraction);
    tcase_add_test(tc1_1, can_convert_multiplication);
    tcase_add_test(tc1_1, can_convert_division);
    tcase_add_test(tc1_1, can_convert_exponent);
    tcase_add_test(tc1_1, when_an_unsupported_operator_is_passed_it_returns_invalid_character_status);
    tcase_add_test(tc1_1, can_convert_with_any_operands_that_are_lowercase_letters);
    tcase_add_test(tc1_1, when_a_non_lowercase_letter_second_operand_is_passed_it_returns_invalid_character_status);
    tcase_add_test(tc1_1, when_a_non_lowercase_letter_first_operand_is_passed_it_returns_invalid_character_status);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}

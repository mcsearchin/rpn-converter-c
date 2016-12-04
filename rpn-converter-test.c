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

START_TEST(when_an_unsupported_operator_is_passed_it_returns_invalid_syntax_status)
{
#line 33
    ck_assert_int_eq(to_rpn("a&b", rpn_result), INVALID_SYNTAX);

    ck_assert_str_eq(rpn_result, "");

}
END_TEST

START_TEST(when_an_unsupported_operator_is_later_in_the_expression_it_returns_invalid_syntax_status)
{
#line 38
    ck_assert_int_eq(to_rpn("a+b|c", rpn_result), INVALID_SYNTAX);

    ck_assert_str_eq(rpn_result, "");

}
END_TEST

START_TEST(when_an_unsupported_operator_is_first_in_a_complex_expression_it_returns_invalid_syntax_status)
{
#line 43
    ck_assert_int_eq(to_rpn("a%b+c", rpn_result), INVALID_SYNTAX);

    ck_assert_str_eq(rpn_result, "");

}
END_TEST

START_TEST(when_the_expression_is_incomplete_it_returns_invalid_syntax_status)
{
#line 48
    ck_assert_int_eq(to_rpn("a", rpn_result), INVALID_SYNTAX);

    ck_assert_str_eq(rpn_result, "");

}
END_TEST

START_TEST(can_convert_with_any_operands_that_are_lowercase_letters)
{
#line 53
    ck_assert_int_eq(to_rpn("a+z", rpn_result), SUCCESS);

    ck_assert_str_eq(rpn_result, "az+");

}
END_TEST

START_TEST(when_a_non_lowercase_letter_first_operand_is_passed_it_returns_invalid_syntax_status)
{
#line 58
    ck_assert_int_eq(to_rpn("`-z", rpn_result), INVALID_SYNTAX);

    ck_assert_str_eq(rpn_result, "");

}
END_TEST

START_TEST(when_a_non_lowercase_letter_last_operand_is_passed_it_returns_invalid_syntax_status)
{
#line 63
    ck_assert_int_eq(to_rpn("a+{", rpn_result), INVALID_SYNTAX);

    ck_assert_str_eq(rpn_result, "");

}
END_TEST

START_TEST(can_convert_multiple_chained_operations_with_the_same_operator)
{
#line 68
    ck_assert_int_eq(to_rpn("a+b+c+d", rpn_result), SUCCESS);

    ck_assert_str_eq(rpn_result, "ab+c+d+");

}
END_TEST

START_TEST(can_convert_subtraction_then_addition)
{
#line 73
    ck_assert_int_eq(to_rpn("a-b+c", rpn_result), SUCCESS);

    ck_assert_str_eq(rpn_result, "ab-c+");

}
END_TEST

START_TEST(can_convert_subtraction_then_multiplication)
{
#line 78
    ck_assert_int_eq(to_rpn("a-b*c", rpn_result), SUCCESS);

    ck_assert_str_eq(rpn_result, "abc*-");

}
END_TEST

START_TEST(can_convert_addition_then_subtraction_then_multiplication)
{
#line 83
    ck_assert_int_eq(to_rpn("a+b-c*d", rpn_result), SUCCESS);

    ck_assert_str_eq(rpn_result, "abcd*-+");

}
END_TEST

START_TEST(can_convert_addition_then_subtraction_then_addition)
{
#line 88
    ck_assert_int_eq(to_rpn("a+b-c+d", rpn_result), SUCCESS);

    ck_assert_str_eq(rpn_result, "abc-+d+");

}
END_TEST

START_TEST(can_convert_division_then_multiplication_then_exponentiation)
{
#line 93
    ck_assert_int_eq(to_rpn("a/b*c^d", rpn_result), SUCCESS);

    ck_assert_str_eq(rpn_result, "ab/cd^*");

}
END_TEST

START_TEST(can_convert_multiplication_then_division_then_subtraction_then_exponentiation)
{
#line 98
    ck_assert_int_eq(to_rpn("b*c/d-e^f", rpn_result), SUCCESS);

    ck_assert_str_eq(rpn_result, "bcd/*ef^-");

}
END_TEST

START_TEST(can_convert_multiplication_then_subtraction_then_exponentiation_then_division)
{
#line 103
    ck_assert_int_eq(to_rpn("b*c-d^e/f", rpn_result), SUCCESS);

    ck_assert_str_eq(rpn_result, "bc*de^f/-");

}
END_TEST

START_TEST(can_convert_exponentiation_then_subtraction_then_addition_then_multiplication_then_division)
{
#line 108
    ck_assert_int_eq(to_rpn("c^d-e+f*g/h", rpn_result), SUCCESS);

    ck_assert_str_eq(rpn_result, "cd^e-fgh/*+");

}
END_TEST

START_TEST(can_convert_addition_in_parentheses_then_addition)
{
#line 113
    ck_assert_int_eq(to_rpn("(a+b)+c", rpn_result), SUCCESS);

    ck_assert_str_eq(rpn_result, "ab+c+");
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
    tcase_add_test(tc1_1, when_an_unsupported_operator_is_passed_it_returns_invalid_syntax_status);
    tcase_add_test(tc1_1, when_an_unsupported_operator_is_later_in_the_expression_it_returns_invalid_syntax_status);
    tcase_add_test(tc1_1, when_an_unsupported_operator_is_first_in_a_complex_expression_it_returns_invalid_syntax_status);
    tcase_add_test(tc1_1, when_the_expression_is_incomplete_it_returns_invalid_syntax_status);
    tcase_add_test(tc1_1, can_convert_with_any_operands_that_are_lowercase_letters);
    tcase_add_test(tc1_1, when_a_non_lowercase_letter_first_operand_is_passed_it_returns_invalid_syntax_status);
    tcase_add_test(tc1_1, when_a_non_lowercase_letter_last_operand_is_passed_it_returns_invalid_syntax_status);
    tcase_add_test(tc1_1, can_convert_multiple_chained_operations_with_the_same_operator);
    tcase_add_test(tc1_1, can_convert_subtraction_then_addition);
    tcase_add_test(tc1_1, can_convert_subtraction_then_multiplication);
    tcase_add_test(tc1_1, can_convert_addition_then_subtraction_then_multiplication);
    tcase_add_test(tc1_1, can_convert_addition_then_subtraction_then_addition);
    tcase_add_test(tc1_1, can_convert_division_then_multiplication_then_exponentiation);
    tcase_add_test(tc1_1, can_convert_multiplication_then_division_then_subtraction_then_exponentiation);
    tcase_add_test(tc1_1, can_convert_multiplication_then_subtraction_then_exponentiation_then_division);
    tcase_add_test(tc1_1, can_convert_exponentiation_then_subtraction_then_addition_then_multiplication_then_division);
    tcase_add_test(tc1_1, can_convert_addition_in_parentheses_then_addition);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}

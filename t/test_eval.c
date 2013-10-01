/* don't included libcheck when I am included as part of test_main.c */
#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#endif

#include "../src/eval.h"

START_TEST (test_eval_linkage){
    plot_env env;
    plot_expr expr;
    plot_value val;

    val = plot_eval_test(&env, &expr);
}
END_TEST

Suite *
eval_suite(void){
    Suite *s = suite_create("suite_eval");

    TCase *tc_eval = tcase_create("test_eval");

    tcase_add_test(tc_eval, test_eval_linkage);

    suite_add_tcase(s, tc_eval);

    return s;
}


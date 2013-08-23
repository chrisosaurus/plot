/* don't included libcheck when I am included as part of test_main.c */
#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#endif

#include "../src/execute.h"

START_TEST (test_execute_linkage){
    fail_unless( plot_execute_test() == 1 );
}
END_TEST

Suite *
execute_suite(void){
    Suite *s = suite_create("suite_execute");

    TCase *tc_execute = tcase_create("test_execute");

    tcase_add_test(tc_execute, test_execute_linkage);

    suite_add_tcase(s, tc_execute);

    return s;
}


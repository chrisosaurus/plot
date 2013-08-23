/* don't included libcheck when I am included as part of test_main.c */
#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#endif

#include "../src/read.h"

START_TEST (test_read){
    fail_if( plot_read("") == 0 );
}
END_TEST

Suite *
read_suite(void){
    Suite *s = suite_create("suite_read");

    TCase *tc_read = tcase_create("test_read");

    tcase_add_test(tc_read, test_read);

    suite_add_tcase(s, tc_read);

    return s;
}


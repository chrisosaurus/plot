/* don't included libcheck when I am included as part of test_main.c */
#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#endif

#include "../src/parse.h"

static char *simple = "(define x 10) (+ x 5)";

START_TEST (test_parse){
    fail_if( plot_parse(simple) == 0 );
}
END_TEST

Suite *
parse_suite(void){
    Suite *s = suite_create("suite_parse");

    TCase *tc_parse = tcase_create("test_parse");

    tcase_add_test(tc_parse, test_parse);

    suite_add_tcase(s, tc_parse);

    return s;
}


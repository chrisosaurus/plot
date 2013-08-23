/* don't included libcheck when I am included as part of test_main.c */
#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#endif

#include "../src/parse.h"

static char *simple = "(define x 10) (+ x 5)";

START_TEST (test_parse){
    fail_if( parse(simple) == 0 );
}
END_TEST

Suite *
parse_suite(void){
    Suite *s = suite_create("parse");

    TCase *tc_parse = tcase_create("parse");

    tcase_add_test(tc_parse, test_parse);

    suite_add_tcase(s, tc_parse);

    return s;
}


/* don't included libcheck when I am included as part of test_main.c */
#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#endif

#include <stdio.h>

#include "../src/parse.h"

static char *simple = "(define x 10) (+ x 5)";
static char *hard = "(define a 5) \
                     (define b (+ a 4)) \
                     (display a) \
                     (newline) \
                     (disply b) \
                     (newline) \
                     ";


START_TEST (test_parse){

    puts("\trunning test_parse");

    fail_if( plot_parse(simple) == 0 );
}
END_TEST

START_TEST(test_parse_sexpr){
    int i = 0;
    char *ch = "(+ y 11)";
    plot_sexpr sexpr;

    puts("\trunning test_parse_sexpr");

    fail_if( plot_parse_sexpr(&sexpr, ch, &i) == 0 );
    fail_if( i != strlen(ch) );
    fail_unless( sexpr.nchildren );
}
END_TEST

START_TEST(test_parse_expr){
    int i = 0;
    char *ch = "14";
    plot_expr expr;

    puts("\trunning test_parse_expr");

    fail_if( plot_parse_expr(&expr, ch, &i) == 0 );
    fail_if( i != strlen(ch) );
}
END_TEST

Suite *
parse_suite(void){
    Suite *s = suite_create("suite_parse");

    TCase *tc_parse = tcase_create("test_parse");

    tcase_add_test(tc_parse, test_parse_sexpr);
    tcase_add_test(tc_parse, test_parse_expr);
    tcase_add_test(tc_parse, test_parse);

    suite_add_tcase(s, tc_parse);

    return s;
}


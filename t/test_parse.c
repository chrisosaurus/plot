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

    puts("\trunning test_parse for 'simple'");
    fail_if( plot_parse(simple) == 0 );

    puts("\trunning test_parse for 'hard'");
    fail_if( plot_parse(hard) == 0 );
}
END_TEST

START_TEST(test_parse_sexpr){
    int i = 0;
#define SEXPR_TEST "(+ y 11)"
    char *ch = SEXPR_TEST;
    plot_sexpr sexpr;

    puts("\trunning test_parse_sexpr on '" SEXPR_TEST "'");

    fail_if( plot_parse_sexpr(&sexpr, ch, &i) == 0 );
    fail_if( i != strlen(ch) );
    fail_unless( sexpr.nchildren );
}
END_TEST

START_TEST(test_parse_expr){
    int i = 0;
#define EXPR_TEST "14"
    char *ch = EXPR_TEST;
    plot_expr expr;

    puts("\trunning test_parse_expr on '" EXPR_TEST "'");

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


/* don't included libcheck when I am included as part of test_main.c */
#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#endif

#include <stdio.h>
#include <stdlib.h>

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
    plot_program *prog;

    puts("\trunning test_parse for 'simple'");
    prog = plot_parse(simple);
    fail_if( prog == 0 );
    free(prog);

    puts("\trunning test_parse for 'hard'");
    prog = plot_parse(hard);
    fail_if( prog == 0 );
    free(prog);
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
#define EXPR_TEST_NUMBER "14"
    char *ch = EXPR_TEST_NUMBER;
    plot_expr expr;

    puts("\trunning test_parse_expr on '" EXPR_TEST_NUMBER "'");
    fail_if( plot_parse_expr(&expr, ch, &i) == 0 );
    fail_if( i != strlen(ch) );
    fail_unless( expr.u.value.u.number.val == 14 );

#define EXPR_TEST_SYMBOL "some_symbol"
    i=0;
    ch = EXPR_TEST_SYMBOL;

    puts("\trunning test_parse_expr on '" EXPR_TEST_SYMBOL "'");
    fail_if( plot_parse_expr(&expr, ch, &i) == 0 );
    fail_if( i != strlen(ch) );
    fail_if( strcmp(expr.u.value.u.symbol.val, ch) );
}
END_TEST

/*
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
*/

TEST_CASE_NEW(parse)
TEST_CASE_ADD(parse, parse_sexpr)
TEST_CASE_ADD(parse, parse_expr)
TEST_CASE_ADD(parse, parse)
TEST_CASE_END(parse)



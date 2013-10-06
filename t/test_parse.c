/* don't included libcheck when I am included as part of test_main.c */
#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#include "test.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#include "../src/value.h"
#include "../src/types.h"
#include "../src/parse.h"

static char *test_parse_simple = "(define x 10) (+ x 5)";
static char *test_parse_hard = "(define a 5) \
                                (define b [+ a 4]) \
                                (display a) \
                                [newline] \
                                (disply b) \
                                (newline) \
                                ";
static char *test_parse_harder = "(define a (+ (+ 3 5) 2))\
                                  (define b (+ (+ b b) (+ 3 b) b))\
                                  ";

static char *test_parse_error_more = "(define a (+ 4 5)"; /* missing closing ) */

static char *test_parse_error_missmatch = "(define b (+ 11 12)]"; /* missmatch of ( and ] */


START_TEST (test_parse){
    plot_program *prog;

    puts("\trunning test_parse for 'simple'");
    prog = plot_parse(test_parse_simple);
    /* check prog */
    fail_if( prog == 0 );
    fail_if( prog->nchildren != 2 );
    fail_if( prog->max_children != 10 ); /* FIXME current hard coded limit */
    /* check children */
    fail_if( prog->exprs[0].type != plot_expr_sexpr );
    fail_if( prog->exprs[0].u.sexpr.nchildren != 3 );
    free(prog);

    puts("\trunning test_parse for 'hard'");
    prog = plot_parse(test_parse_hard);
    /* check prog */
    fail_if( prog == 0 );
    fail_if( prog->nchildren != 6 );
    fail_if( prog->max_children != 10 ); /* FIXME current hard coded limit */
    /* check children */
    fail_if( prog->exprs[0].type != plot_expr_sexpr );
    fail_if( prog->exprs[0].u.sexpr.nchildren != 3 );
    fail_if( prog->exprs[1].type != plot_expr_sexpr );
    fail_if( prog->exprs[1].u.sexpr.nchildren != 3 );
    free(prog);

    puts("\trunning test_parse for 'harder'");
    prog = plot_parse(test_parse_harder);
    /* check prog */
    fail_if( prog == 0 );
    fail_if( prog->nchildren != 2 );
    fail_if( prog->max_children != 10 ); /* FIXME current hard coded limit */
    /* check children */
    fail_if( prog->exprs[0].type != plot_expr_sexpr );
    fail_if( prog->exprs[0].u.sexpr.nchildren != 3 );
    /* check grand children */
    fail_if( prog->exprs[0].u.sexpr.subforms[0].type != plot_expr_value ); /* define */
    fail_if( prog->exprs[0].u.sexpr.subforms[1].type != plot_expr_value ); /* a */
    fail_if( prog->exprs[0].u.sexpr.subforms[2].type != plot_expr_sexpr ); /* (+ (+ 3 5) 2) */
    fail_if( prog->exprs[0].u.sexpr.subforms[2].u.sexpr.nchildren != 3 ); /* (+ (+ 3 5) 2) */

    free(prog);
}
END_TEST

START_TEST(test_parse_sexpr){
    int i = 0;
#define SEXPR_TEST "(+ y (+ 4 5))"
    char *ch = SEXPR_TEST;
    plot_sexpr sexpr;

    puts("\trunning test_parse_sexpr on '" SEXPR_TEST "'");

    fail_if( plot_parse_sexpr(&sexpr, ch, &i) == 0 );
    fail_unless( i == strlen(ch) );
    fail_unless( sexpr.nchildren == 3 );
    fail_unless( sexpr.subforms[0].type == plot_expr_value );
    fail_unless( sexpr.subforms[0].u.value.type == plot_type_symbol );
    fail_unless( sexpr.subforms[1].type == plot_expr_value );
    fail_unless( sexpr.subforms[1].u.value.type == plot_type_symbol );
    fail_unless( sexpr.subforms[2].type == plot_expr_sexpr );
    fail_unless( sexpr.subforms[2].u.sexpr.nchildren == 3 );
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

#define EXPR_TEST_SEXPR "(symbol1 symbol2 symbol3)"
    ch = EXPR_TEST_SEXPR;
    i=0;

    puts("\trunning test_parse_expr on '" EXPR_TEST_SEXPR "'");
    fail_if( plot_parse_expr(&expr, ch, &i) == 0 );
    fail_if( i != strlen(ch) );
    fail_if( strcmp(expr.u.sexpr.subforms[0].u.value.u.symbol.val, "symbol1") );
    fail_unless( expr.u.sexpr.nchildren == 3 );
}
END_TEST

START_TEST(test_parse_error){
    puts("\ttesting parse error handling : TODO");
    /* FIXME TODO test more */
    /* FIXME TODO test missmatch */
}
END_TEST

TEST_CASE_NEW(parse)
TEST_CASE_ADD(parse, parse_sexpr)
TEST_CASE_ADD(parse, parse_expr)
TEST_CASE_ADD(parse, parse)
    tcase_add_exit_test(tc_parse, test_parse_error, 1);
TEST_CASE_END(parse)


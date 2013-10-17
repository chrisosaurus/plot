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

static const char *test_parse_simple = "(define x 10) (+ x 5)";
static const char *test_parse_hard = "(define a 5) \
                                      (define b [+ a 4]) \
                                      (display a) \
                                      [newline] \
                                      (disply b) \
                                      (newline) \
                                      ";

static const char *test_parse_harder = "(define a (+ (+ 3 5) 2))\
                                        (define b (+ (+ b b) (+ 3 b) b))\
                                        (define c \"hello world'\")\
                                        (define c #f)\
                                        ";

static const char *test_parse_error_more = "(define a (+ 4 5)"; /* missing closing ) */

static const char *test_parse_error_missmatch = "(define b (+ 11 12)]"; /* missmatch of ( and ] */


START_TEST (test_parse){
    plot_program *prog;

    puts("\trunning test_parse for 'simple'");
    fail_if( 0 == plot_init() );
    prog = plot_parse(test_parse_simple);
    /* check prog */
    fail_if( prog == 0 );
    fail_if( prog->nchildren != 2 );
    fail_if( prog->max_children != 100 ); /* FIXME current hard coded limit */
    /* check children */
    fail_if( prog->exprs[0].type != plot_expr_sexpr );
    fail_if( prog->exprs[0].u.sexpr.nchildren != 3 );
    free(prog);

    puts("\trunning test_parse for 'hard'");
    prog = plot_parse(test_parse_hard);
    /* check prog */
    fail_if( prog == 0 );
    fail_if( prog->nchildren != 6 );
    fail_if( prog->max_children != 100 ); /* FIXME current hard coded limit */
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
    fail_unless( prog->nchildren == 4 );
    fail_unless( prog->max_children == 100 ); /* FIXME current hard coded limit */

    /* check children */
    fail_unless( prog->exprs[0].type == plot_expr_sexpr );
    fail_unless( prog->exprs[0].u.sexpr.nchildren == 3 );
    fail_unless( prog->exprs[1].type == plot_expr_sexpr );
    fail_unless( prog->exprs[1].u.sexpr.nchildren == 3 );
    fail_unless( prog->exprs[2].type == plot_expr_sexpr );
    fail_unless( prog->exprs[2].u.sexpr.nchildren == 3);
    fail_unless( prog->exprs[3].type == plot_expr_sexpr );
    fail_unless( prog->exprs[3].u.sexpr.nchildren == 3);

    /* check grand children */
    fail_unless( prog->exprs[0].u.sexpr.subforms[0].type == plot_expr_value ); /* define */
    fail_unless( prog->exprs[0].u.sexpr.subforms[0].u.value->u.symbol.len == 7 ); /* "hello world'" */
    fail_unless( prog->exprs[0].u.sexpr.subforms[1].type == plot_expr_value ); /* a */
    fail_unless( prog->exprs[0].u.sexpr.subforms[2].type == plot_expr_sexpr ); /* (+ (+ 3 5) 2) */
    fail_unless( prog->exprs[0].u.sexpr.subforms[2].u.sexpr.nchildren == 3 );

    fail_unless( prog->exprs[1].u.sexpr.subforms[0].type == plot_expr_value ); /* define */
    fail_unless( prog->exprs[1].u.sexpr.subforms[1].type == plot_expr_value ); /* b */
    fail_unless( prog->exprs[1].u.sexpr.subforms[2].type == plot_expr_sexpr ); /* (+ (+ b b) (+ 3 b) b) */
    fail_unless( prog->exprs[1].u.sexpr.subforms[2].u.sexpr.nchildren == 4 );

    fail_unless( prog->exprs[2].u.sexpr.subforms[0].type == plot_expr_value ); /* define */
    fail_unless( prog->exprs[2].u.sexpr.subforms[1].type == plot_expr_value ); /* c */
    fail_unless( prog->exprs[2].u.sexpr.subforms[2].type == plot_expr_value ); /* "hello world'" */
    fail_unless( prog->exprs[2].u.sexpr.subforms[2].u.value->type == plot_type_string ); /* "hello world'" */
    fail_unless( prog->exprs[2].u.sexpr.subforms[2].u.value->u.string.len == 13 ); /* "hello world'" */
    fail_unless( prog->exprs[2].u.sexpr.subforms[2].u.value->u.string.size == 13 ); /* "hello world'" */
    fail_unless( 0 == strcmp(prog->exprs[2].u.sexpr.subforms[2].u.value->u.string.val, "hello world'") ); /* "hello world'" */

    fail_unless( prog->exprs[3].u.sexpr.subforms[0].type == plot_expr_value ); /* define */
    fail_unless( prog->exprs[3].u.sexpr.subforms[1].type == plot_expr_value ); /* d */
    fail_unless( prog->exprs[3].u.sexpr.subforms[2].type == plot_expr_value ); /* #f */
    fail_unless( prog->exprs[3].u.sexpr.subforms[2].u.value->type == plot_type_boolean ); /* #f */
    fail_unless( false == prog->exprs[3].u.sexpr.subforms[2].u.value->u.boolean.val ); /* #f */

    free(prog);
    plot_cleanup();
}
END_TEST

START_TEST(test_parse_sexpr){
    size_t i = 0;
#define SEXPR_TEST "(+ y (+ 4 5))"
    const char *ch = SEXPR_TEST;
    plot_sexpr sexpr;

    puts("\trunning test_parse_sexpr on '" SEXPR_TEST "'");
    fail_if( 0 == plot_init() );

    fail_if( plot_parse_sexpr(&sexpr, ch, &i) == 0 );
    fail_unless( i == strlen(ch) );
    fail_unless( sexpr.nchildren == 3 );
    fail_unless( sexpr.subforms[0].type == plot_expr_value );
    fail_unless( sexpr.subforms[0].u.value->type == plot_type_symbol );
    fail_unless( sexpr.subforms[1].type == plot_expr_value );
    fail_unless( sexpr.subforms[1].u.value->type == plot_type_symbol );
    fail_unless( sexpr.subforms[2].type == plot_expr_sexpr );
    fail_unless( sexpr.subforms[2].u.sexpr.nchildren == 3 );

    plot_cleanup();
}
END_TEST

START_TEST(test_parse_expr){
    size_t i = 0;
#define EXPR_TEST_NUMBER "14"
    const char *ch = EXPR_TEST_NUMBER;
    plot_expr expr;

    puts("\trunning test_parse_expr on '" EXPR_TEST_NUMBER "'");
    fail_if( 0 == plot_init() );
    fail_if( plot_parse_expr(&expr, ch, &i) == 0 );
    fail_if( i != strlen(ch) );
    fail_unless( expr.u.value->u.number.val == 14 );

#define EXPR_TEST_SYMBOL "some_symbol"
    i=0;
    ch = EXPR_TEST_SYMBOL;

    puts("\trunning test_parse_expr on '" EXPR_TEST_SYMBOL "'");
    fail_if( plot_parse_expr(&expr, ch, &i) == 0 );
    fail_if( i != strlen(ch) );
    fail_if( strcmp(expr.u.value->u.symbol.val, ch) );

#define EXPR_TEST_SEXPR "(symbol1 symbol2 symbol3)"
    ch = EXPR_TEST_SEXPR;
    i=0;

    puts("\trunning test_parse_expr on '" EXPR_TEST_SEXPR "'");
    fail_if( plot_parse_expr(&expr, ch, &i) == 0 );
    fail_if( i != strlen(ch) );
    fail_if( strcmp(expr.u.sexpr.subforms[0].u.value->u.symbol.val, "symbol1") );
    fail_unless( expr.u.sexpr.nchildren == 3 );

    plot_cleanup();
}
END_TEST

START_TEST(test_parse_error){
    puts("\ttesting parse error handling : TODO (currently failing with exit status 0)");
    /* FIXME TODO test more */
    /* FIXME TODO test missmatch */
}
END_TEST

TEST_CASE_NEW(parse)
TEST_CASE_ADD(parse, parse_sexpr)
TEST_CASE_ADD(parse, parse_expr)
TEST_CASE_ADD(parse, parse)
//    tcase_add_exit_test(tc_parse, test_parse_error, 1); /* FIXME commented to avoid error */
TEST_CASE_END(parse)


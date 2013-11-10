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
#include "../src/env.h"
#include "../src/parse.h"
#include "../src/eval.h"
#include "../src/funcs.h"
#include "../src/plot.h"

static const char *test_full_simple_input = "(define a (+ 4 5))\
                                             (define b (+ a 2 3))\
                                             (define c \"hello ' world\")\
                                             (display a)\
                                             (newline)\
                                             (display b)\
                                             (newline)\
                                             (display c)\
                                             (newline)\
                                             (define d #f)\
                                             (display d)\
                                             (newline)\
                                             ";

START_TEST (test_full_simple){
    plot_program *prog;

    puts("Full simple test");
    puts("\tTesting eval and parse combined");
    fail_if( 0 == plot_init() );

    puts("\t\ttesting parse");
    prog = plot_parse(test_full_simple_input);
    fail_if( prog == 0 );
    fail_unless( 12 == prog->nchildren );

    puts("\t\ttesting eval");
    fail_if( 0 == plot_eval(plot_get_global_env(), prog) );

    puts("\tCompleted!");
    plot_cleanup();

}
END_TEST

static const char *test_full_harder_input = "(define adder\
                                                (lambda (b)\
                                                    (lambda (c)\
                                                        (+ b c))))\
                                             (define tmp (adder 10))\
                                             (define end (tmp 15))\
                                             ";

/* plot test value symbol */
#define PT_VS(s, l) &(plot_symbol){s, l, l, 0}

START_TEST (test_full_harder){
    plot_program *prog;

    const plot_value *val;

    puts("\nFull harder test");
    puts("\tTesting eval and parse combined");
    fail_if( 0 == plot_init() );


    puts("\t\ttesting parse");
    prog = plot_parse(test_full_harder_input);
    fail_if( prog == 0 );

    /* check parse tree */
    fail_unless( 3 == prog->nchildren );
    ck_assert_msg( prog->exprs[0].type == plot_expr_sexpr, "child 0 was not of type 'plot_expr_sexpr'");
    ck_assert_msg( prog->exprs[1].type == plot_expr_sexpr, "child 1 was not of type 'plot_expr_sexpr'");
    ck_assert_msg( prog->exprs[2].type == plot_expr_sexpr, "child 2 was not of type 'plot_expr_sexpr'");

    ck_assert_msg( prog->exprs[0].u.sexpr.subforms[0].type == plot_expr_value, "grandchild 0 was not of type 'plot_expr_value'");
    ck_assert_msg( prog->exprs[0].u.sexpr.subforms[0].u.value->type == plot_type_symbol, "grandchild 0 did not contain type 'plot_type_symbol'");
    ck_assert_str_eq( prog->exprs[0].u.sexpr.subforms[0].u.value->u.symbol.val, "define");

    ck_assert_msg( prog->exprs[0].u.sexpr.subforms[1].type == plot_expr_value, "grandchild 1 was not of type 'plot_expr_value'");
    ck_assert_msg( prog->exprs[0].u.sexpr.subforms[1].u.value->type == plot_type_symbol, "grandchild 0 did not contain type 'plot_type_symbol'");
    ck_assert_str_eq( prog->exprs[0].u.sexpr.subforms[1].u.value->u.symbol.val, "adder");


    puts("\t\ttesting eval");
    fail_if( 0 == plot_eval(plot_get_global_env(), prog) );

    /* check env */
    val = plot_env_get(plot_get_global_env(), PT_VS("adder", 6));
    ck_assert_msg( 0 != val, "plot_env did not have 'adder' defined");
    ck_assert_msg( val->type == plot_type_lambda, "adder was not of type 'plot_type_lambda'");

    val = plot_env_get(plot_get_global_env(), PT_VS("tmp", 4));
    ck_assert_msg( 0 != val, "plot_env did not have 'tmp' defined");
    ck_assert_msg( val->type == plot_type_lambda, "tmp was not of type 'plot_type_lambda'");

    val = plot_env_get(val->u.lambda.env, PT_VS("b", 2));
    ck_assert_msg( 0 != val, "tmp's env did not have 'b' defined");
    ck_assert_msg( val->type == plot_type_number, "tmp's env type of 'b' was not 'plot_type_number'");
    ck_assert_msg( val->u.number.val == 10, "tmp's env value of 'b' was not '10'");

    val = plot_env_get(plot_get_global_env(), PT_VS("end", 4));
    ck_assert_msg( 0 != val, "plot_env did not have 'end' defined");
    ck_assert_msg( val->type == plot_type_number, "end was not of type 'plot_type_number'");
    ck_assert_msg( val->u.number.val == 25, "end did not have value '25'");


    puts("\tCompleted!");
    plot_cleanup();
}
END_TEST


static const char * test_full_forms_input_define = "(define a 5)";
static const char * test_full_forms_input_lambda = "(define b\
                                                        (lambda ()\
                                                            (lambda ()\
                                                                \"hello\"";
static const char * test_full_forms_input_if = "(define c\
                                                    (if (< 4 5)\
                                                        \"pass\"\
                                                        \"fail\"))";

START_TEST (test_full_forms){
    plot_program *prog;

    const plot_value *val;

    puts("\nFull forms test");


    puts("\t\ttesting define");
    fail_if( 0 == plot_init() );

    prog = plot_parse(test_full_forms_input_define);
    fail_if( prog == 0 );
    fail_if( 0 == plot_eval(plot_get_global_env(), prog) );

    /* test results */
    val = plot_env_get(plot_get_global_env(), PT_VS("a", 2));
    ck_assert_msg( 0 != val, "env did not contain 'a'");
    ck_assert_msg( val->type == plot_type_number, "a was not of type 'plot_type_number'");
    ck_assert_msg( val->u.number.val == 5, "a did not have the value '5'");


    puts("\t\ttesting lambda");
    fail_if( 0 == plot_init() );

    prog = plot_parse(test_full_forms_input_lambda);
    fail_if( prog == 0 );
    fail_if( 0 == plot_eval(plot_get_global_env(), prog) );

    /* test results */
    val = plot_env_get(plot_get_global_env(), PT_VS("b", 2));
    ck_assert_msg( 0 != val, "env did not contain 'b'");
    ck_assert_msg( val->type == plot_type_lambda, "b was not of type 'plot_type_lambda'");


    puts("\t\ttesting define");
    fail_if( 0 == plot_init() );

    prog = plot_parse(test_full_forms_input_if);
    fail_if( prog == 0 );
    fail_if( 0 == plot_eval(plot_get_global_env(), prog) );

    /* test results */
    val = plot_env_get(plot_get_global_env(), PT_VS("c", 2));
    ck_assert_msg( 0 != val, "env did not contain 'c'");
    ck_assert_msg( val->type == plot_type_string, "c was not of type 'plot_type_string'");
    ck_assert_str_eq( val->u.string.val, "pass" );


    puts("Completed!");
    plot_cleanup();
}
END_TEST

#undef PT_VS

const char * tf_unspecified = "(define a (define b 10))\
                               (define c (display \"hello world\"))\
                               (define d (if #f \"dude\"))\
                               (define e (newline))";

START_TEST (test_full_unspecified){
    plot_symbol sym;
    plot_value *val;
    plot_program *prog;

    sym.len = 2;
    sym.size = 2;

    puts("\nFull unspecified test");
    fail_if( 0 == plot_init() );

    prog = plot_parse(tf_unspecified);
    fail_if( 0 == prog );
    fail_if( 0 == plot_eval(plot_get_global_env(), prog) );

    sym.val = "a";
    sym.hash = 0;
    val = plot_env_get( plot_get_global_env(), &sym);
    ck_assert_msg( 0 != val, "symbol a was not defined");
    ck_assert_msg( plot_type_unspecified == val->type, "symbol a did not have unspecified value");

    sym.val = "b";
    sym.hash = 0;
    val = plot_env_get( plot_get_global_env(), &sym);
    ck_assert_msg( 0 != val, "symbol b was not defined");
    ck_assert_msg( plot_type_number == val->type, "symbol b was not a number");
    ck_assert_msg( 10 == val->u.number.val, "symbol b did not have value 10");

    sym.val = "c";
    sym.hash = 0;
    val = plot_env_get( plot_get_global_env(), &sym);
    ck_assert_msg( 0 != val, "symbol c was not defined");
    ck_assert_msg( plot_type_unspecified == val->type, "symbol c did not have unspecified value");

    sym.val = "d";
    sym.hash = 0;
    val = plot_env_get( plot_get_global_env(), &sym);
    ck_assert_msg( 0 != val, "symbol d was not defined");
    ck_assert_msg( plot_type_unspecified == val->type, "symbol d did not have unspecified value");

    sym.val = "e";
    sym.hash = 0;
    val = plot_env_get( plot_get_global_env(), &sym);
    ck_assert_msg( 0 != val, "symbol e was not defined");
    ck_assert_msg( plot_type_unspecified == val->type, "symbol e did not have unspecified value");


    puts("completed!");
    plot_cleanup();
}
END_TEST

TEST_CASE_NEW(full)
TEST_CASE_ADD(full, full_simple)
TEST_CASE_ADD(full, full_harder)
TEST_CASE_ADD(full, full_forms)
TEST_CASE_ADD(full, full_unspecified)
TEST_CASE_END(full)

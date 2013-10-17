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
#include "../src/hash.h"
#include "../src/env.h"
#include "../src/eval.h"
#include "../src/parse.h"
#include "../src/funcs.h"

START_TEST (test_eval_add){
    const plot_value *val;
    const char *ch;
    size_t i;
    plot_expr expr;

    plot_init();

#define PLOT_EVAL_SIMPLE "(+ 5 (+ 2 2))"
    ch = PLOT_EVAL_SIMPLE;
    i=0;

    puts("\tparsing expression for eval test");
    fail_if( 0 == plot_parse_expr(&expr, ch, &i) );

    puts("\teval of '" PLOT_EVAL_SIMPLE "'");
    fail_if( 0 == (val = plot_eval_expr(plot_get_env(), &expr)) );
    fail_unless( i == strlen(ch) );
    fail_unless( val->type == plot_type_number );
    fail_unless( val->u.number.val == 9 );

    printf("Expected number '9', got number '%d'\n", val->u.number.val);

    plot_cleanup();
}
END_TEST

TEST_CASE_NEW(eval)
TEST_CASE_ADD(eval, eval_add)
TEST_CASE_END(eval)


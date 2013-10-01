/* don't included libcheck when I am included as part of test_main.c */
#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "../src/eval.h"
#include "../src/parse.h"

START_TEST (test_eval_linkage){
    plot_value *val;
    plot_env *env = plot_env_init(0);
    plot_expr expr;

    /* FIXME TODO need to setup env to know about addition */

#define PLOT_EVAL_SIMPLE "(+ 5 4)"
    char *ch = PLOT_EVAL_SIMPLE;
    int i=0;

    puts("\tparsing expression for eval test");
    fail_if( 0 == plot_parse_expr(&expr, ch, &i) );

    puts("\teval of '" PLOT_EVAL_SIMPLE "'");
    fail_if( 0 == (val = plot_eval(env, &expr)) );
    fail_unless( i == strlen(ch) );
    fail_unless( val->type == plot_type_number );
    fail_unless( val->u.number.val == 9 );

    free(val);
    plot_env_cleanup(env);
}
END_TEST

Suite *
eval_suite(void){
    Suite *s = suite_create("suite_eval");

    TCase *tc_eval = tcase_create("test_eval");

    tcase_add_test(tc_eval, test_eval_linkage);

    suite_add_tcase(s, tc_eval);

    return s;
}


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

static char *test_full_simple = "(define a (+ 4 5))\
                                 (define b (+ a 2 3))\
                                 (display a)\
                                 (newline)\
                                 (display b)\
                                 (newline)\
                                 ";

START_TEST (test_full){
    plot_program *prog;
    const plot_value *val;
    plot_env *env;

    plot_value add;
    plot_symbol sym;

    puts("\tTesting eval and parse combined");

    env = plot_env_init(0);
    sym.val = "+";
    sym.len = 2;
    sym.size = 2;
    add.type = plot_type_function;
    add.u.function.env = 0;
    add.u.function.func = plot_func_add;
    puts("\t\tdefining function add");
    fail_unless( 1 == plot_env_define(env, &sym, &add) );

    puts("\t\ttesting parse");
    prog = plot_parse(test_full_simple);
    fail_if( prog == 0 );

    puts("\t\ttesting eval");
    val = plot_eval(env, prog);

    puts("\tCompleted!");

}
END_TEST

TEST_CASE_NEW(full)
TEST_CASE_ADD(full, full)
TEST_CASE_END(full)

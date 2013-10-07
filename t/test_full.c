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

static const char *test_full_simple = "(define a (+ 4 5))\
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

    plot_value add, disp, newl;
    plot_symbol sym_add, sym_disp, sym_newl;;

    puts("\tTesting eval and parse combined");
    env = plot_env_init(0);

    sym_add.val = "+";
    sym_add.len = 2;
    sym_add.size = 2;
    add.type = plot_type_function;
    add.u.function.env = 0;
    add.u.function.func = plot_func_add;
    puts("\t\tdefining function 'add'");
    fail_unless( 1 == plot_env_define(env, &sym_add, &add) );

    sym_disp.val = "display";
    sym_disp.len = 7;
    sym_disp.size = 7;
    disp.type = plot_type_function;
    disp.u.function.env = 0;
    disp.u.function.func = plot_func_display;
    puts("\t\tdefining function 'display'");
    fail_unless( 1 == plot_env_define(env, &sym_disp, &disp) );

    sym_newl.val = "newline";
    sym_newl.len = 7;
    sym_newl.size = 7;
    newl.type = plot_type_function;
    newl.u.function.env = 0;
    newl.u.function.func = plot_func_newline;
    puts("\t\tdefining function 'newline'");
    fail_unless( 1 == plot_env_define(env, &sym_newl, &newl) );


    puts("\t\ttesting parse");
    prog = plot_parse(test_full_simple);
    fail_if( prog == 0 );

    puts("\t\ttesting eval");
    fail_if( 0 == (val = plot_eval(env, prog)) );

    puts("\tCompleted!");

}
END_TEST

TEST_CASE_NEW(full)
TEST_CASE_ADD(full, full)
TEST_CASE_END(full)

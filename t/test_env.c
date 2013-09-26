#include <stdlib.h> /* free */
#include <stdio.h> /* printf */

/* don't included libcheck when I am included as part of test_main.c */
#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#endif

#include "../src/env.h"

START_TEST (test_env){
    plot_env *e;
    plot_value v;
    plot_value v2;
    plot_symbol s = (plot_symbol){"sym", 4, 4};

    puts("Testing construction");
    fail_if( 0 == (e = plot_env_init(0)) );

    puts("Testing basic define");
    fail_if( plot_env_get(e, &s) );
    fail_unless( plot_env_define(e, &s, &v) );
    fail_unless( &v == plot_env_get(e, &s) );

    puts("Testing invalid define");
    fail_if( plot_env_define(e, &s, &v) );

    puts("Testing set");
    fail_unless( plot_env_set(e, &s, &v2) );
    fail_unless( &v2 == plot_env_get(e, &s) );

    plot_env_cleanup(e);
}
END_TEST

TEST_CASE_NEW(env)
TEST_CASE_ADD(env, env)
TEST_CASE_END(env)


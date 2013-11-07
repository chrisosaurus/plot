#include <stdlib.h> /* free */
#include <stdio.h> /* printf */

/* don't included libcheck when I am included as part of test_main.c */
#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#include "test.h"
#endif

#include "../src/value.h"
#include "../src/hash.h"
#include "../src/env.h"

START_TEST (test_env){
    plot_env *e;
    plot_value v;
    plot_symbol s = (plot_symbol){"sym", 4, 4};

    fail_if( 0 == plot_init() );

    puts("\tTesting construction");
    fail_if( 0 == (e = plot_alloc_env(0)) );

    puts("\tTesting basic define");
    fail_if( plot_env_get(e, &s) );
    fail_unless( plot_env_define(e, &s, &v) );
    fail_unless( &v == plot_env_get(e, &s) );

    puts("\tTesting define mutation");
    fail_unless( plot_env_define(e, &s, 0) );
    fail_unless( 0 == plot_env_get(e, &s) );

    plot_env_cleanup(e);
    plot_cleanup();
}
END_TEST

TEST_CASE_NEW(env)
TEST_CASE_ADD(env, env)
TEST_CASE_END(env)


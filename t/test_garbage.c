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

/* plot test value symbol */
#define PT_VS(s) &(plot_symbol){s, 0, 0}

START_TEST (test_garbage){
    struct plot_value *v;
    struct plot_program *prog;

    puts("\nTesting garbage collection");
    fail_if( 0 == plot_init() );

    puts("\t\tTesting basic value allocation");
    v = plot_new_value();
    fail_if( 0 == v );

    fail_unless( 1 == v->gc.refcount );
    plot_value_incr(v);
    fail_unless( 2 == v->gc.refcount );

    plot_value_decr(v);
    fail_unless( 1 == v->gc.refcount );
    fail_if( plot_type_reclaimed == v->type );

    plot_value_decr(v);
    fail_unless( 0 == v->gc.refcount );
    fail_unless( plot_type_reclaimed == v->type );

    v = 0;


    puts("\t\tTesting plus");
    prog = plot_parse("(define result (+ 4 11))");
    fail_if( prog == 0 );

    puts("\t\ttesting eval");
    fail_if( 0 == plot_eval(plot_get_env(), prog) );

    v = plot_env_get(plot_get_env(), PT_VS("result"));
    /* NB below it is 3:
     * creating a value starts with refcount 1
     * the env holds one reference (we stored)
     * we hold on reference (as we fetched)
     */
    ck_assert_int_eq( 3,  v->gc.refcount );
    fail_if( plot_type_reclaimed == v->type );

    puts("\tCompleted!");
    plot_cleanup();
}
END_TEST

#undef PT_VS

TEST_CASE_NEW(garbage)
TEST_CASE_ADD(garbage, garbage)
TEST_CASE_END(garbage)

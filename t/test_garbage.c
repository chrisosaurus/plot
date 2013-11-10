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
#define PT_VS(s, l) &(plot_symbol){s, l, l, 0}

START_TEST (test_garbage){
    struct plot_value *v;
    struct plot_program *prog;

    puts("\nTesting garbage collection");
    fail_if( 0 == plot_init() );

    puts("\t\tTesting basic value allocation");
    v = plot_alloc_value();
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
    fail_if( 0 == plot_eval(plot_get_global_env(), prog) );

    v = plot_env_get(plot_get_global_env(), PT_VS("result", 7));
    /* define will create the value (with refount 1)
     * define then stored the value (refcount to 2) and then
     * decrs as it no longer needs the value (refcount back to 1)
     * we then fetch it which called incr (refcount back to 2)
     */
    ck_assert_int_eq( 2,  v->gc.refcount );
    fail_if( plot_type_reclaimed == v->type );

    puts("\tCompleted!");
    plot_cleanup();
}
END_TEST

#undef PT_VS

TEST_CASE_NEW(garbage)
TEST_CASE_ADD(garbage, garbage)
TEST_CASE_END(garbage)

#include <stdlib.h> /* free */
#include <stdio.h> /* printf */

/* don't included libcheck when I am included as part of test_main.c */
#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#include "test.h"
#endif

#include "../../src/value.h"
#include "../../src/hash.h"

START_TEST (test_hash){
    plot_symbol a = (plot_symbol){"a", 2, 2, 0};
    plot_symbol b = (plot_symbol){"b", 2, 2, 0};
    plot_symbol c = (plot_symbol){"c", 2, 2, 0};

    plot_value value;
    plot_hash hash;
    value.gc.refcount = 1;

    puts("\tTesting hash construction");
    fail_if( 0 == plot_init() );
    fail_if( 0 == (plot_hash_init(&hash)) );

    puts("\tTesting basic hash get and insert");
    fail_if( plot_hash_get(&hash, &b) );
    fail_unless( plot_hash_set(&hash, &b, &value) );
    fail_if( plot_hash_get(&hash, &b) != &value );
    fail_if( plot_hash_get(&hash, &a) );

    puts("\tTesting correct number of elements");
    fail_unless( plot_hash_nelems(&hash) == 3);

    puts("\tTesting mutation");
    fail_unless( plot_hash_set(&hash, &b, 0) );
    fail_unless( 0 == plot_hash_get(&hash, &b) );
    fail_unless( 3 == plot_hash_nelems(&hash) );

    plot_hash_cleanup(&hash);
    plot_cleanup();
}
END_TEST

TEST_CASE_NEW(hash)
TEST_CASE_ADD(hash, hash)
TEST_CASE_END(hash)


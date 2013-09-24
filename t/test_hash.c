#include <stdlib.h> /* free */
#include <stdio.h> /* printf */

/* don't included libcheck when I am included as part of test_main.c */
#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#endif

#include "../src/hash.h"

START_TEST (test_hash){
    char *key = "hello";
    plot_value value;
    plot_hash *hash;

    fail_unless( plot_hash_init(hash) );
    fail_if( plot_hash_get(hash, key) );
    fail_unless( plot_hash_insert(hash, key, &value) );
    fail_unless( plot_hash_get(hash, key) );
}
END_TEST

TEST_CASE_NEW(hash)
TEST_CASE_ADD(hash, hash)
TEST_CASE_END(hash)


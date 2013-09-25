#include <stdlib.h> /* free */
#include <stdio.h> /* printf */

/* don't included libcheck when I am included as part of test_main.c */
#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#endif

#include "../src/hash.h"

START_TEST (test_hash){
    char *key1 = "b";
    char *key2 = "a";
    char *key3 = "c";
    plot_value value;
    plot_hash *hash;

    puts("Testing hash construction");
    fail_if( 0 == (hash = plot_hash_init(0)) );

    puts("Testing basic hash get and insert");
    fail_if( plot_hash_get(hash, key1) );
    fail_unless( plot_hash_insert(hash, key1, &value) );
    fail_if( plot_hash_get(hash, key1) != &value );
    fail_if( plot_hash_get(hash, key2) );

    puts("Testing for correct key ordering");
    fail_unless( plot_hash_insert(hash, key2, &value) );
    fail_unless( plot_hash_insert(hash, key3, &value) );
    fail_if( strcmp(key2, hash->head->key) );
    fail_if( strcmp(key1, hash->head->next->key) );
    fail_if( strcmp(key3, hash->head->next->next->key) );

    puts("Testing correct number of elements");
    fail_unless( hash->head->next->next->next == 0 );
    fail_unless( hash->n_elems == 3);

    plot_hash_cleanup(hash);
}
END_TEST

TEST_CASE_NEW(hash)
TEST_CASE_ADD(hash, hash)
TEST_CASE_END(hash)


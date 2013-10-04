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

START_TEST (test_hash){
    plot_symbol a = (plot_symbol){"a", 2, 2};
    plot_symbol b = (plot_symbol){"b", 2, 2};
    plot_symbol c = (plot_symbol){"c", 2, 2};

    plot_value value;
    plot_hash *hash;

    puts("\tTesting hash construction");
    fail_if( 0 == (hash = plot_hash_init()) );

    puts("\tTesting basic hash get and insert");
    fail_if( plot_hash_get(hash, &b) );
    fail_unless( plot_hash_insert(hash, &b, &value) );
    fail_if( plot_hash_get(hash, &b) != &value );
    fail_if( plot_hash_get(hash, &a) );

    puts("\tTesting for correct key ordering");
    fail_unless( plot_hash_insert(hash, &a, &value) );
    fail_unless( plot_hash_insert(hash, &c, &value) );
    fail_if( strcmp(a.val, hash->head->key->val) );
    fail_if( strcmp(b.val, hash->head->next->key->val) );
    fail_if( strcmp(c.val, hash->head->next->next->key->val) );

    puts("\tTesting correct number of elements");
    fail_unless( hash->head->next->next->next == 0 );
    fail_unless( hash->n_elems == 3);

    puts("\tTesting (lack of) mutation");
    fail_if( plot_hash_insert(hash, &b, 0) );
    fail_if( plot_hash_get(hash, &b) == 0 );
    fail_unless( hash->n_elems == 3);

    puts("\tTesting mutation");
    fail_unless( plot_hash_set(hash, &b, &value) );
    fail_unless( plot_hash_get(hash, &b) == &value );

    plot_hash_cleanup(hash);
}
END_TEST

TEST_CASE_NEW(hash)
TEST_CASE_ADD(hash, hash)
TEST_CASE_END(hash)


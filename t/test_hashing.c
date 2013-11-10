#include <stdio.h>
#include <stdlib.h>

#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#include "test.h"
#endif

#include "../src/value.h"

START_TEST (test_hashing){
    plot_symbol s;

    puts("\tBeginning basic hashing tests");

    s.val = "lambda";
    s.len = s.size = 7;
    s.hash =0;

    plot_hash_symbol(&s);
    printf("lambda test got hash '%lu'\n", s.hash);
    ck_assert_msg( s.hash == 508553539801, "lambda hash was not as expected");

    s.val = "define";
    s.len = s.size = 7;
    s.hash =0;

    plot_hash_symbol(&s);
    printf("define test got hash '%lu'\n", s.hash);
    ck_assert_msg( s.hash == 540325222373, "define hash was not as expected");

    s.val = "if";
    s.len = s.size = 3;
    s.hash =0;

    plot_hash_symbol(&s);
    printf("if test got hash '%lu'\n", s.hash);
    ck_assert_msg( s.hash == 6723, "if hash was not as expected");

    s.val = "set!";
    s.len = s.size = 5;
    s.hash =0;

    plot_hash_symbol(&s);
    printf("set! test got hash '%lu'\n", s.hash);
    ck_assert_msg( s.hash == 1622113, "set! hash was not as expected");

    puts("\tCompleted!");
}
END_TEST

TEST_CASE_NEW(hashing)
TEST_CASE_ADD(hashing, hashing)
TEST_CASE_END(hashing)


#include <stdlib.h> /* free */
#include <stdio.h> /* printf */

/* don't included libcheck when I am included as part of test_main.c */
#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#include "test.h"
#endif

#include "../src/read.h"

static const char * const test_read_filename = "Makefile";

START_TEST (test_read){
    char *b = plot_read(test_read_filename);
    fail_if( b == 0 );
    free(b);
}
END_TEST

TEST_CASE_NEW(read)
TEST_CASE_ADD(read, read)
TEST_CASE_END(read)


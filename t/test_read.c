#include <stdlib.h> /* free */
#include <stdio.h> /* printf */

/* don't included libcheck when I am included as part of test_main.c */
#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#endif

#include "../src/read.h"

static const char * const filename = "Makefile";

START_TEST (test_read){
    char *b = plot_read(filename);
    fail_if( b == 0 );
    free(b);
}
END_TEST

Suite *
read_suite(void){
    Suite *s = suite_create("suite_read");

    TCase *tc_read = tcase_create("test_read");

    tcase_add_test(tc_read, test_read);

    suite_add_tcase(s, tc_read);

    return s;
}


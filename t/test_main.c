#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <check.h>

#include "test.h"

/* test_*.c want to include check themselves in order to allow better
 * editor feedback
 * INCLUDED_CHECK prevents them including it
 */
#define PLOT_TEST_MAIN
#include "test_read.c"
#include "test_parse.c"
#include "test_eval.c"
#include "test_hash.c"
#include "test_env.c"
#include "test_funcs.c"
#include "test_forms.c"
#include "test_full.c"

int main(void){
    int number_failed = 0;
    Suite *s;
    SRunner *sr;

    puts("\nBegining testing");

    MAIN_ADD_SUITE(read)

    MAIN_ADD_SUITE(parse)

    MAIN_ADD_SUITE(eval)

    MAIN_ADD_SUITE(full)

    MAIN_ADD_SUITE(forms)

    MAIN_ADD_SUITE(hash)

    MAIN_ADD_SUITE(env)

    MAIN_ADD_SUITE(funcs)
    MAIN_ADD_SUITE(funcs_error)

    puts("\nFinished testing.");
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


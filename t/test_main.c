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

    puts("\n##########\nTesting read");
    s = read_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);

    puts("\n##########\nTesting parse");
    s = parse_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);

    puts("\n##########\nTesting eval");
    s = eval_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);

    puts("\n##########\nTesting full");
    s = full_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);

    puts("\n##########\nTesting forms");
    s = forms_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);

    puts("\n##########\nTesting hash");
    s = hash_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);

    puts("\n##########\nTesting env");
    s = env_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);

    puts("\n##########\nTesting funcs");
    s = funcs_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);


    puts("\nFinished testing.");
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <check.h>

#define TEST_CASE_NEW(name) Suite *\
    name##_suite(void){\
        Suite *s = suite_create("suite_" #name);\
        TCase *tc_##name = tcase_create("test_" #name);

#define TEST_CASE_ADD(name, test) tcase_add_test(tc_##name, test_##test);

#define TEST_CASE_END(name) suite_add_tcase(s, tc_##name);\
        return s;\
    }

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

int main(void){
    int number_failed = 0;
    Suite *s;
    SRunner *sr;

    puts("\nBegining testing");

    puts("\nTesting read");
    s = read_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);

    puts("\nTesting parse");
    s = parse_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);

    puts("\nTesting eval");
    s = eval_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);

    puts("\nTesting forms");
    s = forms_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);

    puts("\nTesting hash");
    s = hash_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);

    puts("\nTesting env");
    s = env_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);

    puts("\nTesting funcs");
    s = funcs_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);


    puts("\nFinished testing.");
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


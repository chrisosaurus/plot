#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <check.h>

/* test_*.c want to include check themselves in order to allow better
 * editor feedback
 * INCLUDED_CHECK prevents them including it
 */
#define PLOT_TEST_MAIN
#include "test_read.c"
#include "test_parse.c"
#include "test_execute.c"

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

    puts("\nTesting execute");
    s = execute_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);

    puts("\nFinished testing.");
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


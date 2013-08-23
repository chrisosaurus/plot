#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <check.h>

#include "test_read.c"
#include "test_parse.c"
#include "test_execute.c"

int main(void){
    int number_failed = 0;
    Suite *s;
    SRunner *sr;


    s = read_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);

    s = parse_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);

    s = execute_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed += srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


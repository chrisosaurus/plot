#include "../src/execute.h"

START_TEST (test_execute_linkage){
    fail_unless( execute_test() == 1 );
}
END_TEST

Suite *
execute_suite(void){
    Suite *s = suite_create("execute");

    TCase *tc_execute = tcase_create("newline");

    tcase_add_test(tc_execute, test_execute_linkage);

    suite_add_tcase(s, tc_execute);

    return s;
}


#include "../src/read.h"

START_TEST (test_read_linkage){
    fail_unless( read_test() == 1 );
}
END_TEST

Suite *
read_suite(void){
    Suite *s = suite_create("read");

    TCase *tc_read = tcase_create("newline");

    tcase_add_test(tc_read, test_read_linkage);

    suite_add_tcase(s, tc_read);

    return s;
}


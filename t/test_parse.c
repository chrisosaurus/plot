#include "../src/parse.h"

START_TEST (test_parse_linkage){
    fail_unless( parse_test() == 1 );
}
END_TEST

Suite *
parse_suite(void){
    Suite *s = suite_create("parse");

    TCase *tc_parse = tcase_create("newline");

    tcase_add_test(tc_parse, test_parse_linkage);

    suite_add_tcase(s, tc_parse);

    return s;
}


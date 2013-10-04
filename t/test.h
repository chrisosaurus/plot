/* this header is defined so that each test_*.c file is valid
 * this means my vim plugin 'syntastic' can check each file
 */

#define TEST_CASE_NEW(name) Suite *\
    name##_suite(void){\
        Suite *s = suite_create("suite_" #name);\
        TCase *tc_##name = tcase_create("test_" #name);

#define TEST_CASE_ADD(name, test) tcase_add_test(tc_##name, test_##test);

#define TEST_CASE_END(name) suite_add_tcase(s, tc_##name);\
        return s;\
    }


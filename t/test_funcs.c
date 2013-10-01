/* don't included libcheck when I am included as part of test_main.c */
#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "../src/value.h"
#include "../src/hash.h"
#include "../src/env.h"
#include "../src/funcs.h"

START_TEST (test_funcs_add){
    plot_value *res;
    plot_value v1, v2;
    plot_env *env;

    v1.type = plot_type_number;
    v1.u.number.val = 4;

    v2.type = plot_type_number;
    v2.u.number.val = 5;

    puts("\tsetting up env for test_funcs_add");
    env = plot_env_init(0);
    fail_if( 0 == env );

    puts("\ttesting basic addition");
    fail_if( 0 == (res = plot_func_add(env, &v1, &v2)) );
    fail_unless( res->type == plot_type_number );
    fail_unless( res->u.number.val == 9 );

    plot_env_cleanup(env);
    free(res);
}
END_TEST

Suite *
funcs_suite(void){
    Suite *s = suite_create("suite_funcs");

    TCase *tc_funcs = tcase_create("test_funcs");

    tcase_add_test(tc_funcs, test_funcs_add);

    suite_add_tcase(s, tc_funcs);

    return s;
}


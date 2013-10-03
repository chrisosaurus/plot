/* don't included libcheck when I am included as part of test_main.c */
#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "../src/value.h"
#include "../src/types.h"
#include "../src/hash.h"
#include "../src/env.h"
#include "../src/funcs.h"

START_TEST (test_funcs_add){
    const plot_value *res;
    plot_expr vals[2];
    plot_env *env;

    vals[0].type = plot_expr_value;
    vals[0].u.value.type = plot_type_number;
    vals[0].u.value.u.number.val = 4;

    vals[1].type = plot_expr_value;
    vals[1].u.value.type = plot_type_number;
    vals[1].u.value.u.number.val = 5;

    puts("\tsetting up env for test_funcs_add");
    env = plot_env_init(0);
    fail_if( 0 == env );

    puts("\ttesting basic addition");
    fail_if( 0 == (res = plot_func_add(env, vals, 2)) );
    fail_unless( res->type == plot_type_number );
    fail_unless( res->u.number.val == 9 );

    plot_env_cleanup(env);
}
END_TEST

START_TEST (test_funcs_env){
    plot_symbol sym;
    plot_value add;
    plot_env *env = plot_env_init(0);

    const plot_value *res;
    plot_expr vals[2];
    const plot_value *f;

    sym.val = "+";
    sym.len = 2;
    sym.size = 2;

    add.type = plot_type_function;
    add.u.function.func = plot_func_add;
    puts("\tdefining function add");
    fail_unless( 1 == plot_env_define(env, &sym, &add) );

    vals[0].type = plot_expr_value;
    vals[0].u.value.type = plot_type_number;
    vals[0].u.value.u.number.val = 4;

    vals[1].type = plot_expr_value;
    vals[1].u.value.type = plot_type_number;
    vals[1].u.value.u.number.val = 5;

    puts("\ttesting fetching function");
    fail_if( 0 == (f = plot_env_get(env, &sym)) );
    fail_unless( f->type == plot_type_function );
    fail_unless( f->u.function.func == plot_func_add );

    puts("\ttesting function call");
    fail_if( 0 == (res = f->u.function.func(env, vals, 2)) );
    fail_unless( res->type == plot_type_number );
    fail_unless( res->u.number.val == 9 );

    plot_env_cleanup(env);
}
END_TEST

START_TEST (test_error_alloc_failed){
    puts("Testing alloc failed error handling:");
    plot_value v;
    v.type = plot_type_error;
    v.u.error.type = plot_error_alloc_failed;
    v.u.error.msg = "testing error";
    plot_handle_error(&v, "test_error");
}
END_TEST

START_TEST (test_error_bad_args){
    puts("Testing bad args error handling:");
    plot_value v;
    v.type = plot_type_error;
    v.u.error.type = plot_error_bad_args;
    v.u.error.msg = "testing error";
    plot_handle_error(&v, "test_error");
}
END_TEST


Suite *
funcs_suite(void){
    Suite *s = suite_create("suite_funcs");

    TCase *tc_funcs = tcase_create("test_funcs");

    tcase_add_test(tc_funcs, test_funcs_add);
    tcase_add_test(tc_funcs, test_funcs_env);

    tcase_add_exit_test(tc_funcs, test_error_alloc_failed, 1);
    tcase_add_exit_test(tc_funcs, test_error_bad_args, 1);

    suite_add_tcase(s, tc_funcs);

    return s;
}

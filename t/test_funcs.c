/* don't included libcheck when I am included as part of test_main.c */
#ifndef PLOT_TEST_MAIN
#define PLOT_TEST_MAIN
#include <check.h>
#include "test.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#include "../src/value.h"
#include "../src/types.h"
#include "../src/hash.h"
#include "../src/env.h"
#include "../src/funcs.h"
#include "../src/plot.h"

/* functions to bind */
struct plot_test_funcs_error_tests {
    plot_value func;
    plot_expr args[2];
    int expected;
    char *failure_msg;
};

/* plot test func expr value */
#define PTF_EV(e) {plot_expr_value, {.value=e}}

/* plot test value number */
#define PTF_VN(n) {plot_type_number, {.number={n}}}

/* plot test func value funct */
#define PTF_VF(f) {plot_type_function, {.function = {0, f}}}

/* length of array */
#define PTF_LENGTH(x) (sizeof x / sizeof x[0])

/* call function given index */
#define PTF_CALL_FUNC(i) math_bindings[i].func.u.function.func

/* yield args and len given index */
#define PTF_ARGS(i) math_bindings[i].args, PTF_LENGTH(math_bindings[i].args)

/* yield expected value given index */
#define PTF_EXP(i) math_bindings[i].expected

/* yield error message given index */
#define PTF_ERR(i) math_bindings[i].failure_msg

START_TEST (test_funcs_math){
    int i;
    plot *plot;
    const plot_value *r;

    struct plot_test_funcs_error_tests math_bindings[] = {
        /* function                   ( args1,            arg2 )              = expected "failure message" */
        {PTF_VF(plot_func_add),      { PTF_EV(PTF_VN(2)),  PTF_EV(PTF_VN(3))  },  5,     "\tfailed test for plot_func_add"},
        {PTF_VF(plot_func_subtract), { PTF_EV(PTF_VN(10)), PTF_EV(PTF_VN(7))  },  3,     "\tfailed test for plot_func_subtract"},
        {PTF_VF(plot_func_multiply), { PTF_EV(PTF_VN(5)),  PTF_EV(PTF_VN(15)) }, 75,     "\tfailed test for plot_func_multiply"},
        {PTF_VF(plot_func_divide),   { PTF_EV(PTF_VN(10)), PTF_EV(PTF_VN(3))  },  3,     "\tfailed test for plot_func_divide"}
    };

    puts("\ttesting math functions");

    fail_if( 0 == (plot = plot_init()) );

    for( i=0; i<PTF_LENGTH(math_bindings); ++i ){
        r = PTF_CALL_FUNC(i)( plot->env, PTF_ARGS(i) );
        fail_unless( r->type == plot_type_number );
        ck_assert_msg( r->u.number.val == PTF_EXP(i), PTF_ERR(i));
    }
}
END_TEST

#undef PTF_EV
#undef PTF_VN
#undef PTF_VF
#undef PTF_LENGTH
#undef PTF_CALL_FUNC
#undef PTF_ARGS
#undef PTF_EXP
#undef PTF_ERR


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
    plot_value v;
    puts("\tTesting alloc failed error handling (error expected):");
    v.type = plot_type_error;
    v.u.error.type = plot_error_alloc_failed;
    v.u.error.msg = "testing alloc failed error";
    v.u.error.location = "test_error_alloc_failed";
    plot_handle_error(&v);
}
END_TEST

START_TEST (test_error_bad_args){
    plot_value v;
    puts("\tTesting bad args error handling (error expected):");
    v.type = plot_type_error;
    v.u.error.type = plot_error_bad_args;
    v.u.error.msg = "testing bad args error";
    v.u.error.location = "test_error_bad_args";
    plot_handle_error(&v);
}
END_TEST

START_TEST (test_error_internal){
    plot_value v;
    puts("\tTesting internal error handling (error expected):");

    v.type = plot_type_error;
    v.u.error.type = plot_error_internal;
    v.u.error.msg = "testing internal error";
    v.u.error.location = "test_error_internal";
    plot_handle_error(&v);
}
END_TEST

START_TEST (test_error_unbound_symbol){
    plot_value v;
    puts("\tTesting 'unbound symbol' error handling (error expected):");

    v.type = plot_type_error;
    v.u.error.type = plot_error_unbound_symbol;
    v.u.error.msg = "testing unbound symbol error";
    v.u.error.location = "test_error_unbound_symbol";
    plot_handle_error(&v);
}
END_TEST


START_TEST (test_display){
    plot_expr v, s;
    plot_env *env;
    env = plot_env_init(0);


    v.type = plot_expr_value;
    puts("\n\tTesting display of values");

    puts("\t\ttesting display of number (expected '3')");
    v.u.value.type = plot_type_number;
    v.u.value.u.number.val = 3;
    plot_func_display(env, &v, 1);
    puts(""); /* trailing \n */

    puts("\t\ttesting display of symbol (expected '3')");
    s.type = plot_expr_value;
    s.u.value.type = plot_type_symbol;
#define TEST_DISPLAY_SYMBOL "symbol"
    s.u.value.u.symbol.val = TEST_DISPLAY_SYMBOL;
    s.u.value.u.symbol.size = strlen(TEST_DISPLAY_SYMBOL) + 1;
    s.u.value.u.symbol.len = strlen(TEST_DISPLAY_SYMBOL) + 1;
    fail_unless( 1 == plot_env_define(env, &(s.u.value.u.symbol), &(v.u.value)) );
    plot_func_display(env, &s, 1);
    puts(""); /* trailing \n */

    puts("\t\ttesting display of string (expected 'testing display of string')");
    s.type = plot_expr_value;
    s.u.value.type = plot_type_string;
#define TEST_DISPLAY_STRING "testing display of string"
    s.u.value.u.string.val = TEST_DISPLAY_STRING;
    s.u.value.u.string.size = strlen(TEST_DISPLAY_STRING) +1;
    s.u.value.u.string.len = strlen(TEST_DISPLAY_STRING) + 1;
    plot_func_display(env, &s, 1);
    puts(""); /* trailing \n */


    puts("\t\ttesting display of function");
    v.u.value.type = plot_type_function;
    v.u.value.u.function.env = 0;
    v.u.value.u.function.func = 0;
    plot_func_display(env, &v, 1);

    /* last as this will cause an exit(1) */
    puts("\t\ttesting display of error (error expected)");
    v.u.value.type = plot_type_error;
    v.u.value.u.error.type = plot_error_internal;
    v.u.value.u.error.msg = "testing display of error";
    v.u.value.u.error.location = "test_display";
    plot_func_display(env, &v, 1);
}
END_TEST


TEST_CASE_NEW(funcs)
TEST_CASE_ADD(funcs, funcs_math)
TEST_CASE_ADD(funcs, funcs_env)
TEST_CASE_END(funcs)

TEST_CASE_NEW(funcs_error)
    /* FIXME display of error will cause exit(1), functions can also not currently be displayed */
    tcase_add_exit_test(tc_funcs_error, test_display, 1);
    tcase_add_exit_test(tc_funcs_error, test_error_alloc_failed, 1);
    tcase_add_exit_test(tc_funcs_error, test_error_bad_args, 1);
    tcase_add_exit_test(tc_funcs_error, test_error_internal, 1);
    tcase_add_exit_test(tc_funcs_error, test_error_unbound_symbol, 1);
TEST_CASE_END(funcs_error)

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
struct plot_test_funcs_tests {
    plot_value *func;
    plot_value *args[2];
    union {
        const int expected;
        const bool expected_val;
    } u;
    const char *failure_msg;
};

/* plot test func expr value */
#define PTF_EV(e) {plot_expr_value, {.value=e}}

/* plot test value number */
#define PTF_VN(n) &(plot_value){{-1, 0}, plot_type_number, {.number={n}}}

/* plot test value boolean */
#define PTF_VBO(n) &(plot_value){{-1, 0}, plot_type_boolean, {.boolean={n}}}

/* plot test value string */
#define PTF_VST(n) &(plot_value){{-1, 0}, plot_type_string, {.string={n, 10, 10}}}

/* plot test value symbol */
#define PTF_VSY(n) &(plot_value){{-1, 0}, plot_type_symbol, {.symbol={n, 10, 10}}}

/* plot test builtin value funct */
#define PTF_VBU(f) &(plot_value){{-1, 0}, plot_type_builtin, {.builtin = {f}}}

/* length of array */
#define PTF_LENGTH(x) (sizeof x / sizeof x[0])

/* call builtin given index */
#define PTF_CALL_BUILTIN(i) bindings[i].func->u.builtin.func

/* yield args and len given index */
#define PTF_ARGS(i) bindings[i].args, PTF_LENGTH(bindings[i].args)

/* yield args list from index and provided len */
#define PTF_ARGS_LEN(i, len) bindings[i].args, len

/* yield expected value given index */
#define PTF_EXP(i) bindings[i].u.expected
#define PTF_EXP_VAL(i) bindings[i].u.expected_val

/* yield error message given index */
#define PTF_ERR(i) bindings[i].failure_msg

START_TEST (test_funcs_math){
    unsigned int i;
    plot_value *r;

    struct plot_test_funcs_tests bindings[] = {
        /* function                     ( args1,            arg2 )                   = expected            "failure message" */
        {PTF_VBU(plot_func_add),         { PTF_VN( 2),  PTF_VN( 3)  },  {.expected =  5},     "failed test for plot_func_add"},
        {PTF_VBU(plot_func_subtract),    { PTF_VN(10),  PTF_VN( 7)  },  {.expected =  3},     "failed test for plot_func_subtract"},
        {PTF_VBU(plot_func_multiply),    { PTF_VN( 5),  PTF_VN(15)  },  {.expected = 75},     "failed test for plot_func_multiply"},
        {PTF_VBU(plot_func_divide),      { PTF_VN(10),  PTF_VN( 3)  },  {.expected =  3},     "failed test for plot_func_divide"},
        {PTF_VBU(plot_func_remainder),   { PTF_VN(10),  PTF_VN( 3)  },  {.expected =  1},     "failed test for plot_func_remainder"}
    };

    puts("\ttesting math functions");

    fail_if( 0 == plot_init() );
    fail_if( 0 == plot_get_env() );

    for( i=0; i < PTF_LENGTH(bindings); ++i ){
        r = PTF_CALL_BUILTIN(i)( plot_get_env(), PTF_ARGS(i) );
        if( 0 == r ){
            puts(PTF_ERR(i));
            puts("received a NULL return value");
            ck_abort_msg(PTF_ERR(i));
        }
        fail_unless( r->type == plot_type_number );
        if( r->u.number.val != PTF_EXP(i) ){
            puts(PTF_ERR(i));
            printf("got '%d', expected '%d'\n", r->u.number.val, PTF_EXP(i));
            ck_abort_msg(PTF_ERR(i));
        }
    }

    plot_cleanup();
}
END_TEST

START_TEST (test_funcs_comparison){
    unsigned int i;
    const plot_value *r;

    struct plot_test_funcs_tests bindings[] = {
        /* function                             ( args1,               arg2 )                  = expected                   "failure message" */
        {PTF_VBU(plot_func_equal),               { PTF_VN( 1),  PTF_VN( 2)  },  {.expected_val = false},     "failed test for plot_func_equal"},
        {PTF_VBU(plot_func_less),                { PTF_VN( 3),  PTF_VN( 5)  },  {.expected_val = true},      "failed test for plot_func_less"},
        {PTF_VBU(plot_func_greater),             { PTF_VN( 1),  PTF_VN( 1)  },  {.expected_val = false},     "failed test for plot_func_greater"},
        {PTF_VBU(plot_func_less_equal),          { PTF_VN(10),  PTF_VN(10)  },  {.expected_val = true},      "failed test for plot_func_less_equal"},
        {PTF_VBU(plot_func_greater_equal),       { PTF_VN( 8),  PTF_VN( 7)  },  {.expected_val = true},      "failed test for plot_func_greater_equal"}
    };

    puts("\ttesting comparison functions");

    fail_if( 0 == plot_init() );
    fail_if( 0 == plot_get_env() );

    for( i=0; i < PTF_LENGTH(bindings); ++i ){
        r = PTF_CALL_BUILTIN(i)( plot_get_env(), PTF_ARGS(i) );
        if( 0 == r ){
            puts(PTF_ERR(i));
            puts("received a NULL return value");
            ck_abort_msg(PTF_ERR(i));
        }
        fail_unless( r->type == plot_type_boolean );
        if( r->u.boolean.val != PTF_EXP_VAL(i) ){
            puts(PTF_ERR(i));
            printf("got '%d', expected '%d'\n", r->u.number.val, PTF_EXP(i));
            ck_abort_msg(PTF_ERR(i));
        }
    }

    plot_cleanup();
}
END_TEST

START_TEST (test_funcs_value_tests){
    unsigned int i;
    const plot_value *r;

    struct plot_test_funcs_tests bindings[] = {
        /* function                             ( args1 )                   = expected                  "failure message" */
        {PTF_VBU(plot_func_boolean_test),        { PTF_VN( 1)},      {.expected_val = false},    "failed test for plot_func_boolean_test (negative)"},
        {PTF_VBU(plot_func_boolean_test),        { PTF_VBO( false)},  {.expected_val = true},     "failed test for plot_func_boolean_test (positive)"},

        {PTF_VBU(plot_func_string_test),         { PTF_VN( 3)},      {.expected_val = false},    "failed test for plot_func_string_test (negative)"},
        {PTF_VBU(plot_func_string_test),         { PTF_VST("hello")},{.expected_val = true},     "failed test for plot_func_string_test (positive)"},

/* FIXME TODO we cannot yet properly store a symbol as a value (would require quoting
 * so symbol? will always be false as it either fails to look up the symbol, or the value returned is not a symbol
 * this is correct behavior as per the scheme spec
 */
//        {PTF_VBU(plot_func_symbol_test),         { PTF_VST("NOPE")}, {.expected_val = false},    "failed test for plot_func_symbol_test (negative)"},
//        {PTF_VBU(plot_func_symbol_test),         { PTF_VSY("sym")},  {.expected_val = true},     "failed test for plot_func_symbol_test (positive)"},

        {PTF_VBU(plot_func_number_test),         { PTF_VBO(true)},    {.expected_val = false},    "failed test for plot_func_number_test (negative)"},
        {PTF_VBU(plot_func_number_test),         { PTF_VN(10)},      {.expected_val = true},     "failed test for plot_func_number_test (positive)"},

        {PTF_VBU(plot_func_procedure_test),       { PTF_VBO(true)},    {.expected_val = false},    "failed test for plot_func_procedure_test (negative)"},
        {PTF_VBU(plot_func_procedure_test),       { PTF_VBU(plot_func_procedure_test)},  {.expected_val = true},      "failed test for plot_func_procedure_test (positive)"}
    };

    puts("\ttesting comparison functions");

    fail_if( 0 == plot_init() );
    fail_if( 0 == plot_get_env() );

    for( i=0; i < PTF_LENGTH(bindings); ++i ){
        r = PTF_CALL_BUILTIN(i)( plot_get_env(), PTF_ARGS_LEN(i, 1) );
        if( 0 == r ){
            puts(PTF_ERR(i));
            puts("received a NULL return value");
            ck_abort_msg(PTF_ERR(i));
        }
        fail_unless( r->type == plot_type_boolean );
        if( r->u.boolean.val != PTF_EXP_VAL(i) ){
            puts(PTF_ERR(i));
            printf("got '%d', expected '%d'\n", r->u.number.val, PTF_EXP(i));
            ck_abort_msg(PTF_ERR(i));
        }
    }

    plot_cleanup();
}
END_TEST

#undef PTF_EV
#undef PTF_VN
#undef PTF_VBO
#undef PTF_VSY
#undef PTF_VST
#undef PTF_VBU
#undef PTF_LENGTH
#undef PTF_CALL_FUNC
#undef PTF_ARGS
#undef PTF_EXP
#undef PTF_ERR


START_TEST (test_funcs_env){
    plot_symbol sym;
    plot_value add;
    const plot_value *f;
    plot_env *env;

    fail_if( 0 == plot_init() );
    env = plot_new_env(0);


    sym.val = "+";
    sym.len = 2;
    sym.size = 2;

    add.type = plot_type_builtin;
    add.u.builtin.func = plot_func_add;
    add.gc.refcount = 1;
    puts("\tdefining function add");
    fail_unless( 1 == plot_env_define(env, &sym, &add) );

    puts("\ttesting fetching function");
    fail_if( 0 == (f = plot_env_get(env, &sym)) );
    fail_unless( f->type == plot_type_builtin );
    fail_unless( f->u.builtin.func == plot_func_add );

    plot_env_cleanup(env);
    plot_cleanup();
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
    plot_value *v, *s;

    puts("\n\tTesting display of values");
    fail_if( 0 == plot_init() );

    /* allocate our values once */
    v = plot_new_value();
    s = plot_new_value();

    puts("\t\ttesting display of number (expected '3')");
    v->type = plot_type_number;
    v->u.number.val = 3;
    plot_func_display(plot_get_env(), &v, 1);
    puts(""); /* trailing \n */

    puts("\t\ttesting display of symbol (expected '3')");
    s->type = plot_type_symbol;
#define TEST_DISPLAY_SYMBOL "symbol"
    s->u.symbol.val = TEST_DISPLAY_SYMBOL;
    s->u.symbol.size = strlen(TEST_DISPLAY_SYMBOL) + 1;
    s->u.symbol.len = strlen(TEST_DISPLAY_SYMBOL) + 1;
    fail_unless( 1 == plot_env_define(plot_get_env(), &(s->u.symbol), v) );
    plot_func_display(plot_get_env(), &s, 1);
    puts(""); /* trailing \n */

    puts("\t\ttesting display of string (expected 'testing display of string')");
    s->type = plot_type_string;
#define TEST_DISPLAY_STRING "testing display of string"
    s->u.string.val = TEST_DISPLAY_STRING;
    s->u.string.size = strlen(TEST_DISPLAY_STRING) +1;
    s->u.string.len = strlen(TEST_DISPLAY_STRING) + 1;
    plot_func_display(plot_get_env(), &s, 1);
    puts(""); /* trailing \n */


    puts("\t\ttesting display of function");
    v->type = plot_type_builtin;
    v->u.builtin.func = 0;
    plot_func_display(plot_get_env(), &v, 1);

    /* last as this will cause an exit(1) */
    puts("\t\ttesting display of error (error expected)");
    v->type = plot_type_error;
    v->u.error.type = plot_error_internal;
    v->u.error.msg = "testing display of error";
    v->u.error.location = "test_display";
    plot_func_display(plot_get_env(), &v, 1);

    plot_cleanup();
}
END_TEST


TEST_CASE_NEW(funcs)
TEST_CASE_ADD(funcs, funcs_math)
TEST_CASE_ADD(funcs, funcs_comparison)
TEST_CASE_ADD(funcs, funcs_value_tests)
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

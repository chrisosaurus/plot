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
#include "../src/env.h"
#include "../src/eval.h"

START_TEST (test_forms_define){
    plot_env *env;
    plot_expr expr;
    const plot_value *val;

    puts("\tTesting evaluation of simple define");
    env = plot_env_init(0);

    expr.type = plot_expr_sexpr;
    expr.u.sexpr.nchildren = 3;
    expr.u.sexpr.subforms = calloc(3, sizeof(plot_expr));
    fail_if( 0 == expr.u.sexpr.subforms );

    expr.u.sexpr.subforms[0].type = plot_expr_value;
    expr.u.sexpr.subforms[0].u.value.type = plot_type_symbol;
    expr.u.sexpr.subforms[0].u.value.u.symbol.val = "define";
    expr.u.sexpr.subforms[0].u.value.u.symbol.size = 7;
    expr.u.sexpr.subforms[0].u.value.u.symbol.len = 7;

    expr.u.sexpr.subforms[1].type = plot_expr_value;
    expr.u.sexpr.subforms[1].u.value.type = plot_type_symbol;
    expr.u.sexpr.subforms[1].u.value.u.symbol.val = "hello";
    expr.u.sexpr.subforms[1].u.value.u.symbol.size = 6;
    expr.u.sexpr.subforms[1].u.value.u.symbol.len = 6;

    expr.u.sexpr.subforms[2].type = plot_expr_value;
    expr.u.sexpr.subforms[2].u.value.type = plot_type_number;
    expr.u.sexpr.subforms[2].u.value.u.number.val = 7;

    plot_eval(env, &expr);

    fail_if( 0 == (val = plot_env_get(env, &(expr.u.sexpr.subforms[1].u.value.u.symbol)) ));
    fail_unless( val->type == plot_type_number );
    fail_unless( 7 == val->u.number.val );
}
END_TEST

TEST_CASE_NEW(forms)
TEST_CASE_ADD(forms, forms_define)
TEST_CASE_END(forms)


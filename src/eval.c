#include <stdio.h>
#include <stdlib.h>

#include "eval.h"

plot_value * plot_eval(plot_env *env, plot_expr *expr){
    if( !env || !expr )
        return 0; /* ERROR */

    switch( expr->type ){
        case plot_expr_value:
            return plot_eval_value(env, expr);
            break;
        case plot_expr_sexpr:
            return plot_eval_sexpr(env, expr);
            break;
        default:
            puts("ERROR: error in plot_eval: unknown plot_expr_type, returning");
            return 0;
            break;
    }

    return 0;
}

plot_value * plot_eval_value(plot_env *env, plot_expr *expr){
    if( !env || !expr )
        return 0; /* ERROR */

    if( expr->type != plot_expr_value )
        return 0; /* ERROR */

    switch( expr->u.value.type ){
        case plot_type_number:
            return &(expr->u.value);
            break;
        case plot_type_symbol:
            return plot_env_get(env, &(expr->u.value.u.symbol));
            break;
        case plot_type_function:
            return &(expr->u.value);
            break;
        default:
            puts("ERROR: error in plot_eval_value: unknown value type, returning");
            return 0;
            break;
    }

    return 0;
}

plot_value * plot_eval_sexpr(plot_env *env, plot_expr *expr){
    plot_value *res;

    if( !env || !expr )
        return 0; /* ERROR */

    if( expr->type != plot_expr_sexpr )
        return 0; /* ERROR */

    res = calloc(1, sizeof *res);
    if( !res )
        return 0; /* ERROR */

    /* TODO FIXME */

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

#include "eval.h"

#define DEBUG 0

/* evals an expr in an environment
 */
const plot_value * plot_eval(plot_env *env, const plot_expr * expr){
    if( !env || !expr )
        return 0; /* ERROR */

    #if DEBUG
    puts("inside plot_eval");
    #endif
    switch( expr->type ){
        case plot_expr_value:
            #if DEBUG
            puts("dispatching to plot_eval_value");
            #endif
            return plot_eval_value(env, &(expr->u.value));
            break;
        case plot_expr_sexpr:
            #if DEBUG
            puts("dispatching to plot_eval_sexpr");
            #endif
            return plot_eval_sexpr(env, &(expr->u.sexpr));
            break;
        default:
            puts("ERROR: error in plot_eval: unknown plot_expr_type, returning");
            return 0;
            break;
    }

    return 0;
}

/* if symbol then resolve in env, otherwise return value
 * cannot modify the env
 *
 * returned value cannot be freed, it is either the same value you passed in OR
 *  the value stored under that symbol in the env.
 */
const plot_value * plot_eval_value(const plot_env *env, const plot_value * val){
    if( !env || !val )
        return 0; /* ERROR */

            #if DEBUG
            puts("inside plot_eval_value");
            #endif

    switch( val->type ){
        case plot_type_symbol:
            #if DEBUG
            puts("symbol found, resolving: dispatching to plot_env_get");
            #endif
            return plot_env_get(env, &(val->u.symbol));
            break;
        default:
            #if DEBUG
            puts("value found, keeping");
            #endif
            return val;
            break;
    }

    return 0;
}

/* FIXME move elsewhere */
/* return 1 if string names a form
 * oterwise returns 0
 */
int plot_is_form(const char *name){
    return 0;
}

/* eval a sexpr in an environment
 * if sexpr is a form then plot_eval_form may be called which can
 *  modify the env
 */
const plot_value * plot_eval_sexpr(plot_env *env, const plot_sexpr * sexpr){
    const plot_value *res;

    if( ! env || ! sexpr )
        return 0; /* ERROR */

    #if DEBUG
    puts("inside plot_eval_sexpr");
    #endif

    /* TODO FIXME this is all horribly hacky and error-prone */

    /* can either be a form or a function lookup
     * NB: the first value could be compound (itself an s_expr)
     */

    if( sexpr->nchildren && sexpr->subforms[0].type == plot_expr_sexpr ){
        #if DEBUG
        puts("compound s_expr, dispatching to plot_eval_sexpr");
        #endif
        res = plot_eval_sexpr( env, &(sexpr->subforms[0].u.sexpr) );
    } else {
        #if DEBUG
        puts("value found, assigning address");
        #endif
        /* must be a value */
        res = &(sexpr->subforms[0].u.value);
    }

    if( ! res )
        return 0; /* ERROR */

    /* check for form */
    if( res->type == plot_type_symbol && plot_is_form(res->u.symbol.val) ){
        #if DEBUG
        puts("form detected, doing nothing...");
        #endif
    } else {
        /* must be a function or a symbol that resolves to a function */

        /* if this is a symbol unpack it for us
         * otherwise this will leave it unchanged
         */
        res = plot_eval_value(env, res);
        switch( res->type ){
            case plot_type_function:
                break;
            case plot_type_symbol:
                break;
            case plot_type_number:
                break;
        }
        if( res->type != plot_type_function ){
            puts("ERROR in plot_eval_sexpr; function expected but found another type, returning");
            return 0;
        }

        #if DEBUG
        puts("calling function");
        printf("env is '%x', v1 is '%x', v2 is '%x'\n", res->u.function.env, &(sexpr->subforms[1].u.value), &(sexpr->subforms[2].u.value) );
        #endif
        /* call function */
        res = res->u.function.func( res->u.function.env ? res->u.function.env : env,
                                    &(sexpr->subforms[1].u.value),
                                    &(sexpr->subforms[2].u.value)); /* FIXME need to generalise */
        return res;
    }

        #if DEBUG
        puts("returning 0...");
        #endif

    return 0;
}

/* eval a form in an environment
 * can modify the environment (e.g. define)
 */
const plot_value * plot_eval_form(plot_env *env, const plot_sexpr * sexpr){
    if( ! env || ! sexpr )
        return 0; /* ERROR */

    /* TODO FIXME */

    return 0;
}

/* eval a function call in an environment
 */
const plot_value * plot_eval_func_call(const plot_env *env, const plot_sexpr * sexpr){
    if( ! env || ! sexpr )
        return 0; /* ERROR */

    /* TODO FIXME */

    return 0;
}


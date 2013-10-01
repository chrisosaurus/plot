#include <stdio.h>
#include <stdlib.h>

#include "eval.h"

plot_value * plot_eval(plot_env *env, plot_expr *expr){
    if( !env || !expr )
        return 0; /* ERROR */

    switch( expr->type ){
        case plot_expr_value:
            return plot_eval_value(env, &(expr->u.value));
            break;
        case plot_expr_sexpr:
            return plot_eval_sexpr(env, &(expr->u.sexpr));
            break;
        default:
            puts("ERROR: error in plot_eval: unknown plot_expr_type, returning");
            return 0;
            break;
    }

    return 0;
}

plot_value * plot_eval_value(plot_env *env, plot_value *val){
    if( !env || !val )
        return 0; /* ERROR */

    switch( val->type ){
        case plot_type_symbol:
            return plot_env_get(env, &(val->u.symbol));
            break;
        default:
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

plot_value * plot_eval_sexpr(plot_env *env, plot_sexpr *sexpr){
    plot_value *res;

    if( ! env || ! sexpr )
        return 0; /* ERROR */

    res = calloc(1, sizeof *res);
    if( !res )
        return 0; /* ERROR */

    /* TODO FIXME this is all horribly hacky and error-prone */

    /* can either be a form or a function lookup
     * NB: the first value could be compound (itself an s_expr)
     */

    if( sexpr->nchildren && sexpr->subforms[0].type == plot_expr_sexpr ){
        res = plot_eval_sexpr( env, &(sexpr->subforms[0].u.sexpr) );
    } else {
        /* must be a value */
        res = &(sexpr->subforms[0].u.value);
    }

    if( ! res )
        return 0; /* ERROR */

    /* check for form */
    if( res->type == plot_type_symbol && plot_is_form(res->u.symbol.val) ){
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

        /* call function */
        res = res->u.function.func( res->u.function.env,
                                    &(sexpr->subforms[1].u.value),
                                    &(sexpr->subforms[2].u.value)); /* FIXME need to generalise */
        return res;
    }

    return 0;
}

plot_value * plot_eval_form(plot_env *env, plot_sexpr *sexpr){
    if( ! env || ! sexpr )
        return 0; /* ERROR */

    /* TODO FIXME */

    return 0;
}

plot_value * plot_eval_func_call(plot_env *env, plot_sexpr *sexpr){
    if( ! env || ! sexpr )
        return 0; /* ERROR */

    /* TODO FIXME */

    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strcmp */

#include "value.h"
#include "env.h"
#include "types.h"
#include "eval.h"
#include "funcs.h"

#define DEBUG 0

/* evals an expr in an environment
 */
const plot_value * plot_eval(plot_env *env, const plot_expr * expr){
    plot_value err;

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
            /* TODO FIXME most likely a bad error usage pattern, revise later */
            err.type = plot_type_error;
            err.u.error.type = plot_error_internal;
            err.u.error.msg - "unknown plot_expr_type";
            /* plot_handle_error will not return */
            plot_handle_error(&err, "plot_eval");
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
const plot_value * plot_eval_value(plot_env *env, const plot_value * val){
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
static int plot_is_form(const plot_sexpr * sexpr){
    const plot_value *val;
    if( ! sexpr )
        return 0;

    if( ! sexpr->nchildren )
        return 0;

    if( sexpr->subforms[0].type != plot_expr_value )
        return 0;

    val = &(sexpr->subforms[0].u.value);
    switch( val->type){
        case plot_type_symbol:
            if( ! strcmp(val->u.symbol.val, "define") )
                return 1;
            if( ! strcmp(val->u.symbol.val, "lambda") )
                return 1;
            break;
        default:
            break;
    }

    return 0;
}

/* eval a sexpr in an environment
 * if sexpr is a form then plot_eval_form may be called which can
 *  modify the env
 */
const plot_value * plot_eval_sexpr(plot_env *env, const plot_sexpr * sexpr){
    if( ! env || ! sexpr )
        return 0; /* ERROR */

    #if DEBUG
    puts("inside plot_eval_sexpr");
    #endif

    /* if first child is an s_expr then it must be a function call
     * if first child is a symbol we have 2 cases:
     *      is_form? -> eval_form
     *      else     -> eval func_call
     */
    if( ! sexpr->nchildren ){
        return 0; /* FIXME error, empty s-expr */
    }

    if( plot_is_form(sexpr) )
        return plot_eval_form(env, sexpr);

    return plot_eval_func_call(env, sexpr);
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
const plot_value * plot_eval_func_call(plot_env *env, const plot_sexpr * sexpr){
    const plot_value *val;
    const plot_value *func;

    if( ! env || ! sexpr )
        return 0; /* FIXME ERROR */

    if( ! sexpr->nchildren )
        return 0; /* FIXME ERROR */

    if( sexpr->subforms[0].type == plot_expr_sexpr ){
        puts("Sorry compound sexpr(s) are currently not supported");
        return 0; /* FIXME ERROR */
    }

    if( sexpr->subforms[0].type == plot_expr_sexpr ){
        puts("ERROR: impossible sexpr subform [0] type");
        return 0; /* FIXME ERROR */
    }

    val = &(sexpr->subforms[0].u.value);

    switch( val->type ){
        case plot_type_symbol:
            func = plot_eval_value(env, val);
            if( func->type != plot_type_function ){
                puts("ERROR: unknown syntax");
                return 0; /* FIXME ERROR */
            }
            return func->u.function.func( func->u.function.env ? func->u.function.env : env,
                                          sexpr->subforms + 1,
                                          sexpr->nchildren - 1);
            break;
        case plot_type_function:
            puts("Sorry function values are not currently supported");
            return 0; /* FIXME ERROR */
            break;
        default:
            puts("ERROR: unknown syntax");
            return 0; /* FIXME ERROR */
            break;
    }

    return 0;
}


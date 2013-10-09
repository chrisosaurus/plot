#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strcmp */

#include "value.h"
#include "env.h"
#include "types.h"
#include "eval.h"
#include "funcs.h"

#define DEBUG 0
#define DEBUG_EVAL 0
#define DEBUG_EXPR 0
#define DEBUG_SEXPR 0
#define DEBUG_VALUE 0
#define DEBUG_FUNC 0
#define DEBUG_FORM 0


/* evals a prog in an environment
 * returns 1 for success, 0 for error
 */
int plot_eval(plot_env *env, const plot_program * prog){
    plot_expr *expr;
    int i=0;

    #if DEBUG_EVAL || DEBUG
    puts("\ninside plot_eval");
    #endif

    if( ! env || ! prog )
        return 0; /* FIXME ERROR */

    if( ! prog->nchildren )
        return 0; /* FIXME nothing to do */

    for( i=0, expr = prog->exprs; i<prog->nchildren; ++i, expr = expr +1){
        #if DEBUG_EVAL || DEBUG
        printf("\ngoing through expr child (i) '%d' of (nchildren) '%d'\n", i, prog->nchildren);
        #endif
        plot_eval_expr(env, expr);
    }

    return 1; /* what is the return value of a program? success of error? */
}

/* evals an expr in an environment
 */
const plot_value * plot_eval_expr(plot_env *env, const plot_expr * expr){
    plot_value err;

    if( !env || !expr )
        return 0; /* ERROR */

    #if DEBUG_EXPR || DEBUG
    puts("inside plot_eval_expr");
    #endif
    switch( expr->type ){
        case plot_expr_value:
            #if DEBUG_EXPR || DEBUG
            puts("dispatching to plot_eval_value");
            #endif
            return plot_eval_value(env, &(expr->u.value));
            break;
        case plot_expr_sexpr:
            #if DEBUG_EXPR || DEBUG
            puts("dispatching to plot_eval_sexpr");
            #endif
            return plot_eval_sexpr(env, &(expr->u.sexpr));
            break;
        default:
            /* TODO FIXME most likely a bad error usage pattern, revise later */
            err.type = plot_type_error;
            err.u.error.type = plot_error_internal;
            err.u.error.msg = "unknown plot_expr_type";
            err.u.error.location = "plot_eval";
            /* plot_handle_error will not return */
            plot_handle_error(&err);
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

            #if DEBUG_VALUE || DEBUG
            puts("inside plot_eval_value");
            #endif

    switch( val->type ){
        case plot_type_symbol:
            #if DEBUG_VALUE || DEBUG
            puts("symbol found, resolving: dispatching to plot_env_get");
            #endif
            return plot_env_get(env, &(val->u.symbol));
            break;
        default:
            #if DEBUG_VALUE || DEBUG
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
    #if DEBUG_FORM || DEBUG
    puts("inside plot_is_form");
    #endif

    if( ! sexpr )
        return 0;

    if( ! sexpr->nchildren )
        return 0;

    if( sexpr->subforms[0].type != plot_expr_value )
        return 0;

    val = &(sexpr->subforms[0].u.value);
    switch( val->type){
        case plot_type_symbol:
            if( ! strcmp(val->u.symbol.val, "define") ){
                #if DEBUG_FORM || DEBUG
                puts("define form found");
                #endif
                return 1;
                }
            if( ! strcmp(val->u.symbol.val, "lambda") ){
                #if DEBUG_FORM || DEBUG
                puts("lambda form found");
                #endif
                return 1;
                }
            break;
        default:
            break;
    }

    #if DEBUG_FORM || DEBUG
    puts("no form found");
    #endif
    return 0;
}

/* eval a sexpr in an environment
 * if sexpr is a form then plot_eval_form may be called which can
 *  modify the env
 */
const plot_value * plot_eval_sexpr(plot_env *env, const plot_sexpr * sexpr){
    if( ! env || ! sexpr )
        return 0; /* ERROR */

    #if DEBUG_SEXPR || DEBUG
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
    const plot_value *form;
    const plot_value *name;
    const plot_value *value;

    #if DEBUG_SEXPR || DEBUG
    puts("inside plot_eval_form");
    #endif

    if( ! env || ! sexpr ){
        #if DEBUG_SEXPR || DEBUG
        puts("DEFINE: bad args");
        #endif
        return 0; /* FIXME ERROR */
    }

    if( ! sexpr->nchildren ){
        #if DEBUG_SEXPR || DEBUG
        puts("DEFINE: bad nchildren");
        #endif
        return 0; /* FIXME ERROR */
    }

    if( sexpr->subforms[0].type != plot_expr_value ){
        #if DEBUG_SEXPR || DEBUG
        puts("DEFINE: bad subform[0] type");
        #endif
        return 0; /* FIXME ERROR */
    }

    form = &(sexpr->subforms[0].u.value);
    switch( form->type){
        case plot_type_symbol:
            if( ! strcmp(form->u.symbol.val, "define") ){
                if( sexpr->nchildren != 3 ){
                    #if DEBUG_SEXPR || DEBUG
                    printf("DEFINE: incorrect number of children '%d'\n", sexpr->nchildren);
                    #endif
                    return 0; /* FIXME ERROR */
                }

                if( sexpr->subforms[1].type != plot_expr_value ){
                    #if DEBUG_SEXPR || DEBUG
                    puts("DEFINE: incorrect 1st arg expr type");
                    #endif
                    return 0; /* FIXME ERROR */
                }
                /* do we even know this is a value yet ? */
                name = &(sexpr->subforms[1].u.value);
                if( name->type != plot_type_symbol ){
                    #if DEBUG_SEXPR || DEBUG
                    puts("DEFINE: incorrect 1st arg value type");
                    #endif
                    return 0; /* FIXME ERROR */
                }

                /* 2nd subform isnt known to be a value ! */
                value = plot_eval_expr(env, &(sexpr->subforms[2]));
                if( ! value ){
                    #if DEBUG_SEXPR || DEBUG
                    puts("DEFINE: faled to eval_value");
                    #endif
                    return 0; /* FIXME ERROR */
                }

                #if DEBUG_SEXPR || DEBUG
                puts("DEFINE: success!");
                printf("Storing value type '%d', under name '%s'\n'", value->type, name->u.symbol.val);
                #endif
                plot_env_define(env, &(name->u.symbol), value);
                return 0;
            }
            if( ! strcmp(form->u.symbol.val, "lambda") ){
                #if DEBUG_SEXPR || DEBUG
                puts("'lambda' form is not currently implemented");
                #endif
                return 0; /* FIXME ERROR */
            }
            break;
        default:
            break;
    }

    #if DEBUG_SEXPR || DEBUG
    puts("leaving plot_eval_form");
    #endif

    return 0; /* FIXME ERROR */
}

/* eval a function call in an environment
 */
const plot_value * plot_eval_func_call(plot_env *env, const plot_sexpr * sexpr){
    const plot_value *val;
    const plot_value *func;

    #if DEBUG_FUNC || DEBUG
    puts("inside plot_eval_func_call");
    #endif

    if( ! env || ! sexpr )
        #if DEBUG_FUNC || DEBUG
        puts("plot_eval_func_call: bad args");
        #endif
        return 0; /* FIXME ERROR */

    if( ! sexpr->nchildren )
        #if DEBUG_FUNC || DEBUG
        puts("plot_eval_func_call: no children");
        #endif
        return 0; /* FIXME ERROR */

    if( sexpr->subforms[0].type == plot_expr_sexpr ){
        #if DEBUG_FUNC || DEBUG
        puts("plot_eval_func_call: subform is an sexpr");
        #endif
        puts("Sorry compound sexpr(s) are currently not supported");
        return 0; /* FIXME ERROR */
    }

    if( sexpr->subforms[0].type == plot_expr_sexpr ){
        #if DEBUG_FUNC || DEBUG
        puts("plot_eval_func_call: subform is an sexpr");
        #endif
        puts("ERROR: impossible sexpr subform [0] type");
        return 0; /* FIXME ERROR */
    }

    val = &(sexpr->subforms[0].u.value);

    switch( val->type ){
        case plot_type_symbol:
            func = plot_eval_value(env, val);

            if( ! func ){
                printf("plot_eval_func_call: no function found for '%s', bailing\n", val->u.symbol.val);
                return 0; /* FIXME ERROR */
            }
            #if DEBUG_FUNC || DEBUG
            puts("got a function...");
            #endif
            if( func->type != plot_type_function ){
                puts("ERROR: unknown syntax");
                return 0; /* FIXME ERROR */
            }

            #if DEBUG_FUNC || DEBUG
            puts("calling function");
            #endif
            return func->u.function.func( func->u.function.env ? func->u.function.env : env,
                                          sexpr->subforms + 1,
                                          sexpr->nchildren - 1);
            break;
        case plot_type_function:
            #if DEBUG_FUNC || DEBUG
            puts("plot_eval_func_cal: function values are not currently supported");
            #endif
            puts("Sorry function values are not currently supported");
            return 0; /* FIXME ERROR */
            break;
        default:
            #if DEBUG_FUNC || DEBUG
            puts("plot_eval_func_call: unknown syntax");
            #endif
            puts("ERROR: unknown syntax");
            return 0; /* FIXME ERROR */
            break;
    }

    return 0;
}


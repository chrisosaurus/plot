#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strcmp */

#include "value.h"
#include "hash.h"
#include "env.h"
#include "eval.h"
#include "funcs.h"
#include "plot.h"

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
int plot_eval(plot_env *env, plot_value * prog){
    plot_value *val;
    plot_value *cur;

    #if DEBUG_EVAL || DEBUG
    puts("\ninside plot_eval");
    #endif

    if( ! env || ! prog )
        return 0; /* FIXME ERROR */

    for( cur = prog; cur->type != plot_type_null; cur = cdr(cur)){
        val = plot_eval_expr(env, car(cur));
        if( val ){
            if( val->type == plot_type_error ){
                puts("plot_eval");
                plot_value_decr(val);
                exit(1);
            }
            plot_value_decr(val);
        }
    }

    return 1; /* what is the return value of a program? success of error? */
}

/* evals an expr in an environment
 */
plot_value * plot_eval_expr(plot_env *env, plot_value * expr){
    plot_value *res;
    if( !env || !expr )
        return 0; /* ERROR */

    #if DEBUG_EXPR || DEBUG
    puts("inside plot_eval_expr");
    #endif

    if( expr->type == plot_type_pair){
        #if DEBUG_EXPR || DEBUG
        puts("\tdispatching to plot_eval_sexpr");
        #endif
        res = plot_eval_sexpr(env, expr);
        if( res && res->type == plot_type_error ){
            puts("plot_eval_expr (sexpr)");
            display_error_expr(expr);
        }
        return res;
    } else { /* value */
        #if DEBUG_EXPR || DEBUG
        puts("\tdispatching to plot_eval_value");
        #endif
        res = plot_eval_value(env, expr);
        if( res && res->type == plot_type_error ){
            puts("plot_eval_expr (value)");
            display_error_expr(expr);
        }
        return res;
    }

    return 0;
}

/* if symbol then resolve in env, otherwise return value
 * cannot modify the env
 *
 * returned value cannot be freed, it is either the same value you passed in OR
 *  the value stored under that symbol in the env.
 */
plot_value * plot_eval_value(plot_env *env, plot_value * val){
    plot_value *res;
    if( !env || !val )
        return 0; /* ERROR */

    #if DEBUG_VALUE || DEBUG
    puts("inside plot_eval_value");
    #endif

    if( val->type == plot_type_pair ){
        plot_fatal_error("plot_eval_value: BOOM");
    }

    switch( val->type ){
        case plot_type_symbol:
            #if DEBUG_VALUE || DEBUG
            printf("\tsymbol found '%s', resolving: dispatching to plot_env_get\n", val->u.symbol.val);
            #endif
            res = plot_env_get(env, &(val->u.symbol));
            if( ! res ){
                return plot_runtime_error(plot_error_unbound_symbol, val->u.symbol.val, "plot_eval_value");
            }
            if( res && res->type == plot_type_error ){
                puts("plot_eval_value (symbol)");
            }
            return res;
            break;
        default:
            #if DEBUG_VALUE || DEBUG
            puts("\tvalue found, keeping");
            #endif
            return val;
            break;
    }

    return 0;
}

/* eval a sexpr in an environment
 * if sexpr is a form then plot_eval_form may be called which can
 *  modify the env
 */
plot_value * plot_eval_sexpr(plot_env *env, plot_value * sexpr){
    plot_value *res;
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

    res = plot_eval_form(env, sexpr);
    if( res && res->type == plot_type_error ){
        puts("plot_eval_sexpr (func call)");
        display_error_expr(sexpr);
    }
    return res;
}

/* eval a form in an environment
 * a form could either be a syntactic form such as `define`
 * or it could be a function call
 *
 * may modify the env (e.g. `define`)
 */
plot_value * plot_eval_form(plot_env *env, plot_value * sexpr){
    plot_value *val = 0;
    plot_value **vals;
    plot_value *func;
    plot_env *new_env;
    plot_value *cur, *curarg;
    int len;
    int i;

    #if DEBUG_FUNC || DEBUG
    puts("inside plot_eval_form");
    #endif

    if( ! env || ! sexpr ){
        #if DEBUG_FUNC || DEBUG
        puts("\tplot_eval_form: bad args");
        #endif
        return 0; /* FIXME ERROR */
    }

    if( sexpr->type != plot_type_pair ){
        plot_fatal_error("plot_eval_form: boom");
    }

    func = plot_eval_expr(env, car(sexpr));
    if( ! func ){
        return plot_runtime_error(plot_error_internal, "evaluating value in function call position returned null", "plot_eval_form");
    }

    switch( func->type ){
        case plot_type_error:
            puts("plot_eval_form (func)");
            return func;
            break;

        case plot_type_legacy:
            #if DEBUG_FUNC || DEBUG
            puts("\tcalling a legacy builtin function...");
            #endif

            /* FIXME dirty shim will be removed*/
            for( len=0, cur=cdr(sexpr); cur->type != plot_type_null ; cur = cdr(cur)) {
                ++len;
            }
            vals = calloc( len, sizeof *vals);
            /* eval each argument and add to list */
            for( i=0, cur=cdr(sexpr); cur->type != plot_type_null; ++i, cur = cdr(cur) ){
                val = plot_eval_expr(env, car(cur));
                if( ! val ){
                    display_error_expr(car(cur));
                    return plot_runtime_error(plot_error_internal, "LEGACY BUILTIN call: evaluating argument returned NULL", "plot_eval_form");
                }
                if( val->type == plot_type_error ){
                    display_error_expr(cur);
                    return val;
                }
                vals[i] = val;
            }
            val = func->u.legacy.func( env, vals, len);
            for( i=0; i < len; ++i ){
                plot_value_decr(vals[i]);
            }
            /* FIXME also dirty */
            free(vals);
            return val;
        case plot_type_form:
            #if DEBUG_FUNC || DEBUG
            puts("calling syntactic with sexpr:");
            display_error_expr(sexpr);
            #endif

            /* form.syntactic is true IFF this is a syntactic form
             *  syntactic forms received their arguments pre-evaluation
             *  normal forms have their arguments evaluated before seeing them
             */
            if( func->u.form.syntactic ){
                /* use cdr here to 'trim' the first symbol (e.g. `lambda`) */
                return func->u.form.func( env, cdr(sexpr) );
            } else {
                /* val is the head of the list of args we pass into the function */
                val = null;
                /* vals is our current tail */
                vals = &val;
                /* we use curarg for the current argument we are evaling
                 * before placing it into vals
                 */
                for( curarg = cdr(sexpr); curarg->type == plot_type_pair; curarg = cdr(curarg) ){
                    /* eval and shove into list */
                    *vals = cons(0, null);
                    car(*vals) = plot_eval_expr(env, car(curarg));
                    vals = &cdr(*vals);
                }
                /* reusing cur, poorly named */
                cur = func->u.form.func( env, val );
                /* need to cleanup created list */
                plot_value_decr(val);
                return cur;
            }
            break;
        case plot_type_lambda:
            #if DEBUG_FUNC || DEBUG
            puts("\tcalling lambda");
            #endif

            /* create a new env with lambda->env as parent */
            new_env = plot_alloc_env(func->u.lambda.env);
            if( ! new_env ){
                puts("\tLAMBDA: call to plot_env_init failed");
                return 0; /* FIXME error */
            }

            #if DEBUG || DEBUG_FORM
            puts("evaluating lambda: ");
            display_error_expr(func->u.lambda.body);
            #endif

            /* for each parameter grab an argument
             *  if no argument then error
             *  define(new-env, parameter, argument)
             */
            curarg = cdr(sexpr);
            for( cur = car(func->u.lambda.body); cur->type != plot_type_null; cur = cdr(cur) ){
                if( car(curarg)->type == plot_type_null ){
                    /* FIXME NO ARG */
                }

                #if DEBUG || DEBUG_FORM
                puts("cur");
                display_error_expr(cur);
                puts("curarg");
                display_error_expr(curarg);
                #endif

                val = plot_eval_expr(env, car(curarg));
                curarg = cdr(curarg);
                if( ! val ){
                    puts("\tLAMBDA: evaluating argument returned NULL");
                    return 0; /* FIXME error */
                }
                if( val->type == plot_type_error ){
                    puts("plot_eval_form (lambda arg)");
                    return val;
                }
                if( ! plot_env_define(new_env, &(car(cur)->u.symbol), val) ){
                    puts("\tLAMBDA: failed to define argument");
                    return 0; /* FIXME error */
                }
                /* we are no longer holding a reference so decr */
                plot_value_decr(val);
            }

            /* eval each part of the body in new_env
             * return value of final expr
             */
            val = 0;
            for( cur = cdr(func->u.lambda.body); cur->type != plot_type_null; cur = cdr(cur) ){
                #if DEBUG || DEBUG_FORM
                puts("body part");
                display_error_expr(car(cur));
                #endif
                if( val ){
                    plot_value_decr(val);
                }
                val = plot_eval_expr(new_env, car(cur) );
                if( val && val->type == plot_type_error ){
                    puts("plot_eval_form (lambda body)");
                    return val;
                }
            }
            plot_env_decr(new_env);
            /* only decr if the function was received by resolving a symbol
             * as fetching from an env causes an incr
             */
            if( car(sexpr)->type == plot_type_symbol ){
                plot_value_decr(func);
            }
            //printf("after lambda we have '%d' refs\n", func->gc.refcount);
            //printf("and our env has '%d'\n", func->u.lambda.env->gc.refcount);
            return val;

            break;
        default:
            val = plot_runtime_error(plot_error_runtime, "trying to call non-function", "plot_eval_form");
            if( func->type == plot_type_symbol ){
                printf("Trying to call literal symbol '%s'\n", func->u.symbol.val);
            } else {
                printf("Trying to call value: ");
                plot_func_display(env, func);
                puts("");
            }
            display_error_expr(sexpr);
            return val;
            break;
    }
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strcmp */

#include "value.h"
#include "hash.h"
#include "env.h"
#include "types.h"
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

/* FIXME some rather ugly error display
 * tidy this up
 */
static void display_expr(plot_expr *expr);

static void display_sexpr(plot_sexpr * sexpr){
    int i;

    fputs("(", stdout);
    for(i = 0; i < sexpr->nchildren; ++i){
        display_expr(&sexpr->subforms[i]);
        if( i < sexpr->nchildren - 1 ){
            fputs(" ", stdout);
        }
    }
    fputs(")", stdout);
}

static void display_expr(plot_expr *expr){
    if( expr->type == plot_expr_sexpr ){
        display_sexpr(&expr->u.sexpr);
    } else {
        plot_func_display(0, &expr->u.value , 1);
    }
}

static void display_error_sexpr(plot_sexpr *sexpr){
    fputs(">\t", stdout);
    display_sexpr(sexpr);
    fputs("\n", stdout);
}

static void display_error_expr(plot_expr *expr){
    fputs(">\t", stdout);
    display_expr(expr);
    fputs("\n", stdout);
}

/* evals a prog in an environment
 * returns 1 for success, 0 for error
 */
int plot_eval(plot_env *env, plot_program * prog){
    plot_expr * expr;
    int i=0;
    plot_value * val;

    #if DEBUG_EVAL || DEBUG
    puts("\ninside plot_eval");
    #endif

    if( ! env || ! prog )
        return 0; /* FIXME ERROR */

    if( ! prog->nchildren )
        return 0; /* FIXME nothing to do */

    for( i=0, expr = prog->exprs; i<prog->nchildren; ++i, expr = expr +1){
        #if DEBUG_EVAL || DEBUG
        printf("\n\tgoing through expr child (i) '%d' of (nchildren) '%d'\n", i, prog->nchildren);
        #endif
        val = plot_eval_expr(env, expr);
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
plot_value * plot_eval_expr(plot_env *env, plot_expr * expr){
    plot_value *res;
    if( !env || !expr )
        return 0; /* ERROR */

    #if DEBUG_EXPR || DEBUG
    puts("inside plot_eval_expr");
    #endif
    switch( expr->type ){
        case plot_expr_value:
            #if DEBUG_EXPR || DEBUG
            puts("\tdispatching to plot_eval_value");
            #endif
            res = plot_eval_value(env, expr->u.value);
            if( res && res->type == plot_type_error ){
                puts("plot_eval_expr (value)");
                display_error_expr(expr);
            }
            return res;
            break;
        case plot_expr_sexpr:
            #if DEBUG_EXPR || DEBUG
            puts("\tdispatching to plot_eval_sexpr");
            #endif
            res = plot_eval_sexpr(env, &(expr->u.sexpr));
            if( res && res->type == plot_type_error ){
                puts("plot_eval_expr (sexpr)");
                display_error_expr(expr);
            }
            return res;
            break;
        default:
            return plot_runtime_error(plot_error_internal, "unknown plot_expr_type", "plot_eval");
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
plot_value * plot_eval_value(plot_env *env, plot_value * val){
    plot_value *res;
    if( !env || !val )
        return 0; /* ERROR */

    #if DEBUG_VALUE || DEBUG
    puts("inside plot_eval_value");
    #endif

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
plot_value * plot_eval_sexpr(plot_env *env, plot_sexpr * sexpr){
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
    if( ! sexpr->nchildren ){
        return 0; /* FIXME error, empty s-expr */
    }

    res = plot_eval_form(env, sexpr);
    if( res && res->type == plot_type_error ){
        puts("plot_eval_sexpr (func call)");
        display_error_sexpr(sexpr);
    }
    return res;
}

/* eval a form in an environment
 * a form could either be a syntactic form such as `define`
 * or it could be a function call
 *
 * may modify the env (e.g. `define`)
 */
plot_value * plot_eval_form(plot_env *env, plot_sexpr * sexpr){
    plot_value *val = 0;
    plot_value **vals;
    plot_value *func;
    plot_env *new_env;
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

    if( ! sexpr->nchildren ){
        #if DEBUG_FUNC || DEBUG
        puts("\tplot_eval_form: no children");
        #endif
        return 0; /* FIXME ERROR */
    }

    func = plot_eval_expr(env, &(sexpr->subforms[0]));
    if( ! func ){
        return plot_runtime_error(plot_error_internal, "evaluating value in function call position returned null", "plot_eval_form");
    }

    switch( func->type ){
        case plot_type_error:
            puts("plot_eval_form (func)");
            return func;
            break;

        case plot_type_builtin:
            #if DEBUG_FUNC || DEBUG
            puts("\tcalling a builtin function...");
            #endif

            /* FIXME dirty */
            vals = calloc( sexpr->nchildren - 1, sizeof *vals);
            /* eval each argument and add to list */
            for( i=0; i < sexpr->nchildren - 1; ++i ){
                val = plot_eval_expr(env, &(sexpr->subforms[1 + i]));
                if( ! val ){
                    return plot_runtime_error(plot_error_internal, "BUILTIN call: evaluating argument returned NULL", "plot_eval_form");
                }
                if( val->type == plot_type_error ){
                    puts("plot_eval_form (arg)");
                    display_error_expr(&sexpr->subforms[1 + i]);
                    return val;
                }
                vals[i] = val;
            }
            val = func->u.builtin.func( env, vals, sexpr->nchildren - 1);
            for( i=0; i < sexpr->nchildren - 1; ++i ){
                plot_value_decr(vals[i]);
            }
            /* FIXME also dirty */
            free(vals);
            return val;
        case plot_type_syntactic:
            return func->u.syntactic.func( env, sexpr );
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

            if( func->u.lambda.body->subforms[1].type != plot_expr_sexpr ){
                puts("\tLAMBDA: plot_eval_form parameter list is not an sexpr");
                return 0; /* FIXME error */
            }

            /* check number of arguments match number of parameters */
            if( func->u.lambda.body->subforms[1].u.sexpr.nchildren != sexpr->nchildren - 1 ){
                printf("\tLAMBDA: incorrect number of arguments, expected '%d' and got '%d'\n", func->u.lambda.body->subforms[1].u.sexpr.nchildren, sexpr->nchildren - 1);
            }
            /* for each parameter grab an argument
             *  if no argument then error
             *  define(new-env, parameter, argument)
             */
            for( i=0; i< func->u.lambda.body->subforms[1].u.sexpr.nchildren ; ++i ){
                if( func->u.lambda.body->subforms[1].u.sexpr.subforms[i].type != plot_expr_value ){
                    puts("\tLAMBDA: expected value");
                    return 0; /* FIXME error */
                }
                if( func->u.lambda.body->subforms[1].u.sexpr.subforms[i].u.value->type != plot_type_symbol ){
                    puts("\tLAMBDA: expected symbol");
                    return 0; /* FIXME error */
                }
                val = plot_eval_expr(env, &(sexpr->subforms[1+i]));
                if( ! val ){
                    puts("\tLAMBDA: evaluating argument returned NULL");
                    return 0; /* FIXME error */
                }
                if( val->type == plot_type_error ){
                    puts("plot_eval_form (lambda arg)");
                    return val;
                }
                if( ! plot_env_define(new_env, &(func->u.lambda.body->subforms[1].u.sexpr.subforms[i].u.value->u.symbol), val) ){
                    puts("\tLAMBDA: failed to define argument");
                    return 0; /* FIXME error */
                }
                /* we are no longer holding a reference so decr */
                plot_value_decr(val);
            }

            /* eval each part of the body in new_env
             * return value of final expr
             */
            for( i=2; i < func->u.lambda.body->nchildren; ++i ){
                val = plot_eval_expr(new_env, &(func->u.lambda.body->subforms[i]) );
                if( val && val->type == plot_type_error ){
                    puts("plot_eval_form (lambda body)");
                    return val;
                }
            }
            plot_env_decr(new_env);
            plot_value_decr(func);
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
                plot_func_display(env, &func, 1);
                puts("");
            }
            display_error_expr(&sexpr->subforms[0]);
            return val;
            break;
    }
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strcmp */

#include "value.h"
#include "hash.h"
#include "env.h"
#include "eval.h"
#include "output.h"
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
        case plot_type_null:
            /* a null should never be evaluated
             * it must be quoted to be a literal empty list
             * this most likely means there is an empty function call somewhere `()`
             */
            #if DEBUG_VALUE || DEBUG
            puts("ERROR TRYING TO EVALUATE A NULL");
            #endif
            return plot_runtime_error(plot_error_runtime, "trying to evaluate a null", "plot_eval_value");
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

    /* an sexpr is either a function call of a use of a form
     *
     * both forms and function calls have now been unified under the same interface
     * plot_eval_form will properly deal with either
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
 *
 * FIXME migrate to plot_eval_apply
 */
plot_value * plot_eval_form(plot_env *env, plot_value * sexpr){
    struct plot_value *val = 0;
    struct plot_value **vals;
    struct plot_value *func;
    struct plot_env *new_env;
    struct plot_value *cur, *curarg;

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
        puts("plot_eval_form: Unexpected type");
        display_error_expr(sexpr);
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
                    lcar(*vals) = plot_eval_expr(env, car(curarg));

                    /* check for error from evaluating argument */
                    if( car(*vals)->type == plot_type_error ){
                        puts("\nSaw error when processing arguments for function call:");
                        puts("  argument:");
                        display_error_expr(car(curarg));
                        puts("  function:");
                        display_error_expr(car(sexpr));
                        puts("  function call:");
                        display_error_expr(sexpr);
                        puts("");
                        return car(*vals);
                    }

                    vals = &lcdr(*vals);
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

                #if DEBUG || DEBUG_FORM
                puts("cur");
                display_error_expr(cur);
                puts("curarg");
                display_error_expr(curarg);
                #endif

                /* if curarg is not a pair then we have ran out of arguments
                 * as we are still going through our expected parameter list
                 * this means we have been ripped off
                 */
                if( curarg->type != plot_type_pair ){
                    return plot_runtime_error(plot_error_runtime, "too few arguments supplied", "plot_eval_form LAMBDA");
                }

                val = plot_eval_expr(env, car(curarg));
                curarg = cdr(curarg);
                if( ! val ){
                    return plot_runtime_error(plot_error_runtime, "evaluating argument returned NULL", "plot_eval_form LAMBDA");
                }
                if( val->type == plot_type_error ){
                    puts("plot_eval_form (lambda arg)");
                    return val;
                }
                if( ! plot_env_define(new_env, &(car(cur)->u.symbol), val) ){
                    return plot_runtime_error(plot_error_runtime, "failed to define argument", "plot_eval_form LAMBDA");
                }
                /* we are no longer holding a reference so decr */
                plot_value_decr(val);
            }

            /* check for too may args
             * curarg should have been depleted when we iterated over our expected parameters
             * if we have any left over here then throw an error
             */
            if( curarg->type != plot_type_null ){
                return plot_runtime_error(plot_error_runtime, "too many args supplied to lambda", "plot_eval_form LAMBDA");
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

/* performs a call to a function
 * can modify the environment
 * will not evaluate arguments in anyway, passes them directly to the func
 *
 * plot_eval_apply(env, func, args)
 *      func must be one of:
 *          plot_type_lambda
 *          plot_type_form
 *          plot_type_symbol - that resolves to either of the above in env
 *      args is a plot_type_pair which is the first item in a plot list of arguments
 *          arguments are NOT evaluated, so plot_eval_apply IS SAFE to call from userspace c
 *
 * both `plot_eval_form` and `plot_func_control_apply` are wrappers for this
 * safe to call from userspace c
 * plot programs instead call `plot_func_control_apply` via `(apply ...)`
 *
 * TODO FIXME
 */
struct plot_value * plot_eval_apply(struct plot_env *env, struct plot_value *func, struct plot_value *args){
    /* return value */
    struct plot_value *ret = 0;

    /*** temporaries used used in lambda evaluation ***/
    /* new environment used for lambdas */
    struct plot_env *new_env = 0;
    /* iterator used for lambda body */
    struct plot_value *bod;
    /* iterator used for lambda params */
    struct plot_value *param;
    /* iterator used for arguments */
    struct plot_value *arg;


    if( ! env || ! func || ! args ){
        return plot_runtime_error(plot_error_internal, "provided argument was null", "plot_eval_apply");
    }

    if( func->type == plot_type_symbol ){
        /* FIXME resolve symbol */
        return plot_runtime_error(plot_error_unimplemented, "provided 'func' was a sybmol, auto resolution not yet implemented", "plot_eval_apply");
    }

    switch( func->type ){
        case plot_type_lambda:
            /* create new env for our lambda */
            new_env = plot_alloc_env(func->u.lambda.env);
            if( ! new_env ){
                plot_fatal_error("call to plot_alloc_env failed");
                return 0; /* keep compiler happy */
            }

            /* binding arguments:
             * for each parameter:
             *      grab an argument and bind in the env
             * if no argument is found then error
             * if left over arguments then error
             */

            /* iterator over params and args
             * NB: first element (car) of lambda body is arg list
             */
            for( param = car(func->u.lambda.body), arg=args;
                 param->type != plot_type_null && arg->type != plot_type_null;
                 param = cdr(param), arg = cdr(arg) ){

                if( param->type != plot_type_pair ){
                    return plot_runtime_error(plot_error_internal, "iterating over params, expected pair but found non-pair and non-null param->type", "plot_eval_apply");
                }

                if( arg->type != plot_type_pair ){
                    return plot_runtime_error(plot_error_internal, "iterating over args, expected pair but found non-pair and non-null arg->type", "plot_eval_apply");
                }

                if( car(param)->type != plot_type_symbol ){
                    return plot_runtime_error(plot_error_internal, "lambda param error, expected symbol", "plot_eval_apply");
                }

                /* (define param arg)
                 * preconditions:
                 *      param must be a symbol
                 *      arg must already have been resolved to a value
                 */
                if( ! plot_env_define(new_env, &(car(param)->u.symbol), car(arg)) ){
                    return plot_runtime_error(plot_error_internal, "failed to define param", "plot_eval_apply");
                }
            }

            /* check arg to catch 'too many supplied args' */
            if( arg->type != plot_type_null ){
                return plot_runtime_error(plot_error_runtime, "too many args supplied to lambda", "plot_eval_apply");
            }

            /* check params to catch 'too few supplied args' */
            if( param->type != plot_type_null ){
                return plot_runtime_error(plot_error_runtime, "too few args supplied to lambda", "plot_eval_apply");
            }


            /* eval each part of body in new_env
             * return vaue of final expr
             *      (gc all intermediary values)
             *      check for intermediary errors
             *
             * NB: second element (cdr) of lambda body is actual body (list of exprs)
             */
            ret = 0;
            for( bod = cdr(func->u.lambda.body); bod->type != plot_type_null; bod=cdr(bod) ){
                if( ret ){
                    /* gc intermediary value (generated from last step through body */
                    plot_value_decr(ret);
                }
                ret = plot_eval_expr(new_env, car(bod));
                if( ret && ret->type == plot_type_error ){
                    puts("plot_eval_apply (lambda body)");
                    return ret;
                }
            }

            /* tidy up new_env */
            plot_env_decr(new_env);
            break;

        case plot_type_form:
            ret = func->u.form.func(env, args);
            break;

        default:
            return plot_runtime_error(plot_error_internal, "provided 'func' was not a function", "plot_eval_apply");
            break;
    }

    return ret;
}


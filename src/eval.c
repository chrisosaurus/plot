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
                puts("plot_eres_expr (resue)");
            }
            return res;
            break;
        case plot_expr_sexpr:
            #if DEBUG_EXPR || DEBUG
            puts("\tdispatching to plot_eval_sexpr");
            #endif
            res = plot_eval_sexpr(env, &(expr->u.sexpr));
            if( res && res->type == plot_type_error ){
                puts("plot_eres_expr (sexpr)");
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

/* FIXME move elsewhere */
/* return 1 if string names a form
 * oterwise returns 0
 */
static int plot_is_form(plot_sexpr * sexpr){
    plot_value *val;
    #if DEBUG_FORM || DEBUG
    puts("inside plot_is_form");
    #endif

    if( ! sexpr )
        return 0;

    if( ! sexpr->nchildren )
        return 0;

    if( sexpr->subforms[0].type != plot_expr_value )
        return 0;

    val = sexpr->subforms[0].u.value;
    switch( val->type){
        case plot_type_symbol:
            if( ! strcmp(val->u.symbol.val, "define") ){
                #if DEBUG_FORM || DEBUG
                puts("\tdefine form found");
                #endif
                return 1;
                }
            if( ! strcmp(val->u.symbol.val, "lambda") ){
                #if DEBUG_FORM || DEBUG
                puts("\tlambda form found");
                #endif
                return 1;
                }
            if( ! strcmp(val->u.symbol.val, "if") ){
                #if DEBUG_FORM || DEBUG
                puts("\tif form found");
                #endif
                return 1;
                }
            if( ! strcmp(val->u.symbol.val, "set!") ){
                #if DEBUG_FORM || DEBUG
                puts("\tset! form found");
                #endif
                return 1;
                }


            break;
        default:
            break;
    }

    #if DEBUG_FORM || DEBUG
    printf("\tno form found for '%s'\n", val->u.symbol.val);
    #endif
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

    if( plot_is_form(sexpr) ){
        res = plot_eval_form(env, sexpr);
        if( res && res->type == plot_type_error ){
            puts("plot_eval_sexpr (form)");
        }
        return res;
    }

    res = plot_eval_func_call(env, sexpr);
    if( res && res->type == plot_type_error ){
        puts("plot_eval_sexpr (func call)");
    }
    return res;
}

/* eval a form in an environment
 * can modify the environment (e.g. define)
 */
plot_value * plot_eval_form(plot_env *env, plot_sexpr * sexpr){
    plot_value *form;
    plot_value *name;
    plot_value *value;
    plot_value *tmp;
    int i;

    #if DEBUG_FORM || DEBUG
    puts("inside plot_eval_form");
    #endif

    if( ! env || ! sexpr ){
        #if DEBUG_FORM || DEBUG
        puts("\tDEFINE: bad args");
        #endif
        return 0; /* FIXME ERROR */
    }

    if( ! sexpr->nchildren ){
        #if DEBUG_FORM || DEBUG
        puts("\tDEFINE: bad nchildren");
        #endif
        return 0; /* FIXME ERROR */
    }

    if( sexpr->subforms[0].type != plot_expr_value ){
        #if DEBUG_FORM || DEBUG
        puts("\tDEFINE: bad subform[0] type");
        #endif
        return 0; /* FIXME ERROR */
    }

    tmp = plot_new_value();
    if( ! tmp ){
        #if DEBUG_FORM || DEBUG
        puts("\tplot_eval_form: failed to create new value");
        #endif
        return 0; /* FIXME error */
    }

    tmp->type = plot_type_unspecified;

    form = sexpr->subforms[0].u.value;
    switch( form->type){
        case plot_type_symbol:
            if( ! strcmp(form->u.symbol.val, "define") ){
                if( sexpr->nchildren != 3 ){
                    #if DEBUG_FORM || DEBUG
                    printf("\tDEFINE: incorrect number of children '%d'\n", sexpr->nchildren);
                    #endif
                    return 0; /* FIXME ERROR */
                }

                if( sexpr->subforms[1].type != plot_expr_value ){
                    #if DEBUG_FORM || DEBUG
                    puts("\tDEFINE: incorrect 1st arg expr type");
                    #endif
                    return 0; /* FIXME ERROR */
                }
                /* do we even know this is a value yet ? */
                name = sexpr->subforms[1].u.value;
                if( name->type != plot_type_symbol ){
                    #if DEBUG_FORM || DEBUG
                    puts("\tDEFINE: incorrect 1st arg value type");
                    #endif
                    return 0; /* FIXME ERROR */
                }

                #if DEBUG_FORM || DEBUG
                puts("\tDEFINE: getting value to store");
                #endif

                /* 2nd subform isnt known to be a value ! */
                value = plot_eval_expr(env, &(sexpr->subforms[2]));
                if( ! value ){
                    #if DEBUG_FORM || DEBUG
                    puts("\tDEFINE: failed to eval_value");
                    #endif
                    return 0; /* FIXME ERROR */
                }

                if( value->type == plot_type_error ){
                    puts("plot_eval_form (define)");
                    return value;
                }

                #if DEBUG_FORM || DEBUG
                puts("\tDEFINE: value fetched");
                #endif

                #if DEBUG_FORM || DEBUG
                puts("\tDEFINE: success!");
                printf("\tStoring value type '%d', under name '%s'\n'", value->type, name->u.symbol.val);
                #endif
                plot_env_define(env, &(name->u.symbol), value);
                /* decrement value as eval and define will both increment it and we are not keeping a reference around */
                plot_value_decr(value);
                return tmp;
            }
            if( ! strcmp(form->u.symbol.val, "lambda") ){
                if( sexpr->nchildren < 3 ){
                    #if DEBUG_FORM || DEBUG
                    printf("\tLAMBDA: incorrect number of children, need at least 3, got '%d'\n", sexpr->nchildren);
                    #endif
                    return 0; /* FIXME error */
                }

                /* check 2nd subform is an sexpr (param list) */
                if( sexpr->subforms[1].type != plot_expr_sexpr ){
                    puts("\tLAMBDA: param list expected");
                    return 0; /* FIXME error */
                }

                /* check all subforms are symbols */
                for( i=0; i< sexpr->subforms[1].u.sexpr.nchildren; ++i ){
                    if( sexpr->subforms[1].u.sexpr.subforms[i].type != plot_expr_value ){
                        puts("LAMBDA: invalid param type, expected value");
                        return 0; /* FIXME error */
                    }
                    if( sexpr->subforms[1].u.sexpr.subforms[i].u.value->type != plot_type_symbol ){
                        puts("LAMBDA: invalid param type, expected symbol");
                        return 0; /* FIXME error */
                    }
                }

                tmp->type = plot_type_lambda;
                tmp->u.lambda.env = env;
                plot_env_incr(env);
                tmp->u.lambda.body = sexpr;
                return tmp;
            }
            if( ! strcmp(form->u.symbol.val, "if") ){
                /* scheme if's can have 2 forms
                 * (if cond if-expr) ; 'guard'
                 * (if cond if-expr else-expr) ; 'branching'
                 */
                if( sexpr->nchildren != 3 && sexpr->nchildren != 4 ){
                    return 0; /* FIXME ERROR */
                }
                /* decr is handled in plot_truthy */
                value = plot_eval_expr(env, &(sexpr->subforms[1]));
                if( ! value ){
                    #if DEBUG_FORM || DEBUG
                    puts("\teval of if condition returned NULL");
                    #endif
                    return 0; /* FIXME ERROR */
                }
                if( value->type == plot_type_error ){
                    puts("plot_eval_form (if cond)");
                    return value;
                }
                if( plot_truthy(value) ){
                    plot_value_decr(value);
                    value = plot_eval_expr(env, &(sexpr->subforms[2]));
                    if( ! value ){
                        #if DEBUG_FORM || DEBUG
                        puts("\teval of if true branch returned NULL");
                        #endif
                        return 0; /* FIXME ERROR */
                    }
                    if( value->type == plot_type_error ){
                        puts("plot_eval_form (if if-expr)");
                        return value;
                    }
                } else if( sexpr->nchildren == 4){ /* (if cond if-expr else-expr) */
                    plot_value_decr(value);
                    value = plot_eval_expr(env, &(sexpr->subforms[3]));
                    if( ! value ){
                        #if DEBUG_FORM || DEBUG
                        puts("\teval of if false branch returned NULL");
                        #endif
                        return 0; /* FIXME ERROR */
                    }
                    if( value->type == plot_type_error ){
                        puts("plot_eval_form (if else-expr)");
                        return value;
                    }
                } else {
                    plot_value_decr(value);
                    /* FIXME need to define an 'undef' value
                     * (display (if #f "hello")) ;; => ??
                     * in csi this is 'unspecified'
                     * in racket (lang scheme) there is no output
                     *
                     * r5rs says: (4.1.5 page 10)
                     *  "if the <test> yields a false value and no <alternate>
                     *  is spcified, then the result of the expression is
                     *  unspecified"
                     *
                     * r6rs says: (11.4.3 page 33)
                     *  "if the <test> yields #f and no <alternate> is
                     *  specified, then the result of the expression is
                     *  unspecified"
                     */
                    return tmp; /* success */
                }

                plot_value_decr(tmp); /* don't leak */
                return value;
            }
            if( ! strcmp(form->u.symbol.val, "set!") ){
                #if DEBUG_FORM || DEBUG
                #endif
                if( sexpr->nchildren != 3 ){
                    puts("\tset! had incorrect number of arguments");
                    return 0; /* FIXME ERROR */
                }

                if( sexpr->subforms[1].type != plot_expr_value || sexpr->subforms[1].u.value->type != plot_type_symbol ){
                    puts("\tset! first argument is wrong type (either not value or not symbol)");
                    return 0; /* FIXME ERROR */
                }

                value = plot_eval_expr(env, &(sexpr->subforms[2]));
                if( ! value ){
                    #if DEBUG_FORM || DEBUG
                    puts("\teval of set value returned NULL");
                    #endif
                    return 0; /* FIXME ERROR */
                }
                if( value->type == plot_type_error ){
                    puts("plot_eval_form (set!");
                    return value;
                }

                if( ! plot_env_set(env, &(sexpr->subforms[1].u.value->u.symbol), value) ){
                    puts("\tset! call to plot_env_set failed");
                    return 0; /* FIXME ERROR */
                }

                return tmp;
            }
            break;
        default:
            break;
    }

    #if DEBUG_FORM || DEBUG
    puts("\tleaving plot_eval_form");
    #endif

    return 0; /* FIXME ERROR */
}

/* eval a function call in an environment
 */
plot_value * plot_eval_func_call(plot_env *env, plot_sexpr * sexpr){
    plot_value *val, *outerval; /* outerval is used for debugging */
    plot_value **vals;
    plot_value *func;
    plot_env *new_env;
    int i;

    #if DEBUG_FUNC || DEBUG
    puts("inside plot_eval_func_call");
    #endif

    if( ! env || ! sexpr ){
        #if DEBUG_FUNC || DEBUG
        puts("\tplot_eval_func_call: bad args");
        #endif
        return 0; /* FIXME ERROR */
    }

    if( ! sexpr->nchildren ){
        #if DEBUG_FUNC || DEBUG
        puts("\tplot_eval_func_call: no children");
        #endif
        return 0; /* FIXME ERROR */
    }

    outerval = val = plot_eval_expr(env, &(sexpr->subforms[0]));
    if( ! val ){
        return plot_runtime_error(plot_error_internal, "evaluating function returned null", "plot_eval_func_call");
    }

    switch( val->type ){
        case plot_type_error:
            puts("plot_eval_func_call (func)");
            return val;
            break;
        case plot_type_builtin:
        case plot_type_lambda:
        case plot_type_symbol:
            func = plot_eval_value(env, val);

            if( ! func ){
                if( val->type == plot_type_symbol ){
                    return plot_runtime_error(plot_error_undefined, val->u.symbol.val, "plot_eval_func_call");
                } else {
                    return plot_runtime_error(plot_error_undefined, "could not find function", "plot_eval_func_call");
                }
            }
            switch( func->type ){
                case plot_type_error:
                    puts("plot_eval_func_call (func second)");
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
                            val = plot_runtime_error(plot_error_internal, "BUILTIN call: evaluating argument returned NULL", "plot_eval_func_call");
                            puts("\touterval:");
                            plot_func_display(env, &outerval, 1);
                            puts("\tfunc:");
                            plot_func_display(env, &func, 1);
                            return val;
                        }
                        if( val->type == plot_type_error ){
                            puts("plot_eval_func_call (arg)");
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
                case plot_type_lambda:
                    #if DEBUG_FUNC || DEBUG
                    puts("\tcalling lambda");
                    #endif

                    /* create a new env with lambda->env as parent */
                    new_env = plot_new_env(func->u.lambda.env);
                    if( ! new_env ){
                        puts("\tLAMBDA: call to plot_env_init failed");
                        return 0; /* FIXME error */
                    }

                    if( func->u.lambda.body->subforms[1].type != plot_expr_sexpr ){
                        puts("\tLAMBDA: plot_eval_func_call parameter list is not an sexpr");
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
                            puts("plot_eval_func_call (lambda arg)");
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
                            puts("plot_eval_func_call (lambda body)");
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
                    puts("\tERROR: unknown syntax");
                    return 0; /* FIXME ERROR */
                    break;
            }
            break;
        default:
            #if DEBUG_FUNC || DEBUG
            puts("\tplot_eval_func_call: unknown syntax");
            #endif
            puts("\tERROR: unknown syntax");
            return 0; /* FIXME ERROR */
            break;
    }
    puts("ended");

    return 0;
}


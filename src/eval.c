#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* strcmp */

#include "value.h"
#include "hash.h"
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
            printf("symbol found '%s', resolving: dispatching to plot_env_get\n", val->u.symbol.val);
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
            if( ! strcmp(val->u.symbol.val, "if") ){
                #if DEBUG_FORM || DEBUG
                puts("if form found");
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

/* returns 1 if value is considered truthy
 * returns 0 if falsy
 */
static int plot_eval_truthy(const plot_value *val){
    if( ! val )
        return 0;

    if( val->type == plot_type_boolean && val->u.boolean.val == false )
        return 0;

    return 1;
}

/* eval a form in an environment
 * can modify the environment (e.g. define)
 */
const plot_value * plot_eval_form(plot_env *env, const plot_sexpr * sexpr){
    const plot_value *form;
    const plot_value *name;
    const plot_value *value;
    plot_value *tmp;
    int i;

    #if DEBUG_FORM || DEBUG
    puts("inside plot_eval_form");
    #endif

    if( ! env || ! sexpr ){
        #if DEBUG_FORM || DEBUG
        puts("DEFINE: bad args");
        #endif
        return 0; /* FIXME ERROR */
    }

    if( ! sexpr->nchildren ){
        #if DEBUG_FORM || DEBUG
        puts("DEFINE: bad nchildren");
        #endif
        return 0; /* FIXME ERROR */
    }

    if( sexpr->subforms[0].type != plot_expr_value ){
        #if DEBUG_FORM || DEBUG
        puts("DEFINE: bad subform[0] type");
        #endif
        return 0; /* FIXME ERROR */
    }

    form = &(sexpr->subforms[0].u.value);
    switch( form->type){
        case plot_type_symbol:
            if( ! strcmp(form->u.symbol.val, "define") ){
                if( sexpr->nchildren != 3 ){
                    #if DEBUG_FORM || DEBUG
                    printf("DEFINE: incorrect number of children '%d'\n", sexpr->nchildren);
                    #endif
                    return 0; /* FIXME ERROR */
                }

                if( sexpr->subforms[1].type != plot_expr_value ){
                    #if DEBUG_FORM || DEBUG
                    puts("DEFINE: incorrect 1st arg expr type");
                    #endif
                    return 0; /* FIXME ERROR */
                }
                /* do we even know this is a value yet ? */
                name = &(sexpr->subforms[1].u.value);
                if( name->type != plot_type_symbol ){
                    #if DEBUG_FORM || DEBUG
                    puts("DEFINE: incorrect 1st arg value type");
                    #endif
                    return 0; /* FIXME ERROR */
                }

                /* 2nd subform isnt known to be a value ! */
                value = plot_eval_expr(env, &(sexpr->subforms[2]));
                if( ! value ){
                    #if DEBUG_FORM || DEBUG
                    puts("DEFINE: failed to eval_value");
                    #endif
                    return 0; /* FIXME ERROR */
                }

                #if DEBUG_FORM || DEBUG
                puts("DEFINE: success!");
                printf("Storing value type '%d', under name '%s'\n'", value->type, name->u.symbol.val);
                #endif
                plot_env_define(env, &(name->u.symbol), value);
                return 0; /* FIXME success */
            }
            if( ! strcmp(form->u.symbol.val, "lambda") ){
                if( sexpr->nchildren < 3 ){
                    #if DEBUG_FORM || DEBUG
                    printf("LAMBDA: incorrect number of children, need at least 3, got '%d'\n", sexpr->nchildren);
                    #endif
                    return 0; /* FIXME error */
                }

                /* check 2nd subform is an sexpr (param list) */
                if( sexpr->subforms[1].type != plot_expr_sexpr ){
                    puts("LAMBDA: param list expected");
                    return 0; /* FIXME error */
                }

                /* check all subforms are symbols */
                for( i=0; i< sexpr->subforms[1].u.sexpr.nchildren; ++i ){
                    if( sexpr->subforms[1].u.sexpr.subforms[i].type != plot_expr_value ){
                        puts("LAMBDA: invalid param type, expected value");
                        return 0; /* FIXME error */
                    }
                    if( sexpr->subforms[1].u.sexpr.subforms[i].u.value.type != plot_type_symbol ){
                        puts("LAMBDA: invalid param type, expected symbol");
                        return 0; /* FIXME error */
                    }
                }

                tmp = calloc(1, sizeof *value);
                if( ! tmp ){
                    #if DEBUG_FORM || DEBUG
                    puts("LAMBDA: failed to calloc");
                    #endif
                    return 0; /* FIXME error */
                }

                tmp->type = plot_type_lambda;
                tmp->u.lambda.env = env;
                tmp->u.lambda.body = sexpr;
                return tmp;
            }
            if( ! strcmp(form->u.symbol.val, "if") ){
                if( sexpr->nchildren != 4 ){
                    return 0; /* FIXME ERROR */
                }
                value = plot_eval_expr(env, &(sexpr->subforms[1]));
                if( ! value ){
                    #if DEBUG_FORM || DEBUG
                    puts("eval of if condition returned NULL");
                    #endif
                    return 0; /* FIXME ERROR */
                }
                if( plot_eval_truthy(value) ){
                    value = plot_eval_expr(env, &(sexpr->subforms[2]));
                    if( ! value ){
                        #if DEBUG_FORM || DEBUG
                        puts("eval of if true branch returned NULL");
                        #endif
                        return 0; /* FIXME ERROR */
                    }
                } else {
                    value = plot_eval_expr(env, &(sexpr->subforms[3]));
                    if( ! value ){
                        #if DEBUG_FORM || DEBUG
                        puts("eval of if false branch returned NULL");
                        #endif
                        return 0; /* FIXME ERROR */
                    }
                }

                return value;
            }
            break;
        default:
            break;
    }

    #if DEBUG_FORM || DEBUG
    puts("leaving plot_eval_form");
    #endif

    return 0; /* FIXME ERROR */
}

/* eval a function call in an environment
 */
const plot_value * plot_eval_func_call(plot_env *env, const plot_sexpr * sexpr){
    const plot_value *val;
    const plot_value *func;
    plot_env *new_env;
    int i;

    #if DEBUG_FUNC || DEBUG
    puts("inside plot_eval_func_call");
    #endif

    if( ! env || ! sexpr ){
        #if DEBUG_FUNC || DEBUG
        puts("plot_eval_func_call: bad args");
        #endif
        return 0; /* FIXME ERROR */
    }

    if( ! sexpr->nchildren ){
        #if DEBUG_FUNC || DEBUG
        puts("plot_eval_func_call: no children");
        #endif
        return 0; /* FIXME ERROR */
    }

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
            switch( func->type ){
                case plot_type_builtin:
                    #if DEBUG_FUNC || DEBUG
                    puts("calling a builtin function...");
                    #endif
                    return func->u.builtin.func( env, sexpr->subforms + 1, sexpr->nchildren - 1);
                case plot_type_lambda:
                    #if DEBUG_FUNC || DEBUG
                    puts("calling lambda");
                    #endif

                    /* create a new env with lambda->env as parent */
                    new_env = plot_env_init(func->u.lambda.env);
                    if( ! new_env ){
                        puts("LAMBDA: call to plot_env_init failed");
                        return 0; /* FIXME error */
                    }

                    if( func->u.lambda.body->subforms[1].type != plot_expr_sexpr ){
                        puts("LAMBDA: plot_eval_func_call parameter list is not an sexpr");
                        return 0; /* FIXME error */
                    }

                    /* check number of arguments match number of parameters */
                    if( func->u.lambda.body->subforms[1].u.sexpr.nchildren != sexpr->nchildren - 1 ){
                        printf("LAMBDA: incorrect number of arguments, expected '%d' and got '%d'\n", func->u.lambda.body->subforms[1].u.sexpr.nchildren, sexpr->nchildren - 1);
                    }
                    /* for each parameter grab an argument
                     *  if no argument then error
                     *  define(new-env, parameter, argument)
                     */
                    for( i=0; i< func->u.lambda.body->subforms[1].u.sexpr.nchildren ; ++i ){
                        if( func->u.lambda.body->subforms[1].u.sexpr.subforms[i].type != plot_expr_value ){
                            puts("LAMBDA: expected value");
                            return 0; /* FIXME error */
                        }
                        if( func->u.lambda.body->subforms[1].u.sexpr.subforms[i].u.value.type != plot_type_symbol ){
                            puts("LAMBDA: expected symbol");
                            return 0; /* FIXME error */
                        }
                        val = plot_eval_expr(env, &(sexpr->subforms[1+i]));
                        if( ! val ){
                            puts("LAMBDA: evaluating argument returned NULL");
                            return 0; /* FIXME error */
                        }
                        if( ! plot_env_define(new_env, &(func->u.lambda.body->subforms[1].u.sexpr.subforms[i].u.value.u.symbol), val) ){
                            puts("LAMBDA: failed to define argument");
                            return 0; /* FIXME error */
                        }
                    }

                    /* eval each part of the body in new_env
                     * return value of final expr
                     */
                    for( i=2; i < func->u.lambda.body->nchildren; ++i ){
                        val = plot_eval_expr(new_env, &(func->u.lambda.body->subforms[i]) );
                    }
                    return val;

                    break;
                default:
                    puts("ERROR: unknown syntax");
                    return 0; /* FIXME ERROR */
                    break;
            }
            break;
        case plot_type_builtin:
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
    puts("ended");

    return 0;
}


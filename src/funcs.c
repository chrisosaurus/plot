#include <stdlib.h>
#include <stdio.h>

#include "value.h"
#include "types.h"
#include "hash.h"
#include "env.h"
#include "funcs.h"
#include "eval.h"

#define DEBUG 0

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

/* print error information and then exit
 */
void plot_handle_error(const plot_value *error){
    const char *type = "unknown";
    if( ! error->type == plot_type_error ){
        printf("Error encountered in 'plot_handle_error', invalid error value supplied\n");
        exit(1);
    }

    switch(error->u.error.type){
        case plot_error_alloc_failed:
            type = "alloc failed";
            break;
        case plot_error_bad_args:
            type = "bad args";
            break;
        case plot_error_internal:
            type = "internal error";
            break;
        case plot_error_unbound_symbol:
            type = "unbound symbol";
            break;
        default:
            type = "IMPOSSIBLE ERROR";
            break;
    }

    printf("Error encountered in '%s', error message: '%s', error type: '%s'\n", error->u.error.location, error->u.error.msg, type);
    exit(1);
}

/* internal routine for displaying a value
 */
static void plot_func_display_value(plot_env *env, const plot_value *val){
    plot_value err;

    if( ! val )
        return;

    switch(val->type){
        case plot_type_boolean:
            printf("#%c", val->u.boolean.val ? 't' : 'f' );
            break;
        case plot_type_number:
            printf("%d", val->u.number.val);
            break;
        case plot_type_string:
            printf("%s", val->u.string.val);
            break;
        case plot_type_symbol:
            puts("Unable to print a symbol at this point in time");
            break;
        case plot_type_builtin:
            puts("Unable to print a builtin function at this point in time");
            break;
        case plot_type_error:
            plot_handle_error(val);
            break;
        default:
            err.type = plot_type_error;
            err.u.error.type = plot_error_internal;
            err.u.error.msg = "impossible type for value";
            err.u.error.location = "plot_func_display_value";
            plot_handle_error(&err);
            break;
    }
}

/* print value to stdout
 */
const plot_value * plot_func_display(plot_env *env, const plot_expr *args, int argc){
    const plot_expr *arg;
    const plot_value *val;
    int i;

    /* FIXME TODO should only care about first arg */
    for(arg=args, i=0; i<argc; ++i, arg=(args+1)){
        val = plot_eval_expr(env, arg);
        if( ! val ){
            /* TODO FIXME handle error cases
             */
            puts( "OOPS" );
        }

        plot_func_display_value(env, val);
    }

    return 0;
}

/* print a newline to stdout
 */
const plot_value * plot_func_newline(plot_env *env, const plot_expr *args, int argc){
    /* FIXME currently ignores arguments, only there to match plot_func interface
     */
    puts("");

    return 0;
}

/* takes a list of expressions
 * evals each and then adds the expression's value if it is a number
 * if any of the expressions evaluate to something other than an error
 * throw plot_error_bad_args
 */
const plot_value * plot_func_add(plot_env *env, const plot_expr *args, int argc){
    plot_value *res;
    const plot_value *tmp;
    const plot_expr *arg;
    int sum=0, i;

    #if DEBUG
    puts("inside plot_func_add");
    #endif

    if( !env ){
        #if DEBUG
        puts("env is null");
        #endif
        return 0; /* ERROR */
    }

    for( arg=args, i=0; i<argc; ++i, arg+=1 ){
        tmp = plot_eval_expr(env, arg);
        if( ! tmp ){
            #if DEBUG
            puts("value returned by plot_eval is null");
            #endif
            return 0; /* ERROR */
        }

        if( ! (tmp->type == plot_type_number) ){
            #if DEBUG
            puts("value returned by plot_eval is not a number");
            #endif
            return 0; /* ERROR */
        }

        sum += tmp->u.number.val;
    }

    res = plot_value_new();
    if( !res ){
        #if DEBUG
        puts("res failed call to plot_value_new");
        #endif
        return 0; /* ERROR */
    }

    res->type = plot_type_number;
    res->u.number.val = sum;

    #if DEBUG
    puts("returning sum of 2 numbers");
    #endif
    return res;
}

/* takes a list of expressions
 * evals each and then subtracts the expression's value if it is a number
 * if any of the expressions evaluate to something other than an error
 * throw plot_error_bad_args
 */
const plot_value * plot_func_subtract(plot_env *env, const plot_expr *args, int argc){
    plot_value *res;
    const plot_value *tmp;
    const plot_expr *arg;
    int difference=0, i;

    #if DEBUG
    puts("inside plot_func_subtract");
    #endif

    if( !env ){
        #if DEBUG
        puts("env is null");
        #endif
        return 0; /* ERROR */
    }

    for( arg=args, i=0; i<argc; ++i, arg+=1 ){
        tmp = plot_eval_expr(env, arg);
        if( ! tmp ){
            #if DEBUG
            puts("value returned by plot_eval is null");
            #endif
            return 0; /* ERROR */
        }

        if( ! (tmp->type == plot_type_number) ){
            #if DEBUG
            puts("value returned by plot_eval is not a number");
            #endif
            return 0; /* ERROR */
        }

        if( i == 0 ){
            difference = tmp->u.number.val;
        } else {
            difference -= tmp->u.number.val;
        }
    }

    res = plot_value_new();
    if( !res ){
        #if DEBUG
        puts("res failed call to plot_value_new");
        #endif
        return 0; /* ERROR */
    }

    res->type = plot_type_number;
    res->u.number.val = difference;

    #if DEBUG
    puts("returning difference");
    #endif
    return res;

    return 0;
}

/* takes a list of expressions
 * evals each and then multiplies the expression's value if it is a number
 * if any of the expressions evaluate to something other than an error
 * throw plot_error_bad_args
 */
const plot_value * plot_func_multiply(plot_env *env, const plot_expr *args, int argc){
    plot_value *res;
    const plot_value *tmp;
    const plot_expr *arg;
    int product=1, i;

    #if DEBUG
    puts("inside plot_func_multiply");
    #endif

    if( !env ){
        #if DEBUG
        puts("env is null");
        #endif
        return 0; /* ERROR */
    }

    for( arg=args, i=0; i<argc; ++i, arg+=1 ){
        tmp = plot_eval_expr(env, arg);
        if( ! tmp ){
            #if DEBUG
            puts("value returned by plot_eval is null");
            #endif
            return 0; /* ERROR */
        }

        if( ! (tmp->type == plot_type_number) ){
            #if DEBUG
            puts("value returned by plot_eval is not a number");
            #endif
            return 0; /* ERROR */
        }

        product *= tmp->u.number.val;
    }

    res = plot_value_new();
    if( !res ){
        #if DEBUG
        puts("res failed call to plot_value_new");
        #endif
        return 0; /* ERROR */
    }

    res->type = plot_type_number;
    res->u.number.val = product;

    #if DEBUG
    puts("returning product");
    #endif
    return res;

}
/* integer division
 * exact only
 */
const struct plot_value * plot_func_divide(struct plot_env *env, const struct plot_expr *args, int argc){
    plot_value *res;
    const plot_value *tmp;
    const plot_expr *arg;
    int quotient=0, i;

    #if DEBUG
    puts("inside plot_func_divide");
    #endif

    if( !env ){
        #if DEBUG
        puts("env is null");
        #endif
        return 0; /* ERROR */
    }

    for( arg=args, i=0; i<argc; ++i, arg+=1 ){
        tmp = plot_eval_expr(env, arg);
        if( ! tmp ){
            #if DEBUG
            puts("value returned by plot_eval is null");
            #endif
            return 0; /* ERROR */
        }

        if( ! (tmp->type == plot_type_number) ){
            #if DEBUG
            puts("value returned by plot_eval is not a number");
            #endif
            return 0; /* ERROR */
        }

        if( i == 0 )
            quotient = tmp->u.number.val;
        else
            quotient /= tmp->u.number.val;
    }

    res = plot_value_new();
    if( !res ){
        #if DEBUG
        puts("res failed call to plot_value_new");
        #endif
        return 0; /* ERROR */
    }

    res->type = plot_type_number;
    res->u.number.val = quotient;

    #if DEBUG
    puts("returning quotient");
    #endif
    return res;

}

/* remainder
 */
const struct plot_value * plot_func_remainder(struct plot_env *env, const struct plot_expr *args, int argc){
    plot_value *res;
    const plot_value *tmp;
    const plot_expr *arg;
    int remainder=0, i;

    #if DEBUG
    puts("inside plot_func_remainder");
    #endif

    if( !env ){
        #if DEBUG
        puts("env is null");
        #endif
        return 0; /* ERROR */
    }

    if( argc != 2 ){
        #if DEBUG
        puts("exected exactly 2 arguments");
        #endif
        return 0; /* FIXME ERROR */
    }

    for( arg=args, i=0; i<argc; ++i, arg+=1 ){
        tmp = plot_eval_expr(env, arg);
        if( ! tmp ){
            #if DEBUG
            puts("value returned by plot_eval is null");
            #endif
            return 0; /* ERROR */
        }

        if( ! (tmp->type == plot_type_number) ){
            #if DEBUG
            puts("value returned by plot_eval is not a number");
            #endif
            return 0; /* ERROR */
        }

        if( i == 0 )
            remainder = tmp->u.number.val;
        else
            remainder %= tmp->u.number.val;
    }

    res = plot_value_new();
    if( !res ){
        #if DEBUG
        puts("res failed call to plot_value_new");
        #endif
        return 0; /* ERROR */
    }

    res->type = plot_type_number;
    res->u.number.val = remainder;

    #if DEBUG
    puts("returning remainder");
    #endif
    return res;
}

/******** comparison functions *******/

/* mathmatical =
 */
const struct plot_value * plot_func_equal(struct plot_env *env, const struct plot_expr *args, int argc){
    plot_value tmp;
    plot_value *res;
    const plot_value *val;
    int i;
    const plot_expr *arg;

    tmp.type = plot_type_number;

    #if DEBUG
    puts("inside plot_func_equal");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    res = plot_value_new();
    if( ! res ){
        #if DEBUG
        puts("called to plot_value_new failed");
        #endif
        return 0; /* FIXME error */
    }

    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    for( arg=args, i=0; i<argc; ++i, arg+=1 ){
        val = plot_eval_expr(env, arg);
        if( ! val ){
            #if DEBUG
            puts("return val was NULL");
            #endif
            return 0; /* FIXME error */
        }

        if( val->type != plot_type_number ){
            #if DEBUG
            puts("evaled expr did not yield number");
            #endif
            return 0; /* FIXME error */
        }

        if( i == 0 ){
            tmp.u.number.val = val->u.number.val;
        } else {
            if( tmp.u.number.val != val->u.number.val ){
                #if DEBUG
                printf("'not equal' for i '%d'\n", i);
                #endif
                return res;
            }
        }
    }

    res->u.boolean.val = true;
    return res;
}

/* <
 */
const struct plot_value * plot_func_less(struct plot_env *env, const struct plot_expr *args, int argc){
    plot_value tmp;
    plot_value *res;
    const plot_value *val;
    int i;
    const plot_expr *arg;

    tmp.type = plot_type_number;

    #if DEBUG
    puts("inside plot_func_less");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    res = plot_value_new();
    if( ! res ){
        #if DEBUG
        puts("called to plot_value_new failed");
        #endif
        return 0; /* FIXME error */
    }

    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    for( arg=args, i=0; i<argc; ++i, arg+=1 ){
        val = plot_eval_expr(env, arg);
        if( ! val ){
            #if DEBUG
            puts("return val was NULL");
            #endif
            return 0; /* FIXME error */
        }

        if( val->type != plot_type_number ){
            #if DEBUG
            puts("evaled expr did not yield number");
            #endif
            return 0; /* FIXME error */
        }

        if( i == 0 ){
            tmp.u.number.val = val->u.number.val;
        } else {
            if( ! (tmp.u.number.val < val->u.number.val) ){
                #if DEBUG
                printf("not 'less than' for i '%d'\n", i);
                #endif
                return res;
            }
            tmp.u.number.val = val->u.number.val;
        }
    }

    res->u.boolean.val = true;
    return res;

    return 0;
}

/* >
 */
const struct plot_value * plot_func_greater(struct plot_env *env, const struct plot_expr *args, int argc){
    plot_value tmp;
    plot_value *res;
    const plot_value *val;
    int i;
    const plot_expr *arg;

    tmp.type = plot_type_number;

    #if DEBUG
    puts("inside plot_func_greater");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    res = plot_value_new();
    if( ! res ){
        #if DEBUG
        puts("called to plot_value_new failed");
        #endif
        return 0; /* FIXME error */
    }

    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    for( arg=args, i=0; i<argc; ++i, arg+=1 ){
        val = plot_eval_expr(env, arg);
        if( ! val ){
            #if DEBUG
            puts("return val was NULL");
            #endif
            return 0; /* FIXME error */
        }

        if( val->type != plot_type_number ){
            #if DEBUG
            puts("evaled expr did not yield number");
            #endif
            return 0; /* FIXME error */
        }

        if( i == 0 ){
            tmp.u.number.val = val->u.number.val;
        } else {
            if( ! (tmp.u.number.val > val->u.number.val) ){
                #if DEBUG
                printf("not 'greater than' for i '%d'\n", i);
                #endif
                return res;
            }
            tmp.u.number.val = val->u.number.val;
        }
    }

    res->u.boolean.val = true;
    return res;
    return 0;
}

/* <=
 */
const struct plot_value * plot_func_less_equal(struct plot_env *env, const struct plot_expr *args, int argc){
    plot_value tmp;
    plot_value *res;
    const plot_value *val;
    int i;
    const plot_expr *arg;

    tmp.type = plot_type_number;

    #if DEBUG
    puts("inside plot_func_less_equal");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    res = plot_value_new();
    if( ! res ){
        #if DEBUG
        puts("called to plot_value_new failed");
        #endif
        return 0; /* FIXME error */
    }

    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    for( arg=args, i=0; i<argc; ++i, arg+=1 ){
        val = plot_eval_expr(env, arg);
        if( ! val ){
            #if DEBUG
            puts("return val was NULL");
            #endif
            return 0; /* FIXME error */
        }

        if( val->type != plot_type_number ){
            #if DEBUG
            puts("evaled expr did not yield number");
            #endif
            return 0; /* FIXME error */
        }

        if( i == 0 ){
            tmp.u.number.val = val->u.number.val;
        } else {
            if( ! (tmp.u.number.val <= val->u.number.val) ){
                #if DEBUG
                printf("not 'less than or equal' for i '%d'\n", i);
                #endif
                return res;
            }
            tmp.u.number.val = val->u.number.val;
        }
    }

    res->u.boolean.val = true;
    return res;
    return 0;
}

/* >=
 */
const struct plot_value * plot_func_greater_equal(struct plot_env *env, const struct plot_expr *args, int argc){
    plot_value tmp;
    plot_value *res;
    const plot_value *val;
    int i;
    const plot_expr *arg;

    tmp.type = plot_type_number;

    #if DEBUG
    puts("inside plot_func_greater_equal");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    res = plot_value_new();
    if( ! res ){
        #if DEBUG
        puts("called to plot_value_new failed");
        #endif
        return 0; /* FIXME error */
    }

    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    for( arg=args, i=0; i<argc; ++i, arg+=1 ){
        val = plot_eval_expr(env, arg);
        if( ! val ){
            #if DEBUG
            puts("return val was NULL");
            #endif
            return 0; /* FIXME error */
        }

        if( val->type != plot_type_number ){
            #if DEBUG
            puts("evaled expr did not yield number");
            #endif
            return 0; /* FIXME error */
        }

        if( i == 0 ){
            tmp.u.number.val = val->u.number.val;
        } else {
            if( ! (tmp.u.number.val >= val->u.number.val) ){
                #if DEBUG
                printf("not 'less than or equal' for i '%d'\n", i);
                #endif
                return res;
            }
            tmp.u.number.val = val->u.number.val;
        }
    }

    res->u.boolean.val = true;
    return res;
    return 0;
}

/********* value testing functions ********/

/* boolean?
 */
const struct plot_value * plot_func_boolean_test(struct plot_env *env, const struct plot_expr *args, int argc){
    plot_value *res;
    const plot_value *val;

    #if DEBUG
    puts("inside plot_func_boolean_test");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    res = plot_value_new();
    if( ! res ){
        #if DEBUG
        puts("called to plot_value_new failed");
        #endif
        return 0; /* FIXME error */
    }

    if( argc != 1 ){
        #if DEBUG
        puts("incorrect number of args to plot_func_boolean_test");
        #endif
        return 0; /* FIXME error */
    }

    val = plot_eval_expr(env, args);

    if( ! val ){
        #if DEBUG
        puts("call to plot_eval_expr returned NULL");
        #endif
        return 0; /* FIXME error */
    }

    res->type = plot_type_boolean;
    if( val->type == plot_type_boolean ){
        res->u.boolean.val = true;
        return res;
    } else {
        res->u.boolean.val = false;
        return res;
    }
}

/* symbol?
 */
const struct plot_value * plot_func_symbol_test(struct plot_env *env, const struct plot_expr *args, int argc){
    plot_value *res;
    const plot_value *val;

    #if DEBUG
    puts("inside plot_func_symbol_test");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    res = plot_value_new();
    if( ! res ){
        #if DEBUG
        puts("called to plot_value_new failed");
        #endif
        return 0; /* FIXME error */
    }

    if( argc != 1 ){
        #if DEBUG
        puts("incorrect number of args to plot_func_symbol_test");
        #endif
        return 0; /* FIXME error */
    }

    val = plot_eval_expr(env, args);

    if( ! val ){
        #if DEBUG
        puts("call to plot_eval_expr returned NULL");
        #endif
        return 0; /* FIXME error */
    }

    res->type = plot_type_boolean;
    if( val->type == plot_type_symbol ){
        res->u.boolean.val = true;
        return res;
    } else {
        res->u.boolean.val = false;
        return res;
    }
}

/* string?
 */
const struct plot_value * plot_func_string_test(struct plot_env *env, const struct plot_expr *args, int argc){
    plot_value *res;
    const plot_value *val;

    #if DEBUG
    puts("inside plot_func_boolean_test");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    res = plot_value_new();
    if( ! res ){
        #if DEBUG
        puts("called to plot_value_new failed");
        #endif
        return 0; /* FIXME error */
    }

    if( argc != 1 ){
        #if DEBUG
        puts("incorrect number of args to plot_func_string_test");
        #endif
        return 0; /* FIXME error */
    }

    val = plot_eval_expr(env, args);

    if( ! val ){
        #if DEBUG
        puts("call to plot_eval_expr returned NULL");
        #endif
        return 0; /* FIXME error */
    }

    res->type = plot_type_boolean;
    if( val->type == plot_type_string ){
        res->u.boolean.val = true;
        return res;
    } else {
        res->u.boolean.val = false;
        return res;
    }
}

/* number?
 */
const struct plot_value * plot_func_number_test(struct plot_env *env, const struct plot_expr *args, int argc){
    plot_value *res;
    const plot_value *val;

    #if DEBUG
    puts("inside plot_func_number_test");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    res = plot_value_new();
    if( ! res ){
        #if DEBUG
        puts("called to plot_value_new failed");
        #endif
        return 0; /* FIXME error */
    }

    if( argc != 1 ){
        #if DEBUG
        puts("incorrect number of args to plot_func_number_test");
        #endif
        return 0; /* FIXME error */
    }

    val = plot_eval_expr(env, args);

    if( ! val ){
        #if DEBUG
        puts("call to plot_eval_expr returned NULL");
        #endif
        return 0; /* FIXME error */
    }

    res->type = plot_type_boolean;
    if( val->type == plot_type_number ){
        res->u.boolean.val = true;
        return res;
    } else {
        res->u.boolean.val = false;
        return res;
    }
}

/* function?
 */
const struct plot_value * plot_func_procedure_test(struct plot_env *env, const struct plot_expr *args, int argc){
    plot_value *res;
    const plot_value *val;

    #if DEBUG
    puts("inside plot_func_procedure_test");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    res = plot_value_new();
    if( ! res ){
        #if DEBUG
        puts("called to plot_value_new failed");
        #endif
        return 0; /* FIXME error */
    }

    if( argc != 1 ){
        #if DEBUG
        puts("incorrect number of args to plot_func_procedure_test");
        #endif
        return 0; /* FIXME error */
    }

    val = plot_eval_expr(env, args);

    if( ! val ){
        #if DEBUG
        puts("call to plot_eval_expr returned NULL");
        #endif
        return 0; /* FIXME error */
    }

    res->type = plot_type_boolean;
    if( val->type == plot_type_builtin || val->type == plot_type_lambda ){
        res->u.boolean.val = true;
        return res;
    } else {
        res->u.boolean.val = false;
        return res;
    }
}


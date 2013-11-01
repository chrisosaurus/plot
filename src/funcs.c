#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h> /* tolower */

#include "value.h"
#include "types.h"
#include "hash.h"
#include "env.h"
#include "funcs.h"
#include "eval.h"
#include "plot.h"

#define DEBUG 0

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

/* internal routine for displaying a value
 * returns 0 on failure, 1 on success
 */
static plot_value * plot_func_display_value(plot_env *env, plot_value *val){
    plot_value *ret;

    if( ! val )
        return 0;

    ret = plot_new_value();
    if( ! ret ){
        plot_fatal_error("plot_func_display_value: failed call to plot_new_value()");
    }

    ret->type = plot_type_unspecified;

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
            return plot_runtime_error(plot_error_internal, "trying to print an error value", "plot_func_display_value");
            break;
        case plot_type_lambda:
            puts("Unable to print a lambda value at this point in time");
            break;
        case plot_type_unspecified:
            puts("<unspecified>");
            break;
        case plot_type_reclaimed:
            puts("ERROR: you are trying to display a garbage collected value, most likely an error in the GC");
            exit(1);
            break;
        default:
            return plot_runtime_error(plot_error_internal, "impossible type for val->type", "plot_func_display_value");
            break;
    }

    return ret;
}

/* print value to stdout
 */
plot_value * plot_func_display(plot_env *env, plot_value **args, int argc){
    plot_value *arg;

    if( argc != 1 ){
        #if DEBUG
        puts("incorrect number of args to plot_func_display");
        #endif
        return 0; /* FIXME error */
    }

    arg = args[0];

    if( ! arg ){
        puts("argument was NULL");
        return 0;
    }

    return plot_func_display_value(env, arg);
}

/* print a newline to stdout
 */
plot_value * plot_func_newline(plot_env *env, plot_value **args, int argc){
    plot_value *ret = plot_new_value();
    if( ret )
        ret->type = plot_type_unspecified;

    /* FIXME currently ignores arguments, only there to match plot_func interface
     */
    puts("");

    return ret;
}

/* takes a list of expressions
 * evals each and then adds the expression's value if it is a number
 * if any of the expressions evaluate to something other than an error
 * throw plot_error_bad_args
 */
plot_value * plot_func_add(plot_env *env, plot_value **args, int argc){
    plot_value *res;
    plot_value *arg;
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

    for( i=0; i<argc; ++i ){
        arg = args[i];
        if( ! arg ){
            #if DEBUG
            puts("argument to plot_func_add was null");
            #endif
            return 0; /* ERROR */
        }

        if( ! (arg->type == plot_type_number) ){
            #if DEBUG
            puts("arg is not a number");
            #endif
            return 0; /* ERROR */
        }

        sum += arg->u.number.val;
    }

    res = plot_new_value();
    if( !res ){
        #if DEBUG
        puts("res failed call to plot_new_value");
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
plot_value * plot_func_subtract(plot_env *env, plot_value **args, int argc){
    plot_value *res;
    plot_value *arg;
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

    for( i=0; i<argc; ++i ){
        arg = args[i];
        if( ! arg ){
            #if DEBUG
            puts("arg was NULL");
            #endif
            return 0; /* ERROR */
        }

        if( ! (arg->type == plot_type_number) ){
            #if DEBUG
            puts("value returned by plot_eval is not a number");
            #endif
            return 0; /* ERROR */
        }

        if( i == 0 ){
            difference = arg->u.number.val;
        } else {
            difference -= arg->u.number.val;
        }
    }

    res = plot_new_value();
    if( !res ){
        #if DEBUG
        puts("res failed call to plot_new_value");
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
plot_value * plot_func_multiply(plot_env *env, plot_value **args, int argc){
    plot_value *res;
    plot_value *arg;
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

    for( i=0; i<argc; ++i ){
        arg = args[i];
        if( ! arg ){
            #if DEBUG
            puts("arg was NULL");
            #endif
            return 0; /* ERROR */
        }

        if( ! (arg->type == plot_type_number) ){
            #if DEBUG
            puts("value returned by plot_eval is not a number");
            #endif
            return 0; /* ERROR */
        }

        product *= arg->u.number.val;
    }

    res = plot_new_value();
    if( !res ){
        #if DEBUG
        puts("res failed call to plot_new_value");
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
struct plot_value * plot_func_divide(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;
    plot_value *arg;
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

    for( i=0; i<argc; ++i ){
        arg = args[i];
        if( ! arg ){
            #if DEBUG
            puts("arg was NULL");
            #endif
            return 0; /* ERROR */
        }

        if( ! (arg->type == plot_type_number) ){
            #if DEBUG
            puts("value returned by plot_eval is not a number");
            #endif
            return 0; /* ERROR */
        }

        if( i == 0 )
            quotient = arg->u.number.val;
        else
            quotient /= arg->u.number.val;
    }

    res = plot_new_value();
    if( !res ){
        #if DEBUG
        puts("res failed call to plot_new_value");
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
struct plot_value * plot_func_remainder(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;
    plot_value *arg;
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

    for( i=0; i<argc; ++i ){
        arg = args[i];
        if( ! arg ){
            #if DEBUG
            puts("arg was NULL");
            #endif
            return 0; /* ERROR */
        }

        if( ! (arg->type == plot_type_number) ){
            #if DEBUG
            puts("value returned by plot_eval is not a number");
            #endif
            return 0; /* ERROR */
        }

        if( i == 0 )
            remainder = arg->u.number.val;
        else
            remainder %= arg->u.number.val;
    }

    res = plot_new_value();
    if( !res ){
        #if DEBUG
        puts("res failed call to plot_new_value");
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
struct plot_value * plot_func_math_equal(struct plot_env *env, struct plot_value **args, int argc){
    plot_value tmp;
    plot_value *res;
    plot_value *arg;
    int i;

    tmp.type = plot_type_number;

    #if DEBUG
    puts("inside plot_func_math_equal");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    res = plot_new_value();
    if( ! res ){
        #if DEBUG
        puts("called to plot_new_value failed");
        #endif
        return 0; /* FIXME error */
    }

    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    for( i=0; i<argc; ++i ){
        arg = args[i];
        if( ! arg ){
            #if DEBUG
            puts("arg was NULL");
            #endif
            return 0; /* FIXME error */
        }

        if( arg->type != plot_type_number ){
            #if DEBUG
            puts("evaled expr did not yield number");
            #endif
            return 0; /* FIXME error */
        }

        if( i == 0 ){
            tmp.u.number.val = arg->u.number.val;
        } else {
            if( tmp.u.number.val != arg->u.number.val ){
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
struct plot_value * plot_func_less(struct plot_env *env, struct plot_value **args, int argc){
    plot_value tmp;
    plot_value *res;
    plot_value *arg;
    int i;

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

    res = plot_new_value();
    if( ! res ){
        #if DEBUG
        puts("called to plot_new_value failed");
        #endif
        return 0; /* FIXME error */
    }

    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    for( i=0; i<argc; ++i ){
        arg = args[i];
        if( ! arg ){
            #if DEBUG
            puts("arg was NULL");
            #endif
            return 0; /* FIXME error */
        }

        if( arg->type != plot_type_number ){
            #if DEBUG
            puts("evaled expr did not yield number");
            #endif
            return 0; /* FIXME error */
        }

        if( i == 0 ){
            tmp.u.number.val = arg->u.number.val;
        } else {
            if( ! (tmp.u.number.val < arg->u.number.val) ){
                #if DEBUG
                printf("not 'less than' for i '%d'\n", i);
                #endif
                return res;
            }
            tmp.u.number.val = arg->u.number.val;
        }
    }

    res->u.boolean.val = true;
    return res;

    return 0;
}

/* >
 */
struct plot_value * plot_func_greater(struct plot_env *env, struct plot_value **args, int argc){
    plot_value tmp;
    plot_value *res;
    plot_value *arg;
    int i;

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

    res = plot_new_value();
    if( ! res ){
        #if DEBUG
        puts("called to plot_new_value failed");
        #endif
        return 0; /* FIXME error */
    }

    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    for( i=0; i<argc; ++i ){
        arg = args[i];
        if( ! arg ){
            #if DEBUG
            puts("arg was NULL");
            #endif
            return 0; /* FIXME error */
        }

        if( arg->type != plot_type_number ){
            #if DEBUG
            puts("evaled expr did not yield number");
            #endif
            return 0; /* FIXME error */
        }

        if( i == 0 ){
            tmp.u.number.val = arg->u.number.val;
        } else {
            if( ! (tmp.u.number.val > arg->u.number.val) ){
                #if DEBUG
                printf("not 'greater than' for i '%d'\n", i);
                #endif
                return res;
            }
            tmp.u.number.val = arg->u.number.val;
        }
    }

    res->u.boolean.val = true;
    return res;
    return 0;
}

/* <=
 */
struct plot_value * plot_func_less_equal(struct plot_env *env, struct plot_value **args, int argc){
    plot_value tmp;
    plot_value *res;
    plot_value *arg;
    int i;

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

    res = plot_new_value();
    if( ! res ){
        #if DEBUG
        puts("called to plot_new_value failed");
        #endif
        return 0; /* FIXME error */
    }

    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    for( i=0; i<argc; ++i ){
        arg = args[i];
        if( ! arg ){
            #if DEBUG
            puts("arg was NULL");
            #endif
            return 0; /* FIXME error */
        }

        if( arg->type != plot_type_number ){
            #if DEBUG
            puts("evaled expr did not yield number");
            #endif
            return 0; /* FIXME error */
        }

        if( i == 0 ){
            tmp.u.number.val = arg->u.number.val;
        } else {
            if( ! (tmp.u.number.val <= arg->u.number.val) ){
                #if DEBUG
                printf("not 'less than or equal' for i '%d'\n", i);
                #endif
                return res;
            }
            tmp.u.number.val = arg->u.number.val;
        }
    }

    res->u.boolean.val = true;
    return res;
    return 0;
}

/* >=
 */
struct plot_value * plot_func_greater_equal(struct plot_env *env, struct plot_value **args, int argc){
    plot_value tmp;
    plot_value *res;
    int i;
    plot_value *arg;

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

    res = plot_new_value();
    if( ! res ){
        #if DEBUG
        puts("called to plot_new_value failed");
        #endif
        return 0; /* FIXME error */
    }

    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    for( i=0; i<argc; ++i ){
        arg = args[i];
        if( ! arg ){
            #if DEBUG
            puts("arg was NULL");
            #endif
            return 0; /* FIXME error */
        }

        if( arg->type != plot_type_number ){
            #if DEBUG
            puts("evaled expr did not yield number");
            #endif
            return 0; /* FIXME error */
        }

        if( i == 0 ){
            tmp.u.number.val = arg->u.number.val;
        } else {
            if( ! (tmp.u.number.val >= arg->u.number.val) ){
                #if DEBUG
                printf("not 'less than or equal' for i '%d'\n", i);
                #endif
                return res;
            }
            tmp.u.number.val = arg->u.number.val;
        }
    }

    res->u.boolean.val = true;
    return res;
    return 0;
}

/********* equivalent predicates *********/

/* equal?
 */
struct plot_value * plot_func_equal(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    #if DEBUG
    puts("inside plot_func_equal");
    #endif

    if( ! env ){
        #if DEBUG
        puts("\tenv if NULL");
        #endif
        return 0;
    }

    if( argc != 2 ){
        #if DEBUG
        puts("\tinvalid number of args (expected 2)");
        #endif
        return 0;
    }

    res = plot_new_value();
    if( ! res ){
        #if DEBUG
        puts("\tfailed to allocated result value");
        #endif
        return 0;
    }

    if( ! args[0] || ! args[1] ){
        #if DEBUG
        puts("\targs were NULL");
        #endif
        return 0;
    }

    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    if( args[0]->type != args[1]->type ){
        #if DEBUG
        puts("\targs were not of the same type");
        #endif
        return res;
    }

    switch( args[0]->type ){
        case plot_type_number:
            res->u.boolean.val = args[0]->u.number.val == args[1]->u.number.val;
            break;
        case plot_type_symbol:
            res->u.boolean.val = ! strcmp( args[0]->u.symbol.val, args[1]->u.symbol.val );
            break;
        case plot_type_lambda:
            res->u.boolean.val =
                args[0]->u.lambda.env == args[1]->u.lambda.env
                &&
                args[0]->u.lambda.body == args[1]->u.lambda.body;
            break;
        case plot_type_builtin:
            res->u.boolean.val = args[0]->u.builtin.func == args[1]->u.builtin.func;
            break;
        case plot_type_error:
            plot_fatal_error("plot_func_equal: saw plot_type_error");
            break;
        case plot_type_string:
            res->u.boolean.val = ! strcmp( args[0]->u.string.val, args[1]->u.string.val );
            break;
        case plot_type_boolean:
            res->u.boolean.val = args[0]->u.boolean.val == args[1]->u.boolean.val;
            break;
        case plot_type_reclaimed:
            plot_fatal_error("plot_func_equal: saw plot_type_reclaimed");
            break;
        default:
            plot_fatal_error("plot_func_equal: impossible args[0]->type");
            break;
    }
    return res;
}

/********* value testing functions ********/

/* boolean?
 */
struct plot_value * plot_func_boolean_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;
    plot_value *val;

    #if DEBUG
    puts("inside plot_func_boolean_test");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    res = plot_new_value();
    if( ! res ){
        #if DEBUG
        puts("called to plot_new_value failed");
        #endif
        return 0; /* FIXME error */
    }

    if( argc != 1 ){
        #if DEBUG
        puts("incorrect number of args to plot_func_boolean_test");
        #endif
        return 0; /* FIXME error */
    }

    val = args[0];

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
struct plot_value * plot_func_symbol_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;
    plot_value *val;

    #if DEBUG
    puts("inside plot_func_symbol_test");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    res = plot_new_value();
    if( ! res ){
        #if DEBUG
        puts("called to plot_new_value failed");
        #endif
        return 0; /* FIXME error */
    }

    if( argc != 1 ){
        #if DEBUG
        puts("incorrect number of args to plot_func_symbol_test");
        #endif
        return 0; /* FIXME error */
    }

    val = args[0];

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
struct plot_value * plot_func_string_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;
    plot_value *val;

    #if DEBUG
    puts("inside plot_func_boolean_test");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    res = plot_new_value();
    if( ! res ){
        #if DEBUG
        puts("called to plot_new_value failed");
        #endif
        return 0; /* FIXME error */
    }

    if( argc != 1 ){
        #if DEBUG
        puts("incorrect number of args to plot_func_string_test");
        #endif
        return 0; /* FIXME error */
    }

    val = args[0];

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
struct plot_value * plot_func_number_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;
    plot_value *val;

    #if DEBUG
    puts("inside plot_func_number_test");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    res = plot_new_value();
    if( ! res ){
        #if DEBUG
        puts("called to plot_new_value failed");
        #endif
        return 0; /* FIXME error */
    }

    if( argc != 1 ){
        #if DEBUG
        puts("incorrect number of args to plot_func_number_test");
        #endif
        return 0; /* FIXME error */
    }

    val = args[0];

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
struct plot_value * plot_func_procedure_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;
    plot_value *val;

    #if DEBUG
    puts("inside plot_func_procedure_test");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    res = plot_new_value();
    if( ! res ){
        #if DEBUG
        puts("called to plot_new_value failed");
        #endif
        return 0; /* FIXME error */
    }

    if( argc != 1 ){
        #if DEBUG
        puts("incorrect number of args to plot_func_procedure_test");
        #endif
        return 0; /* FIXME error */
    }

    val = args[0];

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

/* returns 1 if value is considered truthy
 * returns 0 if falsy
 */
int plot_truthy(plot_value *val){
    int ret = 1;
    if( ! val )
        return 0;

    if( val->type == plot_type_boolean && val->u.boolean.val == false )
        ret =  0;

    return ret;
}

/* logical and of all arguments */
struct plot_value * plot_func_and(struct plot_env *env, struct plot_value **args, int argc){
    int i;
    plot_value *ret;

    if( argc < 2 ){
        return plot_runtime_error(plot_error_bad_args, "insufficient arguments, expected 2", "plot_func_add");
    }

    ret = plot_new_value();
    ret->type = plot_type_boolean;
    ret->u.boolean.val = false;

    for( i=0; i<argc; ++i ){
        if( ! plot_truthy(args[i]) ){
            return ret;
        }
    }

    ret->u.boolean.val = true;
    return ret;
}

/* logical or of all arguments */
struct plot_value * plot_func_or(struct plot_env *env, struct plot_value **args, int argc){
    int i;
    plot_value *ret;

    if( argc < 2 ){
        return plot_runtime_error(plot_error_bad_args, "insufficient arguments, expected 2", "plot_func_or");
    }

    ret = plot_new_value();
    ret->type = plot_type_boolean;
    ret->u.boolean.val = true;

    for( i=0; i<argc; ++i ){
        if( plot_truthy(args[i]) ){
            return ret;
        }
    }

    ret->u.boolean.val = false;
    return ret;
}

/* logical not of single argument */
struct plot_value * plot_func_not(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *ret;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "incorrect arguments: expected exactly 1", "plot_func_not");
    }

    ret = plot_new_value();
    ret->type = plot_type_boolean;

    if( plot_truthy(args[0]) ){
        ret->u.boolean.val = false;
        return ret;
    } else {
        ret->u.boolean.val = true;
        return ret;
    }
}

/******* string operations *******/
/* return plot number representing length of string (excluding null terminator)
 * 'number of characters in string'
 */
struct plot_value * plot_func_string_length(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *ret;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected 1 arg", "plot_func_string_length");
    }

    if( args[0]->type != plot_type_string ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_string", "plot_func_string_lenght");
    }

    ret = plot_new_value();
    ret->type = plot_type_number;
    ret->u.number.val = args[0]->u.string.len - 1; /* len includes null terminator */

    return ret;
}

/* (substring string start end)
 * 0 <= start <= end <= (string-length string)
 * returns a string from start (inclusive) to end (exclusive)
 */
struct plot_value * plot_func_substring(struct plot_env *env, struct plot_value **args, int argc){
    /* FIXME */
    return plot_runtime_error(plot_error_unimplemented, "not yet implemented", "plot_func_substring");
}

/* (string-append string ...)
 * returns a new string whose characters form the concatentation
 * of the given strings
 */
struct plot_value * plot_func_string_append(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;
    int i;
    int len = 0;

    if( argc < 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected at least one argument", "plot_func_substring");
    }

    for( i=0; i<argc; ++i ){
        if( args[i]->type != plot_type_string ){
            return plot_runtime_error(plot_error_bad_args, "argument was not of type plot_type_string", "plot_func_substring");
        }
        len += args[i]->u.string.len - 1; /* do not include null terminator in count */
    }

    /* our final null terminator */
    len += 1;

    res = plot_new_value();
    res->type = plot_type_string;
    res->u.string.len = len;
    res->u.string.size = len;
    res->u.string.val = plot_new_string(len);

    /* we now start using len to refer to the next place in our
     * string to write
     */
    len = 0;
    for( i=0; i<argc; ++i ){
        strncpy( &(res->u.string.val[len]), args[i]->u.string.val, args[i]->u.string.len );
        len += args[i]->u.string.len - 1;
    }

    return res;
}

/* (string-copy string)
 * returns a newly allocated copy of the given string
 */
struct plot_value * plot_func_string_copy(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *ret;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected 1 arg", "plot_func_string_copy");
    }

    if( args[0]->type != plot_type_string ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_string", "plot_func_string_copy");
    }

    ret = plot_new_value();
    ret->type = plot_type_string;
    ret->u.string.size = args[0]->u.string.size;
    ret->u.string.len = args[0]->u.string.len;

    ret->u.string.val = plot_new_string(ret->u.string.len);
    strncpy(ret->u.string.val, args[0]->u.string.val, ret->u.string.len - 1);

    return ret;
}

/* (string=? str1 str2)
 * string equality test (case sensitive)
 * returns #t iff both strings are the same length and contain the same characters
 */
struct plot_value * plot_func_string_equal(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *ret;

    /* FIXME r5rs page 30 section 6.3.5 allows
     * for an implementation to generalise equality functions
     * to many args
     */
    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_string_equal");
    }

    /* FIXME r5rs page 30 section 6.3.5 does not seem to mention
     * what happens if procedures are not called with strings
     *
     * runtime error vs false
     */
    if( args[0]->type != plot_type_string ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_string", "plot_func_string_equal");
    }

    if( args[1]->type != plot_type_string ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_string", "plot_func_string_equal");
    }

    ret = plot_new_value();
    ret->type = plot_type_boolean;
    ret->u.boolean.val = false;

    if( args[0]->u.string.len != args[1]->u.string.len ){
        return ret;
    }

    if( strcmp(args[0]->u.string.val, args[1]->u.string.val) ){
        return ret;
    }

    ret->u.boolean.val = true;
    return ret;
}

/* (string-ci=? str1 str2)
 * string equality test that treats upper and lowercase as the same (case insensitive)
 * returns #t iff both string are the same length and contains the same characters
 */
struct plot_value * plot_func_string_ci_equal(struct plot_env *env, struct plot_value **args, int argc){
    int i;
    plot_value *ret, *a, *b;

    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_string_ci_equal");
    }

    a = args[0];
    if( a->type != plot_type_string ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_string", "plot_func_string_ci_equal");
    }

    b = args[1];
    if( b->type != plot_type_string ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_string", "plot_func_string_ci_equal");
    }

    ret = plot_new_value();
    ret->type = plot_type_boolean;
    ret->u.boolean.val = false;

    if( a->u.string.len != b->u.string.len ){
        return ret;
    }

    for( i=0; i< a->u.string.len; ++i ){
        if( tolower(a->u.string.val[i]) != tolower(b->u.string.val[i]) ){
            printf("chars did not match '%c' '%c'\n", a->u.string.val[i], b->u.string.val[i]);
            return ret;
        }
    }

    ret->u.boolean.val = true;
    return ret;
}



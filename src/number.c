#include <stdio.h> /* debugging output */

#include "number.h"
#include "value.h"

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

#define DEBUG 0


/***** mathmatical functions ******/

/* number?
 */
struct plot_value * plot_func_number_test(struct plot_env *env, struct plot_value *args){
    plot_value *val;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_number_test");
    }

    val = car(args);

    if( ! val ){
        return plot_runtime_error(plot_error_internal, "arg was null", "plot_func_number_test");
    }

    return plot_new_boolean( val->type == plot_type_number );
}

/* (+ number1 number2 ...)
 * takes a list of expressions
 * evals each and then adds the expression's value if it is a number
 * if any of the expressions evaluate to something other than an error
 * throw plot_error_bad_args
 */
plot_value * plot_func_add(struct plot_env *env, plot_value *args){
    plot_value *arg;
    int sum=0;

    #if DEBUG
    puts("inside plot_func_add");
    #endif

    for( arg = args; arg->type == plot_type_pair; arg = cdr(arg) ){
        if( car(arg)->type != plot_type_number ){
            #if DEBUG
            puts("arg is not a number");
            #endif
            return 0; /* ERROR */
        }

        sum += car(arg)->u.number.val;
    }

    #if DEBUG
    puts("returning sum of 2 numbers");
    #endif
    return plot_new_number(sum);
}

/* (- number1 number2 ...)
 * takes a list of expressions
 * evals each and then subtracts the expression's value if it is a number
 * if any of the expressions evaluate to something other than an error
 * throw plot_error_bad_args
 */
plot_value * plot_func_subtract(struct plot_env *env, plot_value *args){
    plot_value *arg;
    int difference = 0;

    #if DEBUG
    puts("inside plot_func_subtract");
    #endif

    if( car(args)->type != plot_type_number ){
        return plot_runtime_error(plot_error_bad_args, "argument is not a number", "plot_func_subtract");
    }

    difference = car(args)->u.number.val;

    for( arg = cdr(args); arg->type == plot_type_pair; arg = cdr(arg) ){
        if( car(arg)->type != plot_type_number ){
            return plot_runtime_error(plot_error_bad_args, "argument is not a number", "plot_func_subtract");
        }

        difference -= car(arg)->u.number.val;
    }
    #if DEBUG
    puts("returning difference");
    #endif

    return plot_new_number(difference);
}

/* (* number1 number2 ...)
 * takes a list of expressions
 * evals each and then multiplies the expression's value if it is a number
 * if any of the expressions evaluate to something other than an error
 * throw plot_error_bad_args
 */
plot_value * plot_func_multiply(struct plot_env *env, plot_value *args){
    plot_value *arg;
    int product=1;

    #if DEBUG
    puts("inside plot_func_multiply");
    #endif

    for( arg = args; arg->type == plot_type_pair; arg = cdr(arg) ){
        if( car(arg)->type != plot_type_number ){
            return plot_runtime_error(plot_error_bad_args, "argument is not a number", "plot_func_subtract");
        }

        product *= car(arg)->u.number.val;
    }

    #if DEBUG
    puts("returning product");
    #endif
    return plot_new_number(product);
}

/* (/ number1 number2 ...)
 * integer division
 * exact only
 */
struct plot_value * plot_func_divide(struct plot_env *env, struct plot_value *args){
    plot_value *arg;
    int quotient=0;

    #if DEBUG
    puts("inside plot_func_divide");
    #endif

    if( car(args)->type != plot_type_number ){
        return plot_runtime_error(plot_error_bad_args, "argument is not a number", "plot_func_divide");
    }

    quotient = car(args)->u.number.val;

    for( arg = args; arg->type == plot_type_pair; arg = cdr(arg) ){
        if( car(arg)->type != plot_type_number ){
            return plot_runtime_error(plot_error_bad_args, "argument is not a number", "plot_func_divide");
        }

        quotient /= car(arg)->u.number.val;
    }

    #if DEBUG
    puts("returning quotient");
    #endif

    return plot_new_number(quotient);
}

/* (remainder number1 number2 ...)
 * remainder
 */
struct plot_value * plot_func_remainder(struct plot_env *env, struct plot_value *args){
    plot_value *arg;
    int remainder=0;

    #if DEBUG
    puts("inside plot_func_remainder");
    #endif

    if( car(args)->type != plot_type_number ){
        return plot_runtime_error(plot_error_bad_args, "argument is not a number", "plot_func_divide");
    }

    remainder = car(args)->u.number.val;

    for( arg = cdr(args); arg->type == plot_type_pair; arg = cdr(arg) ){
        if( car(arg)->type != plot_type_number ){
            return plot_runtime_error(plot_error_bad_args, "argument is not a number", "plot_func_divide");
        }

        remainder %= car(arg)->u.number.val;
    }

    #if DEBUG
    puts("returning remainder");
    #endif

    return plot_new_number(remainder);
}



/******** comparison functions *******/

/* (= number1 number2 ...)
 * mathmatical =
 */
struct plot_value * plot_func_math_equal(struct plot_env *env, struct plot_value *args){
    plot_value *tmp;
    plot_value *cur;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected at least 2 args", "plot_func_equal");
    }

    tmp = car(args);
    for( cur = cdr(args); cur->type == plot_type_pair; cur = cdr(cur) ){
        if( car(cur)->type != plot_type_number ){
            return plot_runtime_error(plot_error_bad_args, "argument was not of type plot_type_number", "plot_func_equal");
        }

        if( tmp->u.number.val != car(cur)->u.number.val){
            return plot_new_boolean(false);
        }
        tmp = car(cur);
    }

    return plot_new_boolean(true);
}

/* (< number1 number2 ...)
 */
struct plot_value * plot_func_less(struct plot_env *env, struct plot_value *args){
    plot_value *tmp;
    plot_value *cur;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected at least 2 args", "plot_func_less");
    }

    tmp = car(args);
    for( cur = cdr(args); cur->type == plot_type_pair; cur = cdr(cur) ){
        if( car(cur)->type != plot_type_number ){
            return plot_runtime_error(plot_error_bad_args, "argument was not of type plot_type_number", "plot_func_less");
        }

        if( ! (tmp->u.number.val < car(cur)->u.number.val) ){
            return plot_new_boolean(false);
        }
        tmp = car(cur);
    }

    return plot_new_boolean(true);
}

/* (> number1 number2 ...)
 */
struct plot_value * plot_func_greater(struct plot_env *env, struct plot_value *args){
    plot_value *tmp;
    plot_value *cur;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected at least 2 args", "plot_func_greater");
    }

    tmp = car(args);
    for( cur = cdr(args); cur->type == plot_type_pair; cur = cdr(cur) ){
        if( car(cur)->type != plot_type_number ){
            return plot_runtime_error(plot_error_bad_args, "argument was not of type plot_type_number", "plot_func_greater");
        }

        if( ! (tmp->u.number.val > car(cur)->u.number.val) ){
            return plot_new_boolean(false);
        }
        tmp = car(cur);
    }

    return plot_new_boolean(true);
}

/* (<= number1 number2 ...)
 */
struct plot_value * plot_func_less_equal(struct plot_env *env, struct plot_value *args){
    plot_value *tmp;
    plot_value *cur;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected at least 2 args", "plot_func_less_equal");
    }

    tmp = car(args);
    for( cur = cdr(args); cur->type == plot_type_pair; cur = cdr(cur) ){
        if( car(cur)->type != plot_type_number ){
            return plot_runtime_error(plot_error_bad_args, "argument was not of type plot_type_number", "plot_func_less_equal");
        }

        if( ! (tmp->u.number.val <= car(cur)->u.number.val) ){
            return plot_new_boolean(false);
        }
        tmp = car(cur);
    }

    return plot_new_boolean(true);
}

/* (>= number1 number2 ...)
 */
struct plot_value * plot_func_greater_equal(struct plot_env *env, struct plot_value *args){
    plot_value *tmp;
    plot_value *cur;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected at least 2 args", "plot_func_greater_equal");
    }

    tmp = car(args);
    for( cur = cdr(args); cur->type == plot_type_pair; cur = cdr(cur) ){
        if( car(cur)->type != plot_type_number ){
            return plot_runtime_error(plot_error_bad_args, "argument was not of type plot_type_number", "plot_func_greater_equal");
        }

        if( ! (tmp->u.number.val >= car(cur)->u.number.val) ){
            return plot_new_boolean(false);
        }
        tmp = car(cur);
    }

    return plot_new_boolean(true);
}



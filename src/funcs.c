#include <stdlib.h>
#include <stdio.h>

#include "value.h"
#include "types.h"
#include "hash.h"
#include "env.h"
#include "funcs.h"
#include "eval.h"

#define DEBUG 1

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
        tmp = plot_eval(env, arg);
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

    res = calloc(1, sizeof *res);
    if( !res ){
        #if DEBUG
        puts("res failed to calloc");
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

/* print error information and then exit
 */
void plot_handle_error(const plot_value *error, const char *place){
    char *type = "unknown";
    if( ! error->type == plot_type_error ){
        printf("Error encountered in '%s', invalid error value supplied\n", place);
        exit(1);
    }

    switch(error->u.error.type){
        case plot_error_alloc_failed:
            type = "alloc failed";
            break;
        case plot_error_bad_args:
            type = "bad args";
            break;
    }

    printf("Error encountered in '%s', error message: '%s', error type: '%s'\n", place, error->u.error.msg, type);
    exit(1);
}



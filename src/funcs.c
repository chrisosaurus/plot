#include <stdlib.h>
#include <stdio.h>

#include "value.h"
#include "hash.h"
#include "env.h"
#include "funcs.h"

#define DEBUG 0

plot_value * plot_func_add(plot_env *env, plot_value *v1, plot_value *v2){
    plot_value *res;

    #if DEBUG
    puts("inside plot_func_add");
    #endif

    if( !env || !v1 || !v2 ){
        #if DEBUG
        puts("env, v1 or v2 are null");
        #endif
        return 0; /* ERROR */
    }


    if( ! (v1->type == plot_type_number) ){
        #if DEBUG
        puts("v1 is not a number");
        #endif
        return 0; /* ERROR */
    }

    if( ! (v2->type == plot_type_number) ){
        #if DEBUG
        puts("v2 is not a number");
        #endif
        return 0; /* ERROR */
    }


    res = calloc(1, sizeof *res);
    if( !res ){
        #if DEBUG
        puts("res failed to calloc");
        #endif
        return 0; /* ERROR */
    }

    res->type = plot_type_number;
    res->u.number.val = v1->u.number.val + v2->u.number.val;

    #if DEBUG
    puts("returning sum of 2 numbers");
    #endif
    return res;
}

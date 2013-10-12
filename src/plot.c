#include <stdlib.h>
#include <stdio.h>

#include "value.h"
#include "env.h"
#include "funcs.h"
#include "plot.h"

#define LENGTH(x) (sizeof x / sizeof x[0])

/* functions to bind */
struct plot_binding {
    plot_symbol sym;
    plot_value func;
};

struct plot_binding bindings[] = {
    {{"+",         2,  2}, {plot_type_function, {.function = {0, plot_func_add}}}},
    {{"-",         2,  2}, {plot_type_function, {.function = {0, plot_func_subtract}}}},
    {{"*",         2,  2}, {plot_type_function, {.function = {0, plot_func_multiply}}}},
    {{"/",         2,  2}, {plot_type_function, {.function = {0, plot_func_divide}}}},
    {{"remainder",10, 10}, {plot_type_function, {.function = {0, plot_func_remainder}}}},
    {{"display",   7,  7}, {plot_type_function, {.function = {0, plot_func_display}}}},
    {{"newline",   7,  7}, {plot_type_function, {.function = {0, plot_func_newline}}}}
};

plot * plot_init(void){
    plot * p;
    size_t i=0;

    p = calloc(1, sizeof *p);
    if( ! p ){
        puts("alloc failed in plot_init");
        return 0;
    }

    p->env = plot_env_init(0);
    if( ! p->env ){
        puts("call to plot_env_init failed");
        plot_cleanup(p);
        return 0;
    }

    for( i=0; i<LENGTH(bindings); ++i ){
        if( ! plot_env_define( p->env, &(bindings[i].sym), &(bindings[i].func) ) ){
            printf("error in plot_init defining symbol '%s'\n", bindings[i].sym.val);
        }
    }

    return p;
}

void plot_cleanup(plot * p){
    plot_env_cleanup(p->env);
    free(p);
}


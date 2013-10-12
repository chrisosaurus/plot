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
    /* math functions */
    {{"+",         2,  2}, {plot_type_builtin, {.builtin = {plot_func_add}}}},
    {{"-",         2,  2}, {plot_type_builtin, {.builtin = {plot_func_subtract}}}},
    {{"*",         2,  2}, {plot_type_builtin, {.builtin = {plot_func_multiply}}}},
    {{"/",         2,  2}, {plot_type_builtin, {.builtin = {plot_func_divide}}}},
    {{"remainder",10, 10}, {plot_type_builtin, {.builtin = {plot_func_remainder}}}},

    /* comparison functions */
    {{"=",         2,  2}, {plot_type_builtin, {.builtin = {plot_func_equal}}}},
    {{"<",         2,  2}, {plot_type_builtin, {.builtin = {plot_func_less}}}},
    {{">",         2,  2}, {plot_type_builtin, {.builtin = {plot_func_greater}}}},
    {{"<=",        2,  2}, {plot_type_builtin, {.builtin = {plot_func_less_equal}}}},
    {{">=",        2,  2}, {plot_type_builtin, {.builtin = {plot_func_greater_equal}}}},

    /* value testing functions */
    {{"boolean?",        9,  9}, {plot_type_builtin, {.builtin = {plot_func_boolean_test}}}},
    {{"string?",         8,  8}, {plot_type_builtin, {.builtin = {plot_func_string_test}}}},
    {{"symbol?",         8,  8}, {plot_type_builtin, {.builtin = {plot_func_symbol_test}}}},
    {{"number?",         8,  8}, {plot_type_builtin, {.builtin = {plot_func_number_test}}}},
    {{"procedure?",     11, 11}, {plot_type_builtin, {.builtin = {plot_func_procedure_test}}}},

    /* display functions */
    {{"display",   7,  7}, {plot_type_builtin, {.builtin = {plot_func_display}}}},
    {{"newline",   7,  7}, {plot_type_builtin, {.builtin = {plot_func_newline}}}}
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


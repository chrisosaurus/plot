#include <stdlib.h>
#include <stdio.h>

#include "value.h"
#include "env.h"
#include "funcs.h"
#include "plot.h"

#define LENGTH(x) (sizeof x / sizeof x[0])

typedef struct plot {
    struct plot_env *env;
} plot;

static plot *plot_instance;

/* functions to bind */
struct plot_binding {
    plot_symbol sym;
    plot_value func;
};

struct plot_binding bindings[] = {
    /* math functions */
    {{"+",         2,  2}, {{-1}, plot_type_builtin, {.builtin = {plot_func_add}}}},
    {{"-",         2,  2}, {{-1}, plot_type_builtin, {.builtin = {plot_func_subtract}}}},
    {{"*",         2,  2}, {{-1}, plot_type_builtin, {.builtin = {plot_func_multiply}}}},
    {{"/",         2,  2}, {{-1}, plot_type_builtin, {.builtin = {plot_func_divide}}}},
    {{"remainder",10, 10}, {{-1}, plot_type_builtin, {.builtin = {plot_func_remainder}}}},

    /* comparison functions */
    {{"=",         2,  2}, {{-1}, plot_type_builtin, {.builtin = {plot_func_equal}}}},
    {{"<",         2,  2}, {{-1}, plot_type_builtin, {.builtin = {plot_func_less}}}},
    {{">",         2,  2}, {{-1}, plot_type_builtin, {.builtin = {plot_func_greater}}}},
    {{"<=",        2,  2}, {{-1}, plot_type_builtin, {.builtin = {plot_func_less_equal}}}},
    {{">=",        2,  2}, {{-1}, plot_type_builtin, {.builtin = {plot_func_greater_equal}}}},

    /* value testing functions */
    {{"boolean?",        9,  9}, {{-1}, plot_type_builtin, {.builtin = {plot_func_boolean_test}}}},
    {{"string?",         8,  8}, {{-1}, plot_type_builtin, {.builtin = {plot_func_string_test}}}},
    {{"symbol?",         8,  8}, {{-1}, plot_type_builtin, {.builtin = {plot_func_symbol_test}}}},
    {{"number?",         8,  8}, {{-1}, plot_type_builtin, {.builtin = {plot_func_number_test}}}},
    {{"procedure?",     11, 11}, {{-1}, plot_type_builtin, {.builtin = {plot_func_procedure_test}}}},

    /* display functions */
    {{"display",   7,  7}, {{-1}, plot_type_builtin, {.builtin = {plot_func_display}}}},
    {{"newline",   7,  7}, {{-1}, plot_type_builtin, {.builtin = {plot_func_newline}}}}
};

int plot_init(void){
    size_t i=0;

    plot_instance = calloc(1, sizeof *plot_instance);
    if( ! plot_instance ){
        puts("alloc failed in plot_init");
        return 0;
    }

    plot_instance->env = plot_env_init(0);
    if( ! plot_instance->env ){
        puts("call to plot_env_init failed");
        plot_cleanup();
        return 0;
    }

    for( i=0; i<LENGTH(bindings); ++i ){
        if( ! plot_env_define( plot_instance->env, &(bindings[i].sym), &(bindings[i].func) ) ){
            printf("error in plot_init defining symbol '%s'\n", bindings[i].sym.val);
        }
    }

    return 1;
}

struct plot_env * plot_get_env(void){
    if( ! plot_instance )
        return 0;

    return plot_instance->env;
}

void plot_cleanup(){
    plot_env_cleanup(plot_instance->env);
    free(plot_instance);
}

/* increase reference count on object */
void plot_incr(struct plot_gc *p){
    if( !p )
        return;
}

/* decrease reference count on object
 * may trigger collection
 */
void plot_decr(struct plot_gc *p){
    if( !p )
        return;
}

/* get new value */
struct plot_value * plot_new_value(void){
    struct plot_value *v =  calloc(1, sizeof(struct plot_value ));
    if( ! v ){
        /* TODO FIXME use plot error handling */
        puts("plot_new_value: calloc failed, dying");
        exit(1);
    }
    return v;
}




#include <stdlib.h>
#include <stdio.h>

#include "value.h"
#include "hash.h"
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

/* get new env */
struct plot_env * plot_new_env(void){
    struct plot_env *e;
    e = calloc(1, sizeof *e);
    if( ! e ){
        /* TODO FIXME use plot error handling */
        puts("plot_new_env: calloc failed, dying");
        exit(1);
    }
    return e;
}

/* get new hash */
struct plot_hash * plot_new_hash(void){
    struct plot_hash *h;
    h = calloc(1, sizeof *h);
    if( ! h ){
        /* TODO FIXME use plot error handling */
        puts("plot_new_hash: calloc failed, dying");
        exit(1);
    }
    return h;
}

/* get new hash entry */
struct plot_hash_entry * plot_new_hash_entry(void){
    struct plot_hash_entry *he;
    he = calloc(1, sizeof *he);
    if( ! he ){
        /* TODO FIXME use plot error handling */
        puts("plot_new_hash_entry: calloc failed, dying");
        exit(1);
    }
    return he;
}

/* get new string */
char * plot_new_string(int len){
    char * c;
    c = calloc(len, sizeof *c);
    if( ! c ){
        /* TODO FIXME use plot error handling */
        puts("plot_new_string: calloc failed, dying");
        exit(1);
    }
    return c;
}


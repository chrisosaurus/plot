#include <stdlib.h>
#include <stdio.h>

#include "value.h"
#include "hash.h"
#include "env.h"
#include "funcs.h"
#include "plot.h"

#define LENGTH(x) (sizeof x / sizeof x[0])

#ifndef PLOT_TESTS
#define GC_STATS 1
#endif

typedef struct plot {
    struct plot_env *env;

    /* array of malloc'd values */
    struct plot_value *arena;
    /* number of values in above arena */
    int num_values_allocated;
    /* number of values currently in use by callers */
    int num_values_used;

    /* head of list of reclaimed plot_value(s) */
    struct plot_value *reclaimed;

    /**** garbage stats ****/
    /* number of plot_value(s) reclaimed (garbage collected) */
    int num_reclaimed;
    /* number of plot_value(s) recycling (handed back out after GC) */
    int num_recycled;
} plot;

static plot *plot_instance;

/* functions to bind */
struct plot_binding {
    plot_symbol sym;
    plot_value func;
};

struct plot_binding bindings[] = {
    /* math functions */
    {{"+",         2,  2}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_add}}}},
    {{"-",         2,  2}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_subtract}}}},
    {{"*",         2,  2}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_multiply}}}},
    {{"/",         2,  2}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_divide}}}},
    {{"remainder",10, 10}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_remainder}}}},

    /* comparison functions */
    {{"=",         2,  2}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_equal}}}},
    {{"<",         2,  2}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_less}}}},
    {{">",         2,  2}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_greater}}}},
    {{"<=",        2,  2}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_less_equal}}}},
    {{">=",        2,  2}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_greater_equal}}}},

    /* value testing functions */
    {{"boolean?",        9,  9}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_boolean_test}}}},
    {{"string?",         8,  8}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_string_test}}}},
    {{"symbol?",         8,  8}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_symbol_test}}}},
    {{"number?",         8,  8}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_number_test}}}},
    {{"procedure?",     11, 11}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_procedure_test}}}},

    /* display functions */
    {{"display",   7,  7}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_display}}}},
    {{"newline",   7,  7}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_newline}}}}
};

void plot_value_init(void);

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

    plot_value_init();

    return 1;
}

struct plot_env * plot_get_env(void){
    if( ! plot_instance )
        return 0;

    return plot_instance->env;
}

void plot_cleanup(){
#if GC_STATS
    int lost = 0;
    plot_value *v;

    printf("\nplot GC stats:\n");
    printf("\tMax in use '%d'\n", plot_instance->num_values_used);
    printf("\tStill had in the bank: '%d'\n", plot_instance->num_values_allocated - plot_instance->num_values_used );
    printf("\tnum recycled '%d', reclaimed '%d'\n", plot_instance->num_recycled, plot_instance->num_reclaimed);
    printf("\tused - reclaimed '%d'\n", plot_instance->num_values_used - plot_instance->num_reclaimed);
    for( v = plot_instance->reclaimed; v; v = (plot_value *) v->gc.next )
        ++lost;
    printf("\tnon-reclaimed (still in use at cleanup) '%d'\n\n", lost);
#endif

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

/* increase reference count on plot_value */
void plot_value_incr(struct plot_value *p){
    if( !p )
        return;

    if( p->gc.refcount < 0 ){
        /* object is NOT under control of gc, do not touch */
        return;
    }

    if( p->gc.refcount > 0 ){
        ++p->gc.refcount;
    } else {
        /* this object already has a refcount of 0, this should be impossible, ERROR */
        puts("plot_value_incr: This object already has a refcount of 0, ERROR has occurred, terminating");
        exit(1);
    }
}

/* decrease reference count on plot_value
 * may trigger collection
 */
void plot_value_decr(struct plot_value *p){
    if( !p )
        return;

    if( p->gc.refcount < 0 ){
        /* object is NOT under control of gc, do not touch */
        return;
    }

    if( p->gc.refcount > 0 ){
        --p->gc.refcount;
        if( p->gc.refcount == 0 ){
            ++plot_instance->num_reclaimed;
            /* time to reclaim */
            //fprintf(stderr, "RECLAIMING\n"); // 2692537
            p->gc.next = (struct plot_gc *) plot_instance->reclaimed;
            plot_instance->reclaimed = (struct plot_value *) p;
            p->type = plot_type_reclaimed; /* FIXME useful for testing */
        }
    } else {
        /* this object already has a refcount of 0, ERROR */
        puts("plot_value_decr: This object already has a refcount of 0, ERROR has occurred, terminating");
        exit(1);
    }
}

/* initialise value arena
 *
 * returns on success
 * will cause a fatal error and exit program on failure
 */
void plot_value_init(void){
    if( ! plot_instance ){
        puts("plot_value_init called without plot_instance being initialised");
        exit(1);
    }

    plot_instance->num_values_used = 0;
    plot_instance->reclaimed = 0;
    /* FIXME
     * before gc fibo(31) would require 6731342 plot_values
     * after tracking waste, this included 2692537 wasted values (mostly from if test position)
     * this means the minimum size of num)values_allocated must be 6731342 - 2692537 = 4038805
     */
    plot_instance->num_values_allocated = 4100000;
    plot_instance->arena = calloc( plot_instance->num_values_allocated, sizeof (struct plot_value) );
    if( ! plot_instance->arena ){
        puts("plot_value_init ERROR: failed to calloc arena");
        exit(1);
    }
}

/* get new value */
struct plot_value * plot_new_value(void){
    struct plot_value *p;
    if( ! plot_instance ){
        puts("plot_new_value called without plot_instance being initialised");
        exit(1);
    }

    if( plot_instance->reclaimed ){
        ++plot_instance->num_recycled;
        //fprintf(stderr, "reusing\n");
        p = plot_instance->reclaimed;
        /* gc is the first element of plot_value so this is safe */
        plot_instance->reclaimed = (plot_value *) p->gc.next;
        p->gc.refcount = 1;
        p->gc.next = 0;
        return p;
    }
    if( plot_instance->num_values_used >= plot_instance->num_values_allocated ){
        /* FIXME realloc */
        printf("THE BANK IS EMPTY; allocated all '%d' plot_value(s)\n", plot_instance->num_values_used);
        exit(1);
    } else {
        /* hand out resources */
        p = &(plot_instance->arena[ plot_instance->num_values_used ++]);
        p->gc.refcount = 1;
        p->gc.next = 0;
        return p;
    }
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


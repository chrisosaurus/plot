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

#define DEBUG 0

typedef struct plot {
    /* error value returned by plot_error */
    struct plot_value error;
    struct plot_env *env;

    /**** GC arena ****/
    /* array of malloc'd values */
    struct plot_value *value_arena;
    /* number of values in above arena */
    int num_value_allocated;
    /* number of values currently in use by callers */
    int num_value_used;
    /* head of list of reclaimed plot_value(s) */
    struct plot_value *value_reclaimed;

    struct plot_env *env_arena;
    int num_env_allocated;
    int num_env_used;
    struct plot_env *env_reclaimed;

    struct plot_hash_entry *he_arena;
    int num_he_allocated;
    int num_he_used;
    struct plot_hash_entry *he_reclaimed;

#if GC_STATS
    /**** garbage stats ****/
    /* number of plot_value(s) reclaimed (garbage collected) */
    int num_value_reclaimed;
    /* number of plot_value(s) recycling (handed back out after GC) */
    int num_value_recycled;

    int num_env_reclaimed;
    int num_env_recycled;

    int num_he_reclaimed;
    int num_he_recycled;
#endif
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
    {{"=",         2,  2}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_math_equal}}}},
    {{"<",         2,  2}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_less}}}},
    {{">",         2,  2}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_greater}}}},
    {{"<=",        2,  2}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_less_equal}}}},
    {{">=",        2,  2}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_greater_equal}}}},

    /* equivalent predicates */
    {{"equal?",    2,  2}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_equal}}}},

    /* value testing functions */
    {{"boolean?",        9,  9}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_boolean_test}}}},
    {{"string?",         8,  8}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_string_test}}}},
    {{"symbol?",         8,  8}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_symbol_test}}}},
    {{"number?",         8,  8}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_number_test}}}},
    {{"procedure?",     11, 11}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_procedure_test}}}},
    {{"char?",           6,  6}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_char_test}}}},

    /* display functions */
    {{"display",   7,  7}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_display}}}},
    {{"newline",   7,  7}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_newline}}}},

    /* logical operations */
    {{"and",   4,  4}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_and}}}},
    {{"or",    3,  3}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_or}}}},
    {{"not",   4,  4}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_not}}}},

    /* string procedures */
    {{"string-length",  14, 14}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_string_length}}}},
    {{"substring",      10, 10}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_substring}}}},
    {{"string-append",  14, 14}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_string_append}}}},
    {{"string-copy",    12, 12}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_string_copy}}}},
    {{"string=?",        9,  9}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_string_equal}}}},
    {{"string-ci=?",    12, 12}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_string_ci_equal}}}},

    /* character procedures */
    {{"char=?",             7,  7}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_char_equal_test}}}},
    {{"char-ci=?",         10, 10}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_char_ci_equal_test}}}},
    {{"char<?",             7,  7}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_char_less_test}}}},
    {{"char>?",             7,  7}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_char_greater_test}}}},
    {{"char<=?",            8,  8}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_char_less_equal_test}}}},
    {{"char>=?",            8,  8}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_char_greater_equal_test}}}},
    {{"char-ci<?",         10, 10}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_char_ci_less_test}}}},
    {{"char-ci>?",         10, 10}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_char_ci_greater_test}}}},
    {{"char-ci<=?",        11, 11}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_char_ci_less_equal_test}}}},
    {{"char-ci>=?",        11, 11}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_char_ci_greater_equal_test}}}},

    {{"char-alphabetic?",  17, 17}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_char_alphabetic_test}}}},
    {{"char-numeric?",     14, 14}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_char_numeric_test}}}},
    {{"char-whitespace?",  17, 17}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_char_whitespace_test}}}},
    {{"char-upper-case?",  17, 17}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_char_upper_case_test}}}},
    {{"char-lower-case?",  17, 17}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_char_lower_case_test}}}},
    {{"char->integer",     14, 14}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_char_to_integer}}}},
    {{"integer->char",     14, 14}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_integer_to_char}}}},
    {{"char-upcase",       12, 12}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_char_upcase}}}},
    {{"char-downcase",     14, 14}, {{-1, 0}, plot_type_builtin, {.builtin = {plot_func_char_downcase}}}}
};

static void plot_gc_incr(struct plot_gc *g);
static void plot_gc_value_init(void);
static void plot_gc_env_init(void);
static void plot_gc_he_init(void);

int plot_init(void){
    size_t i=0;

    plot_instance = calloc(1, sizeof *plot_instance);
    if( ! plot_instance ){
        puts("alloc failed in plot_init");
        return 0;
    }

    plot_instance->error.gc.refcount = -1;
    plot_instance->error.type = plot_type_error;

    plot_gc_value_init();
    plot_gc_he_init();
    plot_gc_env_init();

    plot_instance->env = plot_new_env(0);
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
#if GC_STATS

    printf("\nplot GC stats:\n");

    printf("##### plot_value stats #####\n");
    printf("\tMax in use '%d'\n", plot_instance->num_value_used);
    printf("\tStill had in the bank: '%d'\n", plot_instance->num_value_allocated - plot_instance->num_value_used );
    printf("\tnum recycled '%d', reclaimed '%d'\n", plot_instance->num_value_recycled, plot_instance->num_value_reclaimed);
    printf("\tused - reclaimed = '%d'\n", plot_instance->num_value_used - plot_instance->num_value_reclaimed);
    printf("\tnon-reclaimed (still in use at cleanup) '%d'\n", (plot_instance->num_value_used + plot_instance->num_value_recycled) - plot_instance->num_value_reclaimed);

    printf("##### plot_env stats #####\n");
    printf("\tMax in use '%d'\n", plot_instance->num_env_used);
    printf("\tStill had in the bank: '%d'\n", plot_instance->num_env_allocated - plot_instance->num_env_used );
    printf("\tnum recycled '%d', reclaimed '%d'\n", plot_instance->num_env_recycled, plot_instance->num_env_reclaimed);
    printf("\tused - reclaimed = '%d'\n", plot_instance->num_env_used - plot_instance->num_env_reclaimed);
    printf("\tnon-reclaimed (still in use at cleanup) '%d'\n", (plot_instance->num_env_used + plot_instance->num_env_recycled) - plot_instance->num_env_reclaimed);

    printf("##### plot_hash_entry stats #####\n");
    printf("\tMax in use '%d'\n", plot_instance->num_he_used);
    printf("\tStill had in the bank: '%d'\n", plot_instance->num_he_allocated - plot_instance->num_he_used );
    printf("\tnum recycled '%d', reclaimed '%d'\n", plot_instance->num_he_recycled, plot_instance->num_he_reclaimed);
    printf("\tused - reclaimed = '%d'\n", plot_instance->num_he_used - plot_instance->num_he_reclaimed);
    printf("\tnon-reclaimed (still in use at cleanup) '%d'\n", (plot_instance->num_he_used + plot_instance->num_he_recycled) - plot_instance->num_he_reclaimed);


    printf("\n");

#endif

    plot_env_cleanup(plot_instance->env);
    free(plot_instance);
}

/* generates an appropriate error value, prints it, and then returns it
 * caller is expected to return the result
 *
 * each frame within the callstack is expected to detect this,
 * print relevant trace information, and then return the value further
 */
struct plot_value * plot_runtime_error(plot_error_type type, const char *msg, const char *location){
    plot_value *err;
    const char *error_type_str;

    if( ! plot_instance )
        plot_fatal_error("plot_runtime_error called without plot_instance being initialised");

    err = &plot_instance->error;
    err->type = plot_type_error;
    err->u.error.type = type;
    err->u.error.msg = msg;
    err->u.error.location = location;

    switch(err->u.error.type){
        case plot_error_alloc_failed:
            error_type_str = "alloc failed";
            break;
        case plot_error_bad_args:
            error_type_str = "bad args";
            break;
        case plot_error_internal:
            error_type_str = "internal error";
            break;
        case plot_error_unbound_symbol:
            error_type_str = "unbound symbol";
            break;
        default:
            error_type_str = "IMPOSSIBLE ERROR";
            break;
    }

    printf("Error encountered in '%s', error message: '%s', error type: '%s'\n", err->u.error.location, err->u.error.msg, error_type_str);
    return err;
}

/* a fatal error will print the supplied string and then `exit(1)`
 */
void plot_fatal_error(const char *str){
    puts("FATAL ERROR OCCURED:");
    puts(str);
    exit(1);
}

void plot_gc_incr(struct plot_gc *g){
    if( !g )
        return;

    if( g->refcount < 0 ){
        /* object is NOT under control of gc, do not touch */
        return;
    }

    if( g->refcount > 0 ){
        ++g->refcount;
    } else {
        /* this object already has a refcount of 0, this should be impossible, ERROR */
        puts("plot_value_incr: This object already has a refcount of 0, ERROR has occurred, terminating");
        exit(1);
    }
}

/* increase reference count on plot_value */
void plot_value_incr(struct plot_value *p){
    plot_gc_incr( (struct plot_gc *) p);
}

/* decrease reference count on plot_value
 * may trigger collection
 */
void plot_value_decr(struct plot_value *p){
    if( !p )
        return;

    #if DEBUG
    printf("inside plot_value_decr looking at object '%p' with refcount '%d'\n",
          (void*) p, p->gc.refcount );
    #endif

    if( p->gc.refcount < 0 ){
        #if DEBUG
        puts("\tthis object is NOT under gc control, leaving untouched");
        #endif
        /* object is NOT under control of gc, do not touch */
        return;
    }

    if( p->gc.refcount > 0 ){
        #if DEBUG
        puts("\tdecrementing refcount");
        #endif
        --p->gc.refcount;

        if( p->gc.refcount == 0 ){
            #if DEBUG
            puts("\treclaiming value");
            #endif
            #if GC_STATS
            ++plot_instance->num_value_reclaimed;
            #endif
            /* time to reclaim */
            //fprintf(stderr, "RECLAIMING\n"); // 2692537
            p->gc.next = (struct plot_gc *) plot_instance->value_reclaimed;
            plot_instance->value_reclaimed = p;
            /* if plot_value is a lambda we must also decr the env */
            if( p->type == plot_type_lambda ){
                plot_env_decr(p->u.lambda.env);
            }
            p->type = plot_type_reclaimed; /* FIXME useful for testing */
        #if DEBUG
        } else {
            puts("\tsparing object");
        #endif
        }
    } else {
        /* this object already has a refcount of 0, ERROR */
        puts("plot_value_decr: This object already has a refcount of 0, ERROR has occurred, terminating");
        exit(1);
    }

    #if DEBUG
    puts("\tleaving plot_value_decr");
    #endif
}

/* initialise value arena
 *
 * returns on success
 * will cause a fatal error and exit program on failure
 */
void plot_gc_value_init(void){
    if( ! plot_instance ){
        puts("plot_gc_value_init called without plot_instance being initialised");
        exit(1);
    }

    plot_instance->num_value_used = 0;
    #if GC_STATS
    plot_instance->num_value_reclaimed = 0;
    #endif
    /* FIXME
     * before gc (fibo 31) would require 6731342 plot_values
     * after tracking waste, this included 2692537 wasted values (mostly from if test position)
     * this means the minimum size of num)values_allocated must be 6731342 - 2692537 = 4038805
     *
     * now after func-call-temp-reclaiming this value can be reduced to 2692574
     *
     * and now, after enc-cleanup (fibo 31) only needs 52
     */
    plot_instance->num_value_allocated = 1000;
    plot_instance->value_arena = calloc( plot_instance->num_value_allocated, sizeof (struct plot_value) );
    if( ! plot_instance->value_arena ){
        puts("plot_gc_value_init ERROR: failed to calloc value arena");
        exit(1);
    }
}

/* increase reference count on plot_env */
void plot_env_incr(struct plot_env *e){
    plot_gc_incr( (struct plot_gc *) e);
}

/* decrease reference count on plot_env
 * may trigger collection of the env,
 * it's parents and it's stored values
 */
void plot_env_decr(struct plot_env *e){
    if( !e )
        return;

    #if DEBUG
    printf("inside plot_env_decr considering object '%p' with refcount '%d'\n",
          (void*) e, e->gc.refcount);
    #endif

    if( e->gc.refcount < 0 ){
        #if DEBUG
        puts("\tthis object is not under GC control, leaving untouched");
        #endif
        /* object is NOT under control of gc, do not touch */
        return;
    }

    if( e->gc.refcount > 0 ){
        #if DEBUG
        puts("\tdecrementing refcount");
        #endif

        --e->gc.refcount;
        if( e->gc.refcount == 0 ){
            #if DEBUG
            puts("\treclaiming object");
            #endif
            #if GC_STATS
            ++plot_instance->num_env_reclaimed;
            #endif
            /* time to reclaim */
            //fprintf(stderr, "RECLAIMING\n"); // 2692537
            e->gc.next = (struct plot_gc *) plot_instance->env_reclaimed;
            plot_instance->env_reclaimed = e;

            /* env cleanup will decr parent and trigger decr on all stored values */
            plot_env_cleanup(e);
        #if DEBUG
        } else {
            puts("\tsparing object");
        #endif
        }
    } else {
        /* this object already has a refcount of 0, ERROR */
        puts("plot_env_decr: This object already has a refcount of 0, ERROR has occurred, terminating");
        exit(1);
    }

    #if DEBUG
    puts("\tleaving plot_env_decr");
    #endif
}

/* initialise env arena
 *
 * returns on success
 * will cause a fatal error and exit on failure
 */
void plot_gc_env_init(void){
    if( ! plot_instance ){
        puts("plot_gc_env_init called without plot_instance being initialised");
        exit(1);
    }

    plot_instance->num_env_used = 0;
    #if GC_STATS
    plot_instance->num_env_reclaimed = 0;
    #endif
    plot_instance->num_env_allocated = 1000;
    plot_instance->env_arena = calloc( plot_instance->num_env_allocated, sizeof (struct plot_env) );
    if( ! plot_instance->env_arena ){
        puts("plot_gc_env_init ERROR: failed to calloc env arena");
        exit(1);
    }
}

/* increase reference count on plot_hash_entry */
void plot_he_incr(struct plot_hash_entry *he){
    plot_gc_incr( (struct plot_gc *) he);
}

/* decrease reference count on plot_hash_entry
 * may trigger collection of the he
 * (and the symbol and value stored within)
 */
void plot_he_decr(struct plot_hash_entry *he){
    if( !he )
        return;

    #if DEBUG
    printf("inside plot_he_decr considering object '%p' with refcount '%d'\n",
          (void*) he, he->gc.refcount);
    #endif

    if( he->gc.refcount < 0 ){
        #if DEBUG
        puts("\tthis object is not under GC control, leaving untouched");
        #endif
        /* object is NOT under control of gc, do not touch */
        return;
    }

    if( he->gc.refcount > 0 ){
        #if DEBUG
        puts("\tdecrementing refcount");
        #endif

        --he->gc.refcount;
        if( he->gc.refcount == 0 ){
            #if DEBUG
            puts("\treclaiming object");
            #endif
            #if GC_STATS
            ++plot_instance->num_he_reclaimed;
            #endif
            /* time to reclaim */
            //fprintf(stderr, "RECLAIMING\n"); // 2692537
            he->gc.next = (struct plot_gc *) plot_instance->he_reclaimed;
            plot_instance->he_reclaimed = he;
        #if DEBUG
        } else {
            puts("\tsparing object");
        #endif
        }
    } else {
        /* this object already has a refcount of 0, ERROR */
        puts("plot_he_decr: This object already has a refcount of 0, ERROR has occurred, terminating");
        exit(1);
    }

    #if DEBUG
    puts("\tleaving plot_he_decr");
    #endif
}


/* initialise hash entr arena
 *
 * return on success
 * will cause fatal error and exit on failure
 */
void plot_gc_he_init(void){
    if( ! plot_instance ){
        puts("plot_gc_he_init called without plot_instance being initialised");
        exit(1);
    }

    plot_instance->num_he_used = 0;
    #if GC_STATS
    plot_instance->num_he_reclaimed = 0;
    #endif
    plot_instance->num_he_allocated = 1000;
    plot_instance->he_arena = calloc( plot_instance->num_he_allocated, sizeof (struct plot_hash_entry) );
    if( ! plot_instance->he_arena ){
        puts("plot_gc_he_init ERROR: failed to calloc hv arena");
        exit(1);
    }
}

/* get new ref counted value */
struct plot_value * plot_new_value(void){
    struct plot_value *p;
    if( ! plot_instance ){
        puts("plot_value_new called without plot_instance being initialised");
        exit(1);
    }

    if( plot_instance->value_reclaimed ){
        #if GC_STATS
        ++plot_instance->num_value_recycled;
        #endif
        //fprintf(stderr, "reusing\n");
        p = plot_instance->value_reclaimed;
        /* gc is the first element of plot_value so this is safe */
        plot_instance->value_reclaimed = (plot_value *) p->gc.next;
        p->gc.refcount = 1;
        p->gc.next = 0;
        return p;
    }
    if( plot_instance->num_value_used >= plot_instance->num_value_allocated ){
        /* FIXME realloc */
        printf("THE BANK IS EMPTY; allocated all '%d' plot_value(s)\n", plot_instance->num_value_used);
        exit(1);
    } else {
        /* hand out resources */
        p = &(plot_instance->value_arena[ plot_instance->num_value_used ++]);
        p->gc.refcount = 1;
        p->gc.next = 0;
        return p;
    }
}

/* get new NON-ref counted value
 * this is needed for constants until
 * a better solution comes along
 */
struct plot_value * plot_new_constant(void){
    struct plot_value *p;
    p = plot_new_value();
    p->gc.refcount = -1; /* FIXME hack */
    return p;
}


/* get new env
 * parent is the enclosing environment, or 0
 */
struct plot_env * plot_new_env(struct plot_env *parent){
    struct plot_env *e;
    if( ! plot_instance ){
        puts("plot_value_new called without plot_instance being initialised");
        exit(1);
    }

    if( plot_instance->env_reclaimed ){
        #if GC_STATS
        ++plot_instance->num_env_recycled;
        #endif
        //fprintf(stderr, "reusing\n");
        e = plot_instance->env_reclaimed;
        /* gc is the first element of plot_env so this is safe */
        plot_instance->env_reclaimed = (plot_env *) e->gc.next;
        e->gc.refcount = 1;
        e->gc.next = 0;
        //printf("\tRECLAIMED: trying to set parent for '%p', parent is '%p'\n", (void*)e, (void*)parent);
        if( ! plot_env_init(e, parent) ){
            puts("plot_value_new: call to plot_env_init failed");
            exit(1);
        }
        return e;
    }
    if( plot_instance->num_env_used >= plot_instance->num_env_allocated ){
        /* FIXME realloc */
        printf("THE BANK IS EMPTY; allocated all '%d' plot_env(s)\n", plot_instance->num_env_used);
        exit(1);
    } else {
        /* hand out resources */
        e = &(plot_instance->env_arena[ plot_instance->num_env_used ++]);
        e->gc.refcount = 1;
        e->gc.next = 0;
        //printf("\tNEW: trying to set parent for '%p', parent is '%p'\n", (void*)e, (void*)parent);
        if( ! plot_env_init(e, parent) ){
            puts("plot_value_new: call to plot_env_init failed");
            exit(1);
        }
        return e;
    }

}

/* get new hash entry */
struct plot_hash_entry * plot_new_hash_entry(void){
    struct plot_hash_entry *he;
    if( ! plot_instance ){
        puts("plot_value_new called without plot_instance being initialised");
        exit(1);
    }

    if( plot_instance->he_reclaimed ){
        #if GC_STATS
        ++plot_instance->num_he_recycled;
        #endif
        he = plot_instance->he_reclaimed;
        /* gc is the first element of plot_hash_value so this is safe */
        plot_instance->he_reclaimed = (struct plot_hash_entry *) he->gc.next;
        he->gc.refcount = 1;
        he->gc.next = 0;
        return he;
    }
    if( plot_instance->num_he_used >= plot_instance->num_he_allocated ){
        /* FIXME realloc */
        printf("THE BANK IS EMPTY; allocated all '%d' plot_he(s)\n", plot_instance->num_he_used);
        exit(1);
    } else {
        /* hand out resources */
        he = &(plot_instance->he_arena[ plot_instance->num_he_used ++]);
        he->gc.next = 0;
        he->gc.refcount = 1;
        he->gc.next = 0;
        return he;
    }
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


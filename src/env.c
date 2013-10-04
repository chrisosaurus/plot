#include <stdlib.h> /* calloc, free */
#include <stdio.h> /* puts */

#include "value.h"
#include "hash.h"
#include "env.h"

#define DEBUG 0

/* allocated a new plot_env using calloc
 * *parent is pointer to parent env, or 0
 *
 * returns pointer to new env or 0 on error
 */
plot_env * plot_env_init(plot_env *parent){
    plot_env *e = calloc(1, sizeof *e);
    if( ! e )
        return 0;

    e->hash = plot_hash_init();
    if( ! e->hash )
        return 0;

    return e;
}

/* free env and calls hash cleanup */
void plot_env_cleanup(plot_env *env){
    if( ! env )
        return;

    plot_hash_cleanup(env->hash);
    free(env);
}

/* resolve a symbol to a value
 * searches in each hash 'up' the tree
 *
 * return plot_value* or 0 when symbol is not found
 */
const plot_value * plot_env_get(const plot_env *env, const plot_symbol * sym){
    const plot_env *e;
    const plot_value *v;

    if( ! sym )
        return 0;

    for( e=env; e; e=e->parent ){
        if( ! e->hash )
            break;
        v = plot_hash_get(e->hash, sym);
        if( v )
            return v;
    }

    return 0;
}

/* add a new symbol to this env
 * it can 'mask' a symbol from another env,
 * but it cannot already exist within this env
 *
 * returns 1 on success, 0 on error
 */
int plot_env_define(plot_env *env, const plot_symbol * sym, const plot_value * val){
    #if DEBUG
    puts("inside plot_env_define");
    #endif

    if( ! env || ! sym || ! val ){
        #if DEBUG
        puts("\tbad args");
        return 0;
        #endif
    }
    if( ! env->hash ){
        #if DEBUG
        puts("\tbad args");
        return 0;
        #endif
    }

    return plot_hash_insert(env->hash, sym, val);
}

/* set a a symbol to a value, it may or may not already exist within this env
 *
 * returns 1 on success, 0 on error
 */
int plot_env_set(plot_env *env, const plot_symbol * sym, const plot_value * val){
    if( ! env || ! sym || ! val )
        return 0;
    if( ! env->hash )
        return 0;

    return plot_hash_set(env->hash, sym, val);
}



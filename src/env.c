#include <stdio.h> /* puts */

#include "value.h"
#include "hash.h"
#include "env.h"
#include "plot.h"

#define DEBUG 0

/* initialise a previously allocated env
 *
 * env is the environment to initialise
 * parent is the enclosing env for this new env
 *  (can be 0)
 *
 *
 * returns 1 on success
 * 0 on error
 */
int plot_env_init(plot_env *env, plot_env *parent){
    if( ! env )
        return 0;

    env->parent = parent;

    env->hash = plot_hash_init();
    if( ! env->hash )
        return 0;

    return 1;
}

/* free env and calls hash cleanup */
void plot_env_cleanup(plot_env *env){
    if( ! env )
        return;

    plot_hash_cleanup(env->hash);
    /* FIXME need to deref */
}

/* resolve a symbol to a value
 * searches in each hash 'up' the tree
 *
 * return plot_value* or 0 when symbol is not found
 */
plot_value * plot_env_get(const plot_env *env, const plot_symbol * sym){
    const plot_env *e;
    plot_value *v;

    #if DEBUG
    puts("inside plot_env_get");
    #endif

    if( ! sym ){
        #if DEBUG
        puts("\tnull sym");
        #endif
        return 0;
    }

    for( e=env; e; e=e->parent ){
        if( ! e->hash )
            break;
        v = plot_hash_get(e->hash, sym);
        if( v ){
            plot_value_incr(v);
            return v;
        }
    }

    #if DEBUG
    puts("\tsym not found");
    #endif
    return 0;
}

/* add a new symbol to this env
 * it can 'mask' a symbol from another env,
 * but it cannot already exist within this env
 *
 * returns 1 on success, 0 on error
 */
int plot_env_define(plot_env *env, const plot_symbol * sym, plot_value * val){
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

    return plot_hash_set(env->hash, sym, val);
}


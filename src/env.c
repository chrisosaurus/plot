#include <stdio.h> /* puts */
#include <stdlib.h>

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

    if( env == parent ){
        printf("TRYING TO SET PARENT TO SELF '%p' and '%p'\n", (void*) env, (void*) parent);
        exit(1);
    }

    if( parent ){
        env->parent = parent;
        plot_env_incr(parent);
    }

    plot_hash_init(&(env->hash));

    return 1;
}

/* plot_env_cleanup will decr any parent
 * it will also call decr on all values stored within hash
 */
void plot_env_cleanup(plot_env *env){
    if( ! env )
        return;

    if( env->parent ){
        plot_env_decr(env->parent);
        env->parent = 0;
    }

    /* plot_hash_cleanup will call decr on all values
     * stored within
     *
     * FIXME currently the hash and each hash_entry
     * are allocated on the heap, so this will also free them.
     * Eventually they should also be gc-managed
     */
    plot_hash_cleanup(&(env->hash));
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
        if( ! &(e->hash) )
            break;
        v = plot_hash_get(&(e->hash), sym);
        if( v ){
            #if DEBUG
            puts("\tvalue found, returning");
            #endif
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

    return plot_hash_set(&(env->hash), sym, val);
}


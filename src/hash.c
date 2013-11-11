#include <stdio.h> /* puts */

#include "value.h"
#include "hash.h"
#include "plot.h"

#define DEBUG 0
#define DEBUG_CLEANUP 0

/* initialise a new hash
 * hashes are now part of plot_env
 *
 * requires a pointer to hash to initialise
 *
 * 1 is returned on success
 * or 0 if an error was encountered
 */
int plot_hash_init(plot_hash *hash){
    hash->n_elems = 0;
    hash->head = 0;
    return 1;
}

/* destroy hash
 * plot_hash_cleanup will call decr on all values
 * stored within
 *
 * FIXME currently the hash and each hash_entry
 * are allocated on the heap, so this will also free them.
 * Eventually they should also be gc-managed
 */
void plot_hash_cleanup(plot_hash *hash){
    plot_hash_entry *cur, *nxt;

    if( ! hash )
        return;

    #if DEBUG || DEBUG_CLEANUP
    puts("in plot_hash_cleanup");
    #endif

    /* FIXME currently hash and hash_entry are allocated
     * on the heap (via calloc), these should eventually be gc-ed
     */
    for( cur = hash->head; cur; cur = nxt ){
        nxt = cur->next;
        #if DEBUG || DEBUG_CLEANUP
        if( cur->value ){
            printf("\tdecreasing value '%p' with refcount '%d'\n", (void*) cur->value, cur->value->gc.refcount);
            printf("\tdecreasing hash_entry '%p' with refcount '%d'\n", (void*) cur, cur->gc.refcount);
        }
        #endif
        plot_value_decr(cur->value);
        plot_he_decr(cur);
    }

    #if DEBUG || DEBUG_CLEANUP
    puts("\tleaving plot_hash_cleanup");
    #endif
    /* FIXME need to deref */
}


/* get value stored at key within hash
 * return value for key or 0 if key was not found
 */
plot_value * plot_hash_get(const plot_hash *hash, plot_symbol * key){
    plot_hash_entry *e;

    #if DEBUG
    puts("inside plot_hash_get");
    #endif

    if( ! hash || ! key ){
        #if DEBUG
        puts("\tnull hash or key");
        #endif
        return 0; /* ERROR no hash or key specified */
    }

    plot_stats_hash_get();
    plot_hash_symbol(key);

    for( e = hash->head; e; e = e->next ){
        #if DEBUG
        printf("\tcomparing: looking at key '%s' (%llu), search string is '%s' (%llu)\n", e->key->val, e->key->hash, key->val, key->hash);
        #endif

        plot_stats_hash_comp();

        if( key->hash < e->key->hash )
            break;
        if( key->hash == e->key->hash ){
            plot_value_incr(e->value);
            #if DEBUG || DEBUG_CLEANUP
            if( e->value ){
                printf("\treturning found object '%s' ('%p') with refcount '%d'\n", e->key->val,(void*) e->value, e->value->gc.refcount);
            }
            #endif
            return e->value;
        }
    }

    #if DEBUG
    puts("\tno matching key found");
    #endif

    /* null was encountered
     * element not found
     */
    return 0;
}

/* set value to hash under key
 * keys must be unique within the hash
 * and keys CAN be overwritten once set (local mutation is allowed)
 *
 * this hash will NOT make copies of either key or value
 * therefore both key and value must not be freed until
 *  cleanup is called on this hash
 *
 * returns 1 on success, 0 on error
 */
int plot_hash_set(plot_hash *hash, plot_symbol * key, plot_value *value){
    plot_hash_entry **e, *n;
    int new=1; /* default value of 1, 0 is only used to mean we are re-defining a symbol */

    #if DEBUG
    puts("inside plot_hash_set");
    #endif

    if( ! hash ){
        #if DEBUG
        puts("\tnull hash provided");
        #endif
        return 0;
    }

    plot_hash_symbol(key);

    for( e=&hash->head; e && (*e); e = &(*e)->next ){
        #if DEBUG
        printf("\tcomparing: looking at key '%s' (%llu), search string is '%s' (%llu)\n", (*e)->key->val, (*e)->key->hash, key->val, key->hash);
        #endif
        /* stop iterating when we find an existing entry with a key 'after' us
         */
        if(  key->hash < (*e)->key->hash ) /* TRUE IFF key < (*e)->key */
            break;
        if( key->hash == (*e)->key->hash ){ /* TRUE IFF key == (*e)->key */
            new = 0; /* not new, don't increment counter */
            /* overriding, need to decr value here */
            plot_value_decr((*e)->value);
            break;
        }
    }

    /* if *e is null then we have either reached the end of the list
     * of we are the first element
     *
     * regardless, *e is our next and *e is where we store ourselves
     */
    n = plot_alloc_hash_entry();
    if( ! n )
        return 0;/* ERROR: calloc failed */
    n->key = key;
    n->value = value;

    #if DEBUG || DEBUG_CLEANUP
    if( key && value ){
        printf("\tincrementing value for '%s' ('%p'), refcount was '%d'\n", key->val, (void *) value, value->gc.refcount);
    }
    #endif

    plot_value_incr(value); /* we are holding a reference to value */
    n->next = *e;
    *e = n;

    if( new ) /* only increment if we are no re-defining */
        hash->n_elems++;

    #if DEBUG
    printf("\tsuccessfully added key '%s'\n", key->val);
    #endif

    return 1;
}


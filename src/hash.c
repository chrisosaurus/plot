#include <string.h> /* strcmp */
#include <stdio.h> /* puts */

#include "value.h"
#include "hash.h"
#include "plot.h"

#define DEBUG 0
#define DEBUG_CLEANUP 0

/* create a new hash
 * allocate a new hash using calloc
 *
 * a pointer to the new hash is returned
 * or 0 if an error was encountered
 */
plot_hash * plot_hash_init(void){
    plot_hash *hash;
    hash = plot_new_hash();
    if( ! hash )
        return 0; /* ERROR: calloc failed */
    return hash;
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
        }
        #endif
        plot_value_decr(cur->value);
        /* FIXME need to deref each hash_entry */
    }

    #if DEBUG || DEBUG_CLEANUP
    puts("\tleaving plot_hash_cleanup");
    #endif
    /* FIXME need to deref */
}


/* get value stored at key within hash
 * return value for key or 0 if key was not found
 */
plot_value * plot_hash_get(plot_hash *hash, const plot_symbol * key){
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

    for( e = hash->head; e; e = e->next ){
        #if DEBUG
        printf("\tcomparing: looking at key '%s', search string is '%s'\n", e->key->val, key->val);
        #endif
        if( ! strcmp(key->val, e->key->val) ){
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
int plot_hash_set(plot_hash *hash, const plot_symbol * key, plot_value *value){
    plot_hash_entry **e, *n;
    int sc=1; /* default value of 1, 0 is only used to mean we are re-defining a symbol */

    #if DEBUG
    puts("inside plot_hash_set");
    #endif

    if( ! hash ){
        #if DEBUG
        puts("\tnull hash provided");
        #endif
        return 0;
    }

    for( e=&hash->head; e && (*e); e = &(*e)->next ){
        sc = strcmp(key->val, (*e)->key->val);
        /* stop iterating when we find an existing entry with a key 'after' us
         */
        if(  sc < 0 ) /* TRUE IFF key < (*e)->key */
            break;
        if( sc == 0 ){ /* TRUE IFF key == (*e)->key */
            /* FIXME overriding, need to decr value here */
            puts("OVERRIDING - FIXME need to decr");
            break;
        }
    }

    /* if *e is null then we have either reached the end of the list
     * of we are the first element
     *
     * regardless, *e is our next and *e is where we store ourselves
     */
    n = plot_new_hash_entry();
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

    if( sc ) /* only increment if we are no re-defining */
        hash->n_elems++;

    #if DEBUG
    printf("\tsuccessfully added key '%s'\n", key->val);
    #endif

    return 1;
}


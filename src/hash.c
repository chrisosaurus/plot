#include <stdio.h> /* puts */

#include "value.h"
#include "hash.h"
#include "plot.h"

#define DEBUG 0
#define DEBUG_CLEANUP 0
#define PL_HASH_DEFAULT_SIZE 32

/* initialise a new hash
 * hashes are now part of plot_env
 *
 * requires a pointer to hash to initialise
 *
 * 1 is returned on success
 * or 0 if an error was encountered
 */
int plot_hash_init(plot_hash *hash){
    return lh_init(&(hash->lht), PL_HASH_DEFAULT_SIZE);
}

/* destroy hash
 * plot_hash_cleanup will call decr on all values
 * stored within
 *
 * FIXME currently the hash
 * is allocated on the heap, so this will also free them.
 * Eventually they should also be gc-managed
 */
void plot_hash_cleanup(plot_hash *hash){
    if( ! hash )
        return;

    #if DEBUG || DEBUG_CLEANUP
    puts("in plot_hash_cleanup");
    #endif

    /* FIXME need to deal with decr-ing destroyed values
     * this requires exposing some new functionality in linear_hash
     */

    /* lh_destroy(*table, free_table, free_data); */
    if( lh_destroy(&(hash->lht), 1, 0) ){
        /* FIXME success */
    } else {
        /* FIXME failure */
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
    plot_value *result = 0;

    #if DEBUG
    puts("inside plot_hash_get");
    #endif

    if( ! hash || ! key ){
        #if DEBUG
        puts("\tnull hash or key");
        #endif
        return 0; /* ERROR no hash or key specified */
    }

    #if HASH_STATS
    plot_stats_hash_get();
    #endif

    result =  lh_get(&(hash->lht), key->val);

    #if DEBUG
    printf("\tLooking for key %s:", key->val);
    if( ! result ){
        puts(" no matching key found");
    } else {
        puts(" found");
    }
    #endif

    if( result ) {
        /* have to incr before returning */
        plot_value_incr(result);
    }

    return result;
}

/* set value to hash under key
 * keys must be unique within the hash
 * and keys CAN be overwritten once set (local mutation is allowed)
 *
 * the hash will make a strdup copy of key to use internally
 * but this hash will NOT make copies of the value
 * therefore both the value must not be freed until
 *  cleanup is called on this hash
 *
 * returns 1 on success, 0 on error
 */
int plot_hash_set(plot_hash *hash, plot_symbol * key, plot_value *value){
    void *old = 0;

    #if DEBUG
    puts("inside plot_hash_set");
    #endif

    if( ! hash ){
        #if DEBUG
        puts("\tnull hash provided");
        #endif
        return 0;
    }

    /* plot_hash_set is either an lh_insert or an lh_set */
    if( lh_exists(&(hash->lht), key->val) ){
        old = lh_set(&(hash->lht), key->val, value);
        if( old ){
            /* decr old value stored */
            plot_value_decr(old);
        }
    } else {
        if( ! lh_insert(&(hash->lht), key->val, value) ){
            /* FIXME error on lh_insert */
            printf("\tplot_hash_set: insert failed for key '%s'\n", key->val);
            return 0;
        }
    }

    #if DEBUG || DEBUG_CLEANUP
    if( key && value ){
        printf("\tincrementing value for '%s' ('%p'), refcount was '%d'\n", key->val, (void *) value, value->gc.refcount);
    }
    #endif

    /* key is strndup-ed so no need to incr */

    plot_value_incr(value); /* we are holding a reference to value */

    #if DEBUG
    printf("\tsuccessfully added key '%s'\n", key->val);
    #endif

    return 1;
}

/* returns numbers of elements stored in hash
 * returns 0 on error
 */
unsigned int plot_hash_nelems(plot_hash *hash){
    #if DEBUG
    puts("inside plot_hash_set");
    #endif

    if( ! hash ){
        #if DEBUG
        puts("\tnull hash provided");
        #endif
        return 0;
    }

    return lh_nelems(&(hash->lht));
}



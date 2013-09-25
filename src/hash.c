#include <string.h> /* strcmp */
#include <stdlib.h> /* calloc, free */

#include "hash.h"

/* create a new hash
 * if *hash is 0 then a new plot_hash will be allocated using calloc
 * if *hash is non-0 then it will be used as the location of the hash
 *
 * a pointer to the hash is returned
 * or 0 if an error was encountered
 */
plot_hash * plot_hash_init(plot_hash *hash){
    if( ! hash ){
        hash = calloc(1, sizeof(*hash));
        if( ! hash )
            return 0; /* ERROR: calloc failed */
    }
    return hash;
}

/* destroy hash
 * frees all plot_hash_entry(s) and then finally the plot_hash
 */
void plot_hash_cleanup(plot_hash *hash){
    plot_hash_entry *cur, *nxt;

    if( ! hash )
        return;

    for( cur = hash->head; cur; cur = nxt ){
        nxt = cur->next;
        free(cur);
    }

    free(hash);
}


/* return value for key or 0 if key was not found */
/* FIXME define ownership of key */
plot_value * plot_hash_get(plot_hash *hash, const char * const key){
    plot_hash_entry *e;

    if( ! hash || ! key )
        return 0; /* ERROR no hash or key specified */

    for( e = hash->head; e; e = e->next ){
        if( ! strcmp(key, e->key) )
            return e->value;
    }

    /* null was encountered
     * element not found
     */
    return 0;
}

/* FIXME we may want a way of getting old value at key */
/* set value to key, return 1 on success and 0 on error */
/* FIXME define ownership of key and value */
int plot_hash_insert(plot_hash *hash, const char * const key, plot_value *value){
    plot_hash_entry **e, *n;

    if( ! hash )
        return 0;

    for( e=&hash->head; e && (*e); e = &(*e)->next ){
        /* stop iterating when we find an existing entry with a key 'after' us
         */
        if( strcmp(key, (*e)->key) < 0 ) /* TRUE IFF key < (*e)->key */
            break;
    }

    /* if *e is null then we have either reached the end of the list
     * of we are the first element
     *
     * regardless, *e is our next and *e is where we store ourselves
     */
    n = calloc(1, sizeof(*n));
    if( ! n )
        return 0;/* ERROR: calloc failed */
    n->key = key;
    n->value = value;
    n->next = *e;
    *e = n;

    hash->n_elems++;
    return 1;
}



#include <string.h> /* strcmp */
#include <stdlib.h> /* calloc, free */

#include "hash.h"

/* create a new hash
 * allocate a new hash using calloc
 *
 * a pointer to the new hash is returned
 * or 0 if an error was encountered
 */
plot_hash * plot_hash_init(){
    plot_hash *hash;
    hash = calloc(1, sizeof(*hash));
    if( ! hash )
        return 0; /* ERROR: calloc failed */
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


/* get value stored at key within hash
 * return value for key or 0 if key was not found
 */
plot_value * plot_hash_get(plot_hash *hash, plot_symbol * const key){
    plot_hash_entry *e;

    if( ! hash || ! key )
        return 0; /* ERROR no hash or key specified */

    for( e = hash->head; e; e = e->next ){
        if( ! strcmp(key->val, e->key->val) )
            return e->value;
    }

    /* null was encountered
     * element not found
     */
    return 0;
}

/* set value to hash under key
 * keys must be unique within the hash
 * and keys cannot be overwritten once set (no mutation)
 *
 * this hash will NOT make copies of either key or value
 * therefore both key and value must not be freed until
 *  cleanup is called on this hash
 *
 * returns 1 on success, 0 on error
 */
int plot_hash_insert(plot_hash *hash, plot_symbol * const key, plot_value *value){
    plot_hash_entry **e, *n;
    int sc=0;

    if( ! hash )
        return 0;

    for( e=&hash->head; e && (*e); e = &(*e)->next ){
        sc = strcmp(key->val, (*e)->key->val);
        /* stop iterating when we find an existing entry with a key 'after' us
         */
        if(  sc < 0 ) /* TRUE IFF key < (*e)->key */
            break;
        if( sc == 0 ) /* TRUE IFF key == (*e)->key */
            return 0; /* ERROR cannot overwrite key */
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

/* set value in hash under key
 * keys must be unique within the hash
 * will overwrite any existing value
 *
 * returns 1 on success, 0 on error
 */
int plot_hash_set(plot_hash *hash, plot_symbol * const key, plot_value *value){
    /* FIXME TODO implement... without duplication... */
    return 0;
}



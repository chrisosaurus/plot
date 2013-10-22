#ifndef PLOT_HASH_H
#define PLOT_HASH_H

#include "plot.h" /* needed for plot_gc */

struct plot_symbol;
struct plot_value;

/* a plot_hash is a singly linked-list of plot_hash_entry (s)
 * and a count of the number of elements
 *
 * plot_hash_entry(s) are stored as a linked-list
 * in strcmp order of keys
 */
typedef struct plot_hash {
    struct plot_hash_entry *head;
    int n_elems;
} plot_hash;

/* a plot_hash_entry is an entry inside a plot_hash
 * consisting of a key and value pair
 * and a pointer to the next item in the list
 * the last element will have null as it's next value
 *
 * plot_hash_entry(s) are stored as a linked-list
 * in strcmp order of keys
 */
typedef struct plot_hash_entry {
    struct plot_gc gc;
    const struct plot_symbol * key;
    struct plot_value *value;
    struct plot_hash_entry *next;
} plot_hash_entry;

/* initialise a new hash
 * hashes are now part of plot_env
 *
 * requires a pointer to hash to initialise
 *
 * 1 is returned on success
 * or 0 if an error was encountered
 */
int plot_hash_init(plot_hash *hash);

/* destroy hash
 * plot_hash_cleanup will call decr on all values
 * stored within
 *
 * FIXME currently the hash and each hash_entry
 * are allocated on the heap, so this will also free them.
 * Eventually they should also be gc-managed
 */
void plot_hash_cleanup(plot_hash *hash);

/* get value stored at key within hash
 * return value for key or 0 if key was not found
 */
struct plot_value * plot_hash_get(const plot_hash *hash, const struct plot_symbol * key);

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
int plot_hash_set(plot_hash *hash, const struct plot_symbol * key, struct plot_value *value);

#endif


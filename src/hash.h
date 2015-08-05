#ifndef PLOT_HASH_H
#define PLOT_HASH_H

#include "plot.h" /* needed for plot_gc */
#include "../libs/linear_hash/linear_hash.h"

struct plot_symbol;
struct plot_value;

/* a plot_hash is a wrapper around linear_hash
 * each key is a string
 * each value is a void* which refers to a plot_value
 * and a count of the number of elements
 */
typedef struct plot_hash {
    struct lh_table lht;
} plot_hash;

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
 * FIXME currently the hash
 * is allocated on the heap, so this will also free them.
 * Eventually they should also be gc-managed
 */
void plot_hash_cleanup(plot_hash *hash);

/* get value stored at key within hash
 * return value for key or 0 if key was not found
 */
struct plot_value * plot_hash_get(const plot_hash *hash, struct plot_symbol * key);

/* set value to hash under key
 * keys must be unique within the hash
 * and keys CAN be overwritten once set (local mutation is allowed)
 *
 * the hash will make a strdup copy of key to use internally
 * but this hash will NOT make copies of the value
 * therefore both the value must not be freed until
 *  cleanup is called on this hash
 *
 *
 * returns 1 on success, 0 on error
 */
int plot_hash_set(plot_hash *hash, struct plot_symbol * key, struct plot_value *value);

/* returns numbers of elements stored in hash
 * returns 0 on error
 */
unsigned int plot_hash_nelems(plot_hash *hash);

#endif


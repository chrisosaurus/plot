#ifndef PLOT_HASH_H
#define PLOT_HASH_H

#include "value.h"

typedef struct plot_hash {
    char *key;
    int key_len;
    plot_value *value;
    struct plot_hash *next;
} plot_hash;

/* new */
plot_hash * plot_hash_init(plot_hash *hash);

/* destroy */
void plot_hash_cleanup(plot_hash *hash);


/* return value for key or 0 if key was not found */
plot_value * plot_hash_get(plot_hash *hash, const char * const key);

/* FIXME we may want a way of getting old value at key */
/* set value to key, return 1 on success and 0 on error */
int plot_hash_insert(plot_hash *hash, const char * const key, plot_value *value);

#endif

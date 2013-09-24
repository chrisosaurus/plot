#include "hash.h"

/* new */
plot_hash * plot_hash_init(plot_hash *hash){
    return 0;
}

/* destroy */
void plot_hash_cleanup(plot_hash *hash){
}


/* return value for key or 0 if key was not found */
plot_value * plot_hash_get(plot_hash *hash, const char * const key){
    return 0;
}

/* FIXME we may want a way of getting old value at key */
/* set value to key, return 1 on success and 0 on error */
int plot_hash_insert(plot_hash *hash, const char * const key, plot_value *value){
    return 0;
}



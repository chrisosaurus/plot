#ifndef PLOT_H
#define PLOT_H

/**** functions for dealing with plot's internal state ******/

/* plot maintains internal state
 * plot_init sets up internal state and must be called
 * before any other methods
 *
 void* returns 1 on success or 0 on failure
 */
int plot_init(void);

struct plot_env;
/* return root env
 * root env represents 'global' scope
 */
struct plot_env * plot_get_env(void);

/* plot_cleanup cleans up internal state
 * must be last method called
 */
void plot_cleanup(void);

struct plot_value;
/* print error information and then exit
 */
void plot_handle_error(const struct plot_value *error);

/******* public garbage collection interface ******/

/* struct that must be first element within all garbage
 * collected objects
 */
struct plot_gc {
    /* number of references
     * > 0 means we keep it around
     * == 0 means we collect
     * < = means this is NOT managed
     */
    int refcount;
};

/* increase reference count on object */
void plot_incr(struct plot_gc *p);

/* decrease reference count on object
 * may trigger collection
 */
void plot_decr(struct plot_gc *p);

/* get new value */
struct plot_value * plot_new_value(void);

/* get new env */
struct plot_env * plot_new_env(void);

/* get new hash */
struct plot_hash * plot_new_hash(void);

/* get new hash entry */
struct plot_hash_entry * plot_new_hash_entry(void);

/* get new string */
char * plot_new_string(int len);

#endif

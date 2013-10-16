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


#endif

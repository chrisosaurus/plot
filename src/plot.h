#ifndef PLOT_H
#define PLOT_H

#define HASH_STATS 1

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
struct plot_env * plot_get_global_env(void);

/* plot_cleanup cleans up internal state
 * must be last method called
 */
void plot_cleanup(void);

struct plot_value;

/* generates an appropriate error value, prints it, and then returns it
 * caller is expected to return the result
 *
 * each frame within the callstack is expected to detect this,
 * print relevant trace information, and then return the value further
 */
struct plot_value * plot_runtime_error(enum plot_error_type type, const char *msg, const char *location);

/* a fatal error will print the supplied string and then `exit(1)`
 */
void plot_fatal_error(const char *str);

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

    /* pointer to next collected object (for reallocation) */
    struct plot_gc *next;
};

/* increase reference count on plot_value */
void plot_value_incr(struct plot_value *p);

/* decrease reference count on plot_value
 * may trigger collection
 */
void plot_value_decr(struct plot_value *p);

/* allocate new ref counted value */
struct plot_value * plot_alloc_value(void);

/* allocate new NON-ref counted value
 * this is needed for constants until
 * a better solution comes along
 */
struct plot_value * plot_alloc_constant(void);

/* increase reference count on plot_env */
void plot_env_incr(struct plot_env *e);

/* decrease reference count on plot_env
 * may trigger collection of the env,
 * it's parents and it's stored values
 */
void plot_env_decr(struct plot_env *e);

/* allocate new env
 * parent is the enclosing environment, or 0
 */
struct plot_env * plot_alloc_env(struct plot_env *parent);

struct plot_hash_entry;

/* increase reference count on plot_entry */
void plot_he_incr(struct plot_hash_entry *he);

/* decrease reference count on plot_entry
 * may trigger collection of the entry
 * (and the symbol and value stored within)
 */
void plot_he_decr(struct plot_hash_entry *he);

/* allocate new hash entry */
struct plot_hash_entry * plot_alloc_hash_entry(void);

/* allocate new string */
char * plot_alloc_string(int len);

struct plot_value * plot_get_unspecified_constant(void);

void plot_stats_hash_get(void);
void plot_stats_hash_comp(void);
void plot_stats_env_get(void);
void plot_stats_env_loop(void);

#endif

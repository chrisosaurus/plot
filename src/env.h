#ifndef PLOT_ENV_H
#define PLOT_ENV_H

struct plot_hash;
struct plot_symbol;
struct plot_value;

typedef struct plot_env {
    struct plot_env *parent;
    struct plot_hash *hash;
} plot_env;

/* allocated a new plot_env using calloc
 * *parent is pointer to parent env, or 0
 *
 * returns pointer to new env or 0 on error
 */
plot_env * plot_env_init(plot_env *parent);

/* free env and calls hash cleanup */
void plot_env_cleanup(plot_env *env);

/* resolve a symbol to a value
 * searches in each hash 'up' the tree
 *
 * return plot_value* or 0 when symbol is not found
 */
const struct plot_value * plot_env_get(const plot_env *env, const struct plot_symbol * sym);

/* add a new symbol to this env
 * it can 'mask' a symbol from another env,
 * but it cannot already exist within this env
 *
 * returns 1 on success, 0 on error
 */
int plot_env_define(plot_env *env, const struct plot_symbol * sym, const struct plot_value * val);

/* set a a symbol to a value, it may or may not already exist within this env
 *
 * returns 1 on success, 0 on error
 */
int plot_env_set(plot_env *env, const struct plot_symbol * sym, const struct plot_value * val);

#endif

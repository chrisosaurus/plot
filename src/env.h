#ifndef PLOT_ENV_H
#define PLOT_ENV_H

#include "plot.h" /* needed for plot_gc */

struct plot_hash;
struct plot_symbol;
struct plot_value;

typedef struct plot_env {
    struct plot_gc gc;
    struct plot_env *parent;
    struct plot_hash *hash;
} plot_env;

/* initialise a previously allocated env
 * returns 1 on success
 * 0 on error
 */
int plot_env_init(plot_env *env, plot_env *parent);

/* free env and calls hash cleanup */
void plot_env_cleanup(plot_env *env);

/* resolve a symbol to a value
 * searches in each hash 'up' the tree
 *
 * return plot_value* or 0 when symbol is not found
 */
struct plot_value * plot_env_get(const plot_env *env, const struct plot_symbol * sym);

/* add a new symbol to this env
 * it can 'mask' a symbol from another env,
 * but it cannot already exist within this env
 *
 * returns 1 on success, 0 on error
 */
int plot_env_define(plot_env *env, const struct plot_symbol * sym, struct plot_value * val);

#endif

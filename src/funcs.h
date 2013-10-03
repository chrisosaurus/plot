#ifndef PLOT_FUNCS_H
#define PLOT_FUNCS_H

struct plot_env;
struct plot_value;
struct plot_expr;

const struct plot_value * plot_func_add(struct plot_env *env, const struct plot_expr *args, int argc);

#endif

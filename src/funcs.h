#ifndef PLOT_FUNCS_H
#define PLOT_FUNCS_H

struct plot_env;
struct plot_value;

const struct plot_value * plot_func_add(const struct plot_env *env, const struct plot_value *v1, const struct plot_value *v2);

#endif

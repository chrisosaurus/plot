#ifndef PLOT_EVAL_H
#define PLOT_EVAL_H

#include "value.h"
#include "hash.h"
#include "types.h"
#include "env.h"

plot_value * plot_eval_value(plot_env *env, plot_expr *expr);

plot_value * plot_eval_sexpr(plot_env *env, plot_expr *expr);

/* evals an expr in an environment */
plot_value * plot_eval(plot_env *env, plot_expr *expr);

#endif

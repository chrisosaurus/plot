#ifndef PLOT_EVAL_H
#define PLOT_EVAL_H

#include "value.h"
#include "hash.h"
#include "types.h"
#include "env.h"

/* if symbol then resolve in env, otherwise return value */
plot_value * plot_eval_value(plot_env *env, plot_value *value);

plot_value * plot_eval_sexpr(plot_env *env, plot_sexpr *sexpr);

plot_value * plot_eval_form(plot_env *env, plot_sexpr *sexpr);

plot_value * plot_eval_func_call(plot_env *env, plot_sexpr *sexpr);

/* evals an expr in an environment */
plot_value * plot_eval(plot_env *env, plot_expr *expr);

#endif

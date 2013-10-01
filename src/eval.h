#ifndef PLOT_EVAL_H
#define PLOT_EVAL_H

#include "value.h"
#include "types.h"
#include "env.h"

/* evals an expr in an environment */
plot_value * plot_eval_test(plot_env *env, plot_expr *expr);

#endif

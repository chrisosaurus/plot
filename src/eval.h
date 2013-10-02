#ifndef PLOT_EVAL_H
#define PLOT_EVAL_H

#include "value.h"
#include "hash.h"
#include "types.h"
#include "env.h"

/* if symbol then resolve in env, otherwise return value
 * cannot modify the env
 *
 * returned value cannot be freed, it is either the same value you passed in OR
 *  the value stored under that symbol in the env.
 */
const plot_value * plot_eval_value(const plot_env * env, const plot_value * value);

/* eval a sexpr in an environment
 * if sexpr is a form then plot_eval_form may be called which can
 *  modify the env
 *
 * TODO FIXME
 * returned value may or may not have been calloced (by plot_func_add), a complete mess...
 */
const plot_value * plot_eval_sexpr(plot_env *env, const plot_sexpr * sexpr);

/* eval a form in an environment
 * can modify the environment (e.g. define)
 *
 * TODO FIXME
 */
const plot_value * plot_eval_form(plot_env *env, const plot_sexpr * sexpr);

/* eval a function call in an environment
 *
 * TODO FIXME
 */
const plot_value * plot_eval_func_call(const plot_env * env, const plot_sexpr * sexpr);

/* evals an expr in an environment
 *
 * TODO FIXME
 */
const plot_value * plot_eval(plot_env * env, const plot_expr * expr);

#endif

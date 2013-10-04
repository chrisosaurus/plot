#ifndef PLOT_EVAL_H
#define PLOT_EVAL_H

struct plot_value;
struct plot_expr;
struct plot_sexpr;
struct plot_env;

/* if symbol then resolve in env, otherwise return value
 * cannot modify the env
 *
 * returned value cannot be freed, it is either the same value you passed in OR
 *  the value stored under that symbol in the env.
 */
const struct plot_value * plot_eval_value(struct plot_env * env, const struct plot_value * value);

/* eval a sexpr in an environment
 * if sexpr is a form then plot_eval_form may be called which can
 *  modify the env
 *
 * TODO FIXME
 * returned value may or may not have been calloced (by plot_func_add), a complete mess...
 */
const struct plot_value * plot_eval_sexpr(struct plot_env *env, const struct plot_sexpr * sexpr);

/* eval a form in an environment
 * can modify the environment (e.g. define)
 *
 * TODO FIXME
 */
const struct plot_value * plot_eval_form(struct plot_env *env, const struct plot_sexpr * sexpr);

/* eval a function call in an environment
 *
 * TODO FIXME
 */
const struct plot_value * plot_eval_func_call(struct plot_env * env, const struct plot_sexpr * sexpr);

/* evals an expr in an environment
 *
 * TODO FIXME
 */
const struct plot_value * plot_eval(struct plot_env * env, const struct plot_expr * expr);

#endif

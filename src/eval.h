#ifndef PLOT_EVAL_H
#define PLOT_EVAL_H

struct plot_value;
struct plot_program;
struct plot_expr;
struct plot_sexpr;
struct plot_env;

/* if symbol then resolve in env, otherwise return value
 * cannot modify the env
 *
 * returned value cannot be freed, it is either the same value you passed in OR
 *  the value stored under that symbol in the env.
 */
struct plot_value * plot_eval_value(struct plot_env * env, struct plot_value * value);

/* eval a sexpr in an environment
 * if sexpr is a form then plot_eval_form may be called which can
 *  modify the env
 *
 * takes a plot list as argument
 *
 * TODO FIXME
 * returned value may or may not have been calloced (by plot_func_add), a complete mess...
 */
struct plot_value * plot_eval_sexpr(struct plot_env *env, struct plot_value * sexpr);

/* eval a form in an environment
 * can modify the environment (e.g. define)
 *
 * takes a plot list as argument
 *
 * wrapper around `plot_eval_apply`
 * MAY evaluate arguments depending on form
 * userspace should generally call `plot_eval_apply` or `plot_func_control_apply`
 *
 * TODO FIXME
 * FIXME migrate to plot_eval_apply
 */
struct plot_value * plot_eval_form(struct plot_env *env, struct plot_value * sexpr);

/* performs a call to a function
 * can modify the environment
 * will not evaluate arguments in anyway, passes them directly to the func
 *
 * plot_eval_apply(env, func, args)
 *      func must be one of:
 *          plot_type_lambda
 *          plot_type_form
 *          plot_type_symbol - that resolves to either of the above in env
 *      args is a plot_type_pair which is the first item in a plot list of arguments
 *          arguments are NOT evaluated, so plot_eval_apply IS SAFE to call from userspace c
 *
 * both `plot_eval_form` and `plot_func_control_apply` are wrappers for this
 * safe to call from userspace c
 * plot programs instead call `plot_func_control_apply` via `(apply ...)`
 *
 * TODO FIXME
 */
struct plot_value * plot_eval_apply(struct plot_env *env, struct plot_value *func, struct plot_value *args);

/* evals an expr in an environment
 *
 * takes a plot list as argument
 *
 * TODO FIXME
 */
struct plot_value * plot_eval_expr(struct plot_env * env, struct plot_value * expr);


/* evals a program in an environment
 * returns 1 on success, 0 on error
 *
 * takes a plot list as argument
 *
 * TODO FIXME
 */
int plot_eval(struct plot_env * env, struct plot_value * prog);

#endif

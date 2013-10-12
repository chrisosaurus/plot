#ifndef PLOT_FUNCS_H
#define PLOT_FUNCS_H

struct plot_env;
struct plot_value;
struct plot_expr;

/* print error information and then exit
 */
void plot_handle_error(const plot_value *error);

/* print value to stdout
 */
const struct plot_value * plot_func_display(struct plot_env *env, const struct plot_expr *args, int argc);

/* print a newline to stdout
 */
const struct plot_value * plot_func_newline(struct plot_env *env, const struct plot_expr *args, int argc);

/***** mathmatical functions ******/

/* takes a list of expressions
 * evals each and then adds the expression's value if it is a number
 * if any of the expressions evaluate to something other than an error
 * throw plot_error_bad_args
 */
const struct plot_value * plot_func_add(struct plot_env *env, const struct plot_expr *args, int argc);

/* takes a list of expressions
 * evals each and then subtracts the expression's value if it is a number
 * if any of the expressions evaluate to something other than an error
 * throw plot_error_bad_args
 */
const struct plot_value * plot_func_subtract(struct plot_env *env, const struct plot_expr *args, int argc);

/* takes a list of expressions
 * evals each and then multiplies the expression's value if it is a number
 * if any of the expressions evaluate to something other than an error
 * throw plot_error_bad_args
 */
const struct plot_value * plot_func_multiply(struct plot_env *env, const struct plot_expr *args, int argc);

/* integer division
 * exact only
 */
const struct plot_value * plot_func_divide(struct plot_env *env, const struct plot_expr *args, int argc);

/* remainder
 */
const struct plot_value * plot_func_remainder(struct plot_env *env, const struct plot_expr *args, int argc);

/******** comparison functions *******/

/* mathmatical =
 */
const struct plot_value * plot_func_equal(struct plot_env *env, const struct plot_expr *args, int argc);

/* <
 */
const struct plot_value * plot_func_less_than(struct plot_env *env, const struct plot_expr *args, int argc);

/* >
 */
const struct plot_value * plot_func_greater_than(struct plot_env *env, const struct plot_expr *args, int argc);

/* <=
 */
const struct plot_value * plot_func_less_than_or_equal(struct plot_env *env, const struct plot_expr *args, int argc);

/* >=
 */
const struct plot_value * plot_func_greater_than_or_equal(struct plot_env *env, const struct plot_expr *args, int argc);

#endif

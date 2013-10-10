#ifndef PLOT_FUNCS_H
#define PLOT_FUNCS_H

struct plot_env;
struct plot_value;
struct plot_expr;

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

/* print error information and then exit
 */
void plot_handle_error(const plot_value *error);

/* print value to stdout
 */
const struct plot_value * plot_func_display(struct plot_env *env, const struct plot_expr *args, int argc);

/* print a newline to stdout
 */
const struct plot_value * plot_func_newline(struct plot_env *env, const struct plot_expr *args, int argc);

#endif

#ifndef PLOT_FUNCS_H
#define PLOT_FUNCS_H

struct plot_env;
struct plot_value;
struct plot_expr;

/* print value to stdout
 */
struct plot_value * plot_func_display(struct plot_env *env, struct plot_expr *args, int argc);

/* print a newline to stdout
 */
struct plot_value * plot_func_newline(struct plot_env *env, struct plot_expr *args, int argc);

/***** mathmatical functions ******/

/* takes a list of expressions
 * evals each and then adds the expression's value if it is a number
 * if any of the expressions evaluate to something other than an error
 * throw plot_error_bad_args
 */
struct plot_value * plot_func_add(struct plot_env *env, struct plot_expr *args, int argc);

/* takes a list of expressions
 * evals each and then subtracts the expression's value if it is a number
 * if any of the expressions evaluate to something other than an error
 * throw plot_error_bad_args
 */
struct plot_value * plot_func_subtract(struct plot_env *env, struct plot_expr *args, int argc);

/* takes a list of expressions
 * evals each and then multiplies the expression's value if it is a number
 * if any of the expressions evaluate to something other than an error
 * throw plot_error_bad_args
 */
struct plot_value * plot_func_multiply(struct plot_env *env, struct plot_expr *args, int argc);

/* integer division
 * exact only
 */
struct plot_value * plot_func_divide(struct plot_env *env, struct plot_expr *args, int argc);

/* remainder
 */
struct plot_value * plot_func_remainder(struct plot_env *env, struct plot_expr *args, int argc);

/******** comparison functions *******/

/* mathmatical =
 */
struct plot_value * plot_func_equal(struct plot_env *env, struct plot_expr *args, int argc);

/* <
 */
struct plot_value * plot_func_less(struct plot_env *env, struct plot_expr *args, int argc);

/* >
 */
struct plot_value * plot_func_greater(struct plot_env *env, struct plot_expr *args, int argc);

/* <=
 */
struct plot_value * plot_func_less_equal(struct plot_env *env, struct plot_expr *args, int argc);

/* >=
 */
struct plot_value * plot_func_greater_equal(struct plot_env *env, struct plot_expr *args, int argc);

/********* value testing functions ********/

/* boolean?
 */
struct plot_value * plot_func_boolean_test(struct plot_env *env, struct plot_expr *args, int argc);

/* symbol?
 */
struct plot_value * plot_func_symbol_test(struct plot_env *env, struct plot_expr *args, int argc);

/* string?
 */
struct plot_value * plot_func_string_test(struct plot_env *env, struct plot_expr *args, int argc);

/* number?
 */
struct plot_value * plot_func_number_test(struct plot_env *env, struct plot_expr *args, int argc);

/* function?
 */
struct plot_value * plot_func_procedure_test(struct plot_env *env, struct plot_expr *args, int argc);

#endif

#ifndef PLOT_NUMBER_H
#define PLOT_NUMBER_H

struct plot_env;
struct plot_value;

/***** mathmatical functions ******/

/* number?
 */
struct plot_value * plot_func_number_test(struct plot_env *env, struct plot_value **args, int argc);

/* takes a list of expressions
 * evals each and then adds the expression's value if it is a number
 * if any of the expressions evaluate to something other than an error
 * throw plot_error_bad_args
 */
struct plot_value * plot_func_add(struct plot_env *env, struct plot_value **args, int argc);

/* takes a list of expressions
 * evals each and then subtracts the expression's value if it is a number
 * if any of the expressions evaluate to something other than an error
 * throw plot_error_bad_args
 */
struct plot_value * plot_func_subtract(struct plot_env *env, struct plot_value **args, int argc);

/* takes a list of expressions
 * evals each and then multiplies the expression's value if it is a number
 * if any of the expressions evaluate to something other than an error
 * throw plot_error_bad_args
 */
struct plot_value * plot_func_multiply(struct plot_env *env, struct plot_value **args, int argc);

/* integer division
 * exact only
 */
struct plot_value * plot_func_divide(struct plot_env *env, struct plot_value **args, int argc);

/* remainder
 */
struct plot_value * plot_func_remainder(struct plot_env *env, struct plot_value **args, int argc);


/******** comparison functions *******/

/* mathmatical =
 */
struct plot_value * plot_func_math_equal(struct plot_env *env, struct plot_value **args, int argc);

/* <
 */
struct plot_value * plot_func_less(struct plot_env *env, struct plot_value **args, int argc);

/* >
 */
struct plot_value * plot_func_greater(struct plot_env *env, struct plot_value **args, int argc);

/* <=
 */
struct plot_value * plot_func_less_equal(struct plot_env *env, struct plot_value **args, int argc);

/* >=
 */
struct plot_value * plot_func_greater_equal(struct plot_env *env, struct plot_value **args, int argc);


#endif

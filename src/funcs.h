#ifndef PLOT_FUNCS_H
#define PLOT_FUNCS_H

struct plot_env;
struct plot_value;
struct plot_expr;

/* print value to stdout
 */
struct plot_value * plot_func_display(struct plot_env *env, struct plot_value **args, int argc);

/* print a newline to stdout
 */
struct plot_value * plot_func_newline(struct plot_env *env, struct plot_value **args, int argc);

/***** mathmatical functions ******/

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

/********* equivalent predicates *********/
/* equal?
 */
struct plot_value * plot_func_equal(struct plot_env *env, struct plot_value **args, int argc);

/********* value testing functions ********/

/* boolean?
 */
struct plot_value * plot_func_boolean_test(struct plot_env *env, struct plot_value **args, int argc);

/* symbol?
 */
struct plot_value * plot_func_symbol_test(struct plot_env *env, struct plot_value **args, int argc);

/* string?
 */
struct plot_value * plot_func_string_test(struct plot_env *env, struct plot_value **args, int argc);

/* number?
 */
struct plot_value * plot_func_number_test(struct plot_env *env, struct plot_value **args, int argc);

/* function?
 */
struct plot_value * plot_func_procedure_test(struct plot_env *env, struct plot_value **args, int argc);

/****** logical operations ******/

/* truthy not exposed to runtime
 * returns 1 if value is considered truthy
 * returns 0 if falsy
 */
int plot_truthy(plot_value *val);

/* logical and of all arguments */
struct plot_value * plot_func_and(struct plot_env *env, struct plot_value **args, int argc);

/* logical or of all arguments */
struct plot_value * plot_func_or(struct plot_env *env, struct plot_value **args, int argc);

/* logical not of single argument */
struct plot_value * plot_func_not(struct plot_env *env, struct plot_value **args, int argc);

/******* string operations *******/
/* return plot number representing length of string (excluding null terminator)
 * 'number of characters in string'
 */
struct plot_value * plot_func_string_length(struct plot_env *env, struct plot_value **args, int argc);

/* (substring string start end)
 * 0 <= start <= end <= (string-length string)
 * returns a string from start (inclusive) to end (exclusive)
 */
struct plot_value * plot_func_substring(struct plot_env *env, struct plot_value **args, int argc);

/* (string-append string ...)
 * returns a new string whose characters form the concatentation
 * of the given strings
 */
struct plot_value * plot_func_string_append(struct plot_env *env, struct plot_value **args, int argc);

/* (string-copy string)
 * returns a newly allocated copy of the given string
 */
struct plot_value * plot_func_string_copy(struct plot_env *env, struct plot_value **args, int argc);

/* (string=? str1 str2)
 * string equality test (case sensitive)
 * returns #t iff both strings are the same length and contain the same characters
 */
struct plot_value * plot_func_string_equal(struct plot_env *env, struct plot_value **args, int argc);

/* (string-ci=? str1 str2)
 * string equality test that treats upper and lowercase as the same (case insensitive)
 * returns #t iff both string are the same length and contains the same characters
 */
struct plot_value * plot_func_string_ci_equal(struct plot_env *env, struct plot_value **args, int argc);

#endif

#ifndef PLOT_FUNCS_H
#define PLOT_FUNCS_H

struct plot_env;
struct plot_value;
struct plot_expr;

#include "character.h"
#include "string.h"
#include "number.h"
#include "pair.h"

/* print value to stdout
 */
struct plot_value * plot_func_display(struct plot_env *env, struct plot_value **args, int argc);

/* print a newline to stdout
 */
struct plot_value * plot_func_newline(struct plot_env *env, struct plot_value **args, int argc);

/********* equivalent predicates *********/
/* equal?
 */
struct plot_value * plot_func_equal_test(struct plot_env *env, struct plot_value **args, int argc);

/********* value testing functions ********/

/* boolean?
 */
struct plot_value * plot_func_boolean_test(struct plot_env *env, struct plot_value **args, int argc);

/* symbol?
 */
struct plot_value * plot_func_symbol_test(struct plot_env *env, struct plot_value **args, int argc);

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

#endif

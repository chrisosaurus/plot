#ifndef PLOT_FUNCS_H
#define PLOT_FUNCS_H

struct plot_env;
struct plot_value;
struct plot_expr;

#include "character.h"
#include "string.h"
#include "number.h"
#include "pair.h"

/* (display obj)
 * print value to stdout
 * FIXME should generalise if spec allows
 */
struct plot_value * plot_func_display(struct plot_env *env, struct plot_value **args, int argc);

/* (newline)
 * print a newline to stdout
 */
struct plot_value * plot_func_newline(struct plot_env *env, struct plot_value **args, int argc);

/********* equivalent predicates *********/
/* (equal? obj1 obj2)
 * in plot equal?, eqv? and eq? are equivalent.
 * FIXME need to generalise
 */
struct plot_value * plot_func_equal_test(struct plot_env *env, struct plot_value **args, int argc);

/* (eqv? obj1 obj2)
 * in plot equal?, eqv? and eq? are equivalent.
 * FIXME need to generalise
 */
struct plot_value * plot_func_eqv_test(struct plot_env *env, struct plot_value **args, int argc);

/* (eq? obj1 obj2)
 * in plot equal?, eqv? and eq? are equivalent.
 * FIXME need to generalise
 */
struct plot_value * plot_func_eq_test(struct plot_env *env, struct plot_value **args, int argc);

/********* value testing functions ********/

/* (boolean? obj1)
 * FIXME should generalise if spec allows
 */
struct plot_value * plot_func_boolean_test(struct plot_env *env, struct plot_value **args, int argc);

/* (symbol? obj)
 * FIXME should generalise if spec allows
 */
struct plot_value * plot_func_symbol_test(struct plot_env *env, struct plot_value **args, int argc);

/****** logical operations ******/

/* truthy not exposed to runtime
 * returns 1 if value is considered truthy
 * returns 0 if falsy
 *
 * will decr suppleid val
 */
int plot_truthy(plot_value *val);

/* (and obj1 obj2 ...)
 * logical and of all arguments
 */
struct plot_value * plot_func_and(struct plot_env *env, struct plot_value **args, int argc);

/* (or obj1 obj2 ...)
 * logical or of all arguments
 */
struct plot_value * plot_func_or(struct plot_env *env, struct plot_value **args, int argc);

/* (not obj)
 * logical not of single argument
 */
struct plot_value * plot_func_not(struct plot_env *env, struct plot_value **args, int argc);

/* (exit obj)
 * exit after running all dynamic-wind `after` procedures
 * FIXME plot currently lacks dynamic-wind so this is equivalent to `emergency-exit`
 *
 * exits with success for the following calls:
 *      (exit)
 *      (exit 0)
 *      (exit #t)
 *
 * otherwise exits failure
 *
 */
struct plot_value * plot_func_exit(struct plot_env *env, struct plot_value **args, int argc);

/* (emergency-exit obj)
 * emergency-exit will immediately exit the program without running any dynamic-unwind
 *
 * exits with success for the following calls:
 *      (emergency-exit)
 *      (emergency-exit 0)
 *      (emergency-exit #t)
 *
 * otherwise exits failure
 *
 */
struct plot_value * plot_func_emergency_exit(struct plot_env *env, struct plot_value **args, int argc);

/* (force promise)
 */
struct plot_value * plot_func_force(struct plot_env *env, struct plot_value **args, int argc);


#endif

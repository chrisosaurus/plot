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
 * FIXME need to generalise
 */
struct plot_value * plot_func_equal_test(struct plot_env *env, struct plot_value **args, int argc);

/********* value testing functions ********/

/* (boolean? obj1)
 * FIXME should generalise if spec allows
 */
struct plot_value * plot_func_boolean_test(struct plot_env *env, struct plot_value **args, int argc);

/* (symbol? obj)
 * FIXME should generalise if spec allows
 */
struct plot_value * plot_func_symbol_test(struct plot_env *env, struct plot_value **args, int argc);

/* (procedure? obj)
 * FIXME should generalise if spec allows
 */
struct plot_value * plot_func_procedure_test(struct plot_env *env, struct plot_value **args, int argc);

/****** logical operations ******/

/* truthy not exposed to runtime
 * returns 1 if value is considered truthy
 * returns 0 if falsy
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
 * FIXME not standard compliant, see r7rs page 59 section 6.14
 *
 * exits normally iff obj is #t or 0
 * otherwise is equiv to (exit 1)
 *
 */
struct plot_value * plot_func_exit(struct plot_env *env, struct plot_value **args, int argc);

#endif

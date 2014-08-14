#ifndef PLOT_FUNCS_H
#define PLOT_FUNCS_H

struct plot_env;
struct plot_value;
struct plot_expr;

/********* equivalent predicates *********/
/* (equal? obj1 obj2)
 * in plot equal?, eqv? and eq? are equivalent.
 * FIXME need to generalise
 */
struct plot_value * plot_func_equal_test(struct plot_env *env, struct plot_value *args);

/* (eqv? obj1 obj2)
 * in plot equal?, eqv? and eq? are equivalent.
 * FIXME need to generalise
 */
struct plot_value * plot_func_eqv_test(struct plot_env *env, struct plot_value *args);

/* (eq? obj1 obj2)
 * in plot equal?, eqv? and eq? are equivalent.
 * FIXME need to generalise
 */
struct plot_value * plot_func_eq_test(struct plot_env *env, struct plot_value *args);

/********* value testing functions ********/

/* (boolean? obj1)
 * FIXME should generalise if spec allows
 */
struct plot_value * plot_func_boolean_test(struct plot_env *env, struct plot_value *args);

/* (symbol? obj)
 * FIXME should generalise if spec allows
 */
struct plot_value * plot_func_symbol_test(struct plot_env *env, struct plot_value *args);

/* (symbol=? obj1 obj2)
 * returns #t iff obj1 and obj2 are both symbols and are considered equal
 * otherwise returns #f
 */
struct plot_value * plot_func_symbol_equal_test(struct plot_env *env, struct plot_value *args);

/****** logical operations ******/

/* truthy not exposed to runtime
 * returns 1 if value is considered truthy
 * returns 0 if falsy
 *
 * will decr suppleid val
 */
int plot_truthy(plot_value *val);

/* (not obj)
 * logical not of single argument
 */
struct plot_value * plot_func_not(struct plot_env *env, struct plot_value *args);

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
struct plot_value * plot_func_exit(struct plot_env *env, struct plot_value *args);

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
struct plot_value * plot_func_emergency_exit(struct plot_env *env, struct plot_value *args);

/* (force promise)
 * returns a non-promise unmodified
 */
struct plot_value * plot_func_force(struct plot_env *env, struct plot_value *args);

/* (promise? obj)
 * returns #t iff obj is of type promise
 * otherwise returns #f
 */
struct plot_value * plot_func_promise_test(struct plot_env *env, struct plot_value *args);

/* (make-promise obj)
 * returns a new promise that will yield obj when forced
 * if obj is already a promise then it is returned
 *
 * make-promise is a procedure rather than syntax so it WILL evaluated it's argument
 * make-promise is like delay but does not delay it's argument
 *
 *  (make-promise obj)
 *
 * is equivalent to
 *
 *  (begin
 *      (define tmp (delay obj))
 *      (force tmp)
 *      tmp)
 */
struct plot_value * plot_func_make_promise(struct plot_env *env, struct plot_value *args);

#endif

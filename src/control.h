#ifndef PLOT_CONTROL_H
#define PLOT_CONTROL_H

struct plot_env;
struct plot_value;

/* procedures from r7rs 6.10 Control Features
 */

/* (procedure? obj)
 * FIXME should generalise if spec allows
 */
struct plot_value * plot_func_control_procedure_test(struct plot_env *env, struct plot_value *args);

/* (apply proc args1 ... args)
 *   calls proc with the elements of the list
 *   (append (list arg1...) args) as the argument
 *
 * allows for both:
 *  (apply + '(3 4))   ; => 7
 *  (apply + 3 '(1 3)) ; => 7
 */
struct plot_value * plot_func_control_apply(struct plot_env *env, struct plot_value *args);

/* (map proc list1 list2...)
 * error if proc does not accept as many args as there are lists
 *
 * map applies proc element-wise to the elements of the
 * lists and returns a list of the results.
 *
 * will only go as far as the shortest list.
 *
 * it is an error for proc to mutate any of the lists
 *
 * (map cadr '((a b) (d e) (g h))) ; => (b e h)
 * (map (lambda (x y) (* x y)) '(1 2) '(3 4)) ; => (3 8)
 */
struct plot_value * plot_func_control_map(struct plot_env *env, struct plot_value *args);

/* (string-map proc string1 string2 ...)
 * it is an error if proc does not take as many arguments as there are strings and return a single character
 *
 * string-map applies proc element-wise to the elements of the strings and returns a string of the results, in order.
 * (string-map char-foldcase "AbdEgH") ;; => "abdegh"
 */
struct plot_value * plot_func_control_string_map(struct plot_env *env, struct plot_value *args);

/* (for-each proc list1 list2 ... )
 * error if proc does not accept as many args as there are lists
 *
 * similar to map except that for-each is run for it's side effects rather
 * than for its values.
 *
 * return value is unspecified.
 *
 * unlike map, for-each is guranteed to call proc on the elements of the lists in order
 * (from first to last)
 *
 * it is an error for proc to mutate any of the lists.
 *
 *
 */
struct plot_value * plot_func_control_for_each(struct plot_env *env, struct plot_value *args);

/* (string-for-each proc string1 string2 ...)
 * it is an error if proc does not take as may argumnets as there are string and return a single character
 *
 * string-for-each applies proc element wise to the elements of the strings and returns a string of the results in order.
 *
 * return value is unspecified
 */
struct plot_value * plot_func_control_string_for_each(struct plot_env *env, struct plot_value *args);

#endif

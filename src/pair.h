#ifndef PLOT_PAIR_H
#define PLOT_PAIR_H

struct plot_env;
struct plot_value;

/* (pair? obj)
 * return #t iff obj is of type plot_type_pair
 */
struct plot_value * plot_func_pair_test(struct plot_env *env, struct plot_value **args, int argc);

/* (cons v1 v2)
 * return a pair with v1 as car and v2 as cdr
 */
struct plot_value * plot_func_pair_cons(struct plot_env *env, struct plot_value **args, int argc);

/* (car obj)
 * return contents of car or error
 */
struct plot_value * plot_func_pair_car(struct plot_env *env, struct plot_value **args, int argc);

/* (cdr obj)
 * returns contents of cdr or error
 */
struct plot_value * plot_func_pair_cdr(struct plot_env *env, struct plot_value **args, int argc);

/* (set-car! pair obj)
 */
struct plot_value * plot_func_pair_set_car(struct plot_env *env, struct plot_value **args, int argc);

/* (set-cdr! pair obj)
 */
struct plot_value * plot_func_pair_set_cdr(struct plot_env *env, struct plot_value **args, int argc);

/* (null? obj)
 * return #t iff obj is the empty list, otherwise #f
 */
struct plot_value * plot_func_pair_null_test(struct plot_env *env, struct plot_value **args, int argc);

/* (list? obj)
 * return #t iff obj is a list, otherwise returns #f
 */
struct plot_value * plot_func_pair_list_test(struct plot_env *env, struct plot_value **args, int argc);

/* (list obj ...)
 * list constructor
 * returns a newly allocated made up of all the objects specified
 * terminated by empty list
 */
struct plot_value * plot_func_pair_list(struct plot_env *env, struct plot_value **args, int argc);

/* (length list)
 * returns list length
 */
struct plot_value * plot_func_pair_length(struct plot_env *env, struct plot_value **args, int argc);

/* (append list ...)
 * returns a newly allocated list made up on the concatenation of all provided lists
 */
struct plot_value * plot_func_pair_append(struct plot_env *env, struct plot_value **args, int argc);

/* (reverse list)
 * return new allocated list containing all the values form list in reverse order
 */
struct plot_value * plot_func_pair_reverse(struct plot_env *env, struct plot_value **args, int argc);


#endif

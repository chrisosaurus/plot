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

#endif

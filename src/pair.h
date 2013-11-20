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

/* (make-list k)
 * (make-list k fill)
 * returns a newly allocated list of k elements
 * if `fill` is provided then each element will be set, otherwise contents are unspecified
 */
struct plot_value * plot_func_pair_make_list(struct plot_env *env, struct plot_value **args, int argc);

/* (list-tail list k)
 * returns sublist of list obtained by omitting the first k elements
 *
 * error if list has fewer than k elements
 */
struct plot_value * plot_func_pair_list_tail(struct plot_env *env, struct plot_value **args, int argc);

/* (list-ref list k)
 * returns kth element of list
 * equivalent to (car (list-tail list k))
 *
 * error if list has fewer than k elements
 * list is allowed to be circular
 */
struct plot_value * plot_func_pair_list_ref(struct plot_env *env, struct plot_value **args, int argc);

/* (list-set! list k obj)
 * stores obj in element k of list
 *
 * error if k is not a valid index into list
 */
struct plot_value * plot_func_pair_list_set(struct plot_env *env, struct plot_value **args, int argc);

/* (memq obj list)
 * (memq obj list compare)
 *
 * returns sublist of list whose car is obj
 * otherwise returns #f
 *
 * compares using `eqq?`
 */
struct plot_value * plot_func_pair_memq(struct plot_env *env, struct plot_value **args, int argc);

/* (memv obj list)
 * (memv obj list compare)
 *
 * returns sublist of list whose car is obj
 * otherwise returns #f
 *
 * compares using `eqv?`
 */
struct plot_value * plot_func_pair_memv(struct plot_env *env, struct plot_value **args, int argc);

/* (member obj list)
 * (member obj list compare)
 *
 * returns sublist of list whose car is obj
 * otherwise returns #f
 *
 * compares using provided `compare` proc, otherwise uses `equal?`
 */
struct plot_value * plot_func_pair_member(struct plot_env *env, struct plot_value **args, int argc);

/* (assq obj alist)
 * alist is an association list (list of pairs)
 *
 * assq finds the first set of pairs of car is obj and returns that pair
 * otherwise #f (if no pair has obj as car)
 *
 * compares using `eq?`
 */
struct plot_value * plot_func_pair_assq(struct plot_env *env, struct plot_value **args, int argc);

/* (assv obj alist)
 * alist is an association list (list of pairs)
 *
 * assv finds the first set of pairs of car is obj and returns that pair
 * otherwise #f (if no pair has obj as car)
 *
 * compares using `eqv?`
 */
struct plot_value * plot_func_pair_assv(struct plot_env *env, struct plot_value **args, int argc);

/* (assoc obj alist)
 * (assoc obj alist compare)
 * alist is an association list (list of pairs)
 *
 * assoc finds the first set of pairs of car is obj and returns that pair
 * otherwise #f (if no pair has obj as car)
 *
 * compares using provided `compare` proc, otherwise uses `equal?`
 */
struct plot_value * plot_func_pair_assoc(struct plot_env *env, struct plot_value **args, int argc);

/* (list-copy obj)
 * returns a newly allocated copy of obj if it is a list
 * only pairs are copied, cars are the same (in the sense of eqv?)
 * if obj is improper then so is the result (and then the cdrs are the same)
 * an obj which is not a list is returned unchanged
 * error if obj is circular
 */
struct plot_value * plot_func_pair_list_copy(struct plot_env *env, struct plot_value **args, int argc);

#endif

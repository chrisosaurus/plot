#ifndef PLOT_PAIR_H
#define PLOT_PAIR_H

struct plot_env;
struct plot_value;

/* (pair? obj)
 * return #t iff obj is of type plot_type_pair
 */
struct plot_value * plot_func_pair_test(struct plot_env *env, struct plot_value *args);

/* (cons v1 v2)
 * return a pair with v1 as car and v2 as cdr
 */
struct plot_value * plot_func_pair_cons(struct plot_env *env, struct plot_value *args);

/* (car obj)
 * return contents of car or error
 */
struct plot_value * plot_func_pair_car(struct plot_env *env, struct plot_value *args);

/* (cdr obj)
 * returns contents of cdr or error
 */
struct plot_value * plot_func_pair_cdr(struct plot_env *env, struct plot_value *args);

/* (set-car! pair obj)
 */
struct plot_value * plot_func_pair_set_car(struct plot_env *env, struct plot_value *args);

/* (set-cdr! pair obj)
 */
struct plot_value * plot_func_pair_set_cdr(struct plot_env *env, struct plot_value *args);

/* (null? obj)
 * return #t iff obj is the empty list, otherwise #f
 */
struct plot_value * plot_func_pair_null_test(struct plot_env *env, struct plot_value *args);

/* (list? obj)
 * return #t iff obj is a list, otherwise returns #f
 */
struct plot_value * plot_func_pair_list_test(struct plot_env *env, struct plot_value *args);

/* (list obj ...)
 * list constructor
 * returns a newly allocated made up of all the objects specified
 * terminated by empty list
 */
struct plot_value * plot_func_pair_list(struct plot_env *env, struct plot_value *args);

/* (length list)
 * returns list length
 */
struct plot_value * plot_func_pair_length(struct plot_env *env, struct plot_value *args);


/* (append list ...)
 * returns a newly allocated list made up on the concatenation of all provided lists
 * final item can be a non-list, in which case the resulting list is an improper-list
 *
 * if there are no arguments then the empty list is returned
 *
 * the result is a newly allocated list except that it shares structure with the final arg
 *
 * (append '(1 2 3) '(4 5 6))   ; => '(1 2 3 4 5 6)
 * (append '(1 2 3) '(4 5 6) 7) ; => '(1 2 3 4 5 6 . 7)
 *
 * results will be messy if any of the args except for the final one are not lists
 * (append 'a '(1 2 3) 'a '(4 5 6) 7 ) ; => '(1 2 3 4 5 6 . 7)
 * notice how both the 'a (s) are not present in the final output.
 *
 * you should not rely on this behavior as it is non-standard, in the future plot may
 * throw errors
 *
 * FIXME consider throwing errors on non-lists as non-last arguments.
 *
 * (define (flatten list)
 *      (apply append list))
 * (flatten '((1 2 3) (4 5 6))) ; => '(1 2 3 4 5 6)
 *
 */
struct plot_value * plot_func_pair_append(struct plot_env *env, struct plot_value *args);

/* (reverse list)
 * return new allocated list containing all the values from list in reverse order
 */
struct plot_value * plot_func_pair_reverse(struct plot_env *env, struct plot_value *args);

/* (make-list k)
 * (make-list k fill)
 * returns a newly allocated list of k elements
 * if `fill` is provided then each element will be set, otherwise contents are unspecified
 */
struct plot_value * plot_func_pair_make_list(struct plot_env *env, struct plot_value *args);

/* (list-tail list k)
 * returns sublist of list obtained by omitting the first k elements
 *
 * error if list has fewer than k elements
 */
struct plot_value * plot_func_pair_list_tail(struct plot_env *env, struct plot_value *args);

/* (list-ref list k)
 * returns kth element of list
 * equivalent to (car (list-tail list k))
 *
 * error if list has fewer than k elements
 * list is allowed to be circular
 */
struct plot_value * plot_func_pair_list_ref(struct plot_env *env, struct plot_value *args);

/* (list-set! list k obj)
 * stores obj in element k of list
 *
 * error if k is not a valid index into list
 */
struct plot_value * plot_func_pair_list_set(struct plot_env *env, struct plot_value *args);

/* (memq obj list)
 * (memq obj list compare)
 *
 * returns sublist of list whose car is obj
 * otherwise returns #f
 *
 * compares using `eq?`
 */
struct plot_value * plot_func_pair_memq(struct plot_env *env, struct plot_value *args);

/* (memv obj list)
 * (memv obj list compare)
 *
 * returns sublist of list whose car is obj
 * otherwise returns #f
 *
 * compares using `eqv?`
 */
struct plot_value * plot_func_pair_memv(struct plot_env *env, struct plot_value *args);

/* (member obj list)
 * (member obj list compare)
 *
 * returns sublist of list whose car is obj
 * otherwise returns #f
 *
 * compares using provided `compare` proc, otherwise uses `equal?`
 */
struct plot_value * plot_func_pair_member(struct plot_env *env, struct plot_value *args);

/* (assq obj alist)
 * alist is an association list (list of pairs)
 *
 * assq finds the first set of pairs of car is obj and returns that pair
 * otherwise #f (if no pair has obj as car)
 *
 * compares using `eq?`
 */
struct plot_value * plot_func_pair_assq(struct plot_env *env, struct plot_value *args);

/* (assv obj alist)
 * alist is an association list (list of pairs)
 *
 * assv finds the first set of pairs of car is obj and returns that pair
 * otherwise #f (if no pair has obj as car)
 *
 * compares using `eqv?`
 */
struct plot_value * plot_func_pair_assv(struct plot_env *env, struct plot_value *args);

/* (assoc obj alist)
 * (assoc obj alist compare)
 * alist is an association list (list of pairs)
 *
 * assoc finds the first set of pairs of car is obj and returns that pair
 * otherwise #f (if no pair has obj as car)
 *
 * compares using provided `compare` proc, otherwise uses `equal?`
 */
struct plot_value * plot_func_pair_assoc(struct plot_env *env, struct plot_value *args);

/* (list-copy obj)
 * returns a newly allocated copy of obj if it is a list
 * only pairs are copied, cars are the same (in the sense of eqv?)
 * if obj is improper then so is the result (and then the cdrs are the same)
 * an obj which is not a list is returned unchanged
 * error if obj is circular
 */
struct plot_value * plot_func_pair_list_copy(struct plot_env *env, struct plot_value *args);

#endif

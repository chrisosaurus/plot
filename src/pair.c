#include "value.h"
#include "pair.h"
#include <stdio.h>

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

#define DEBUG 0

/* (pair? obj)
 * return #t iff obj is of type plot_type_pair
 */
struct plot_value * plot_func_pair_test(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 argument", "plot_func_pair_test");
    }

    return plot_new_boolean( args[0]->type == plot_type_pair );
}

/* (cons v1 v2)
 * return a pair with v1 as car and v2 as cdr
 */
struct plot_value * plot_func_pair_cons(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 arguments", "plot_func_pair_cons");
    }

    plot_value_incr(args[0]);

    plot_value_incr(args[1]);

    return plot_new_pair(args[0], args[1]);
}

/* (car obj)
 * return contents of car or error
 */
struct plot_value * plot_func_pair_car(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 argument", "plot_func_pair_car");
    }

    if( args[0]->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_pair", "plot_func_pair_car");
    }

    res = args[0]->u.pair.car;
    plot_value_incr(res);

    return res;
}

/* (cdr obj)
 * returns contents of cdr or error
 */
struct plot_value * plot_func_pair_cdr(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 argument", "plot_func_pair_cdr");
    }

    if( args[0]->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_pair", "plot_func_pair_cdr");
    }

    res = args[0]->u.pair.cdr;
    plot_value_incr(res);

    return res;
}

/* (set-car! pair obj)
 */
struct plot_value * plot_func_pair_set_car(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 args", "plot_func_pair_set_car");
    }

    if( args[0]->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not a pair", "plot_func_pair_set_car");
    }

    plot_value_decr(args[0]->u.pair.car);
    args[0]->u.pair.car = args[1];
    plot_value_incr(args[0]->u.pair.car);

    return plot_new_unspecified();
}

/* (set-cdr! pair obj)
 */
struct plot_value * plot_func_pair_set_cdr(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 args", "plot_func_pair_set_cdr");
    }

    if( args[0]->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not a pair", "plot_func_pair_set_cdr");
    }

    plot_value_decr(args[0]->u.pair.cdr);
    args[0]->u.pair.cdr = args[1];
    plot_value_incr(args[0]->u.pair.cdr);

    return plot_new_unspecified();
}

/* (null? obj)
 * return #t iff obj is the empty list, otherwise #f
 */
struct plot_value * plot_func_pair_null_test(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 arg", "plot_func_pair_null_test");
    }

    return plot_new_boolean( args[0]->type == plot_type_null );
}

/* (list? obj)
 * return #t iff obj is a list, otherwise returns #f
 * FIXME generalise
 */
struct plot_value * plot_func_pair_list_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *val;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_pair_list_test");
    }

    val = args[0];
    while( val->type == plot_type_pair ){
        val = val->u.pair.cdr;
    }

    return plot_new_boolean(val->type == plot_type_null);
}

/* (list obj ...)
 * list constructor
 * returns a newly allocated made up of all the objects specified
 * terminated by empty list
 */
struct plot_value * plot_func_pair_list(struct plot_env *env, struct plot_value **args, int argc){
    int i;
    plot_value *current;

    current = plot_new_null();

    for( i=argc; i>0; --i ){
        current = plot_new_pair(args[i - 1], current);
    }

    return current;
}

/* (length list)
 * returns list length
 * FIXME may want to trigger an error if obj is not a list
 */
struct plot_value * plot_func_pair_length(struct plot_env *env, struct plot_value *args){
    int len = 0;
    plot_value *val;

    val = car(args);
    while( val->type == plot_type_pair ){
        ++len;
        val = cdr(val);
    }

    return plot_new_number(len);
}

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
 */
struct plot_value * plot_func_pair_append(struct plot_env *env, struct plot_value *args){
    plot_value *head, **outcur, *arg, *elem;

    head = null;
    outcur = &head;
    arg = args;

    /* each argument is a list, so for each argument we need to iterate
     * through the sublist copying over the elements until we find the tail
     */
    for( arg = args; arg->type == plot_type_pair; arg = cdr(arg) ){
        for( elem = car(arg); elem->type == plot_type_pair; elem = cdr(elem) ){
            *outcur = cons(0, null);
            car(*outcur) = car(elem);
            plot_value_incr(car(elem));
            outcur = &cdr(*outcur);
        }

        /* need to deal with final element which may not be a list */
        if( elem->type != plot_type_null ){
            /* neither pair nor null */
            *outcur = elem;
            plot_value_incr(elem);
        }
    }

    return head;
}

/* (reverse list)
 * return new allocated list containing all the values form list in reverse order
 */
struct plot_value * plot_func_pair_reverse(struct plot_env *env, struct plot_value *args){
    plot_value *val, *tmp;

    if( args->type == plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_pair_reverse");
    }

    switch( car(args)->type ){
        case plot_type_pair:
            break;
        case plot_type_null:
            return null;
            break;
        default:
            return plot_runtime_error(plot_error_bad_args, "first arg was not a list", "plot_func_pair_reverse");
            break;
    }

    tmp = car(args);
    val = null;

    while( tmp->type == plot_type_pair ){
        plot_value_incr(car(tmp));
        val = plot_new_pair(car(tmp), val);
        tmp = cdr(tmp);
    }

    if( tmp->type == plot_type_null ){
        return val;
    } else {
        /* FIXME need to tidy up locally
         * created objects and ref counts
         */
        return plot_runtime_error(plot_error_bad_args, "obj was not a list, encountered unexpected type", "plot_func_pair_reverse");
    }
}

/* (make-list k)
 * (make-list k fill)
 * returns a newly allocated list of k elements
 * if `fill` is provided then each element will be set, otherwise contents are unspecified
 */
struct plot_value * plot_func_pair_make_list(struct plot_env *env, struct plot_value *args){
    int i;
    int k;
    plot_value *fill;
    plot_value *list;

    if( args->type == plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected either 1 or 2 arguments", "plot_func_pair_make_list");
    }

    if( car(args)->type != plot_type_number ){
        return plot_runtime_error(plot_error_bad_args, "first argument is not of type plot_type_number", "plot_func_pair_make_list");
    }

    if( cdr(args)->type == plot_type_null ){
        fill = plot_new_unspecified();
    } else {
        fill = car(cdr(args));
    }

    list = null;
    k = car(args)->u.number.val;

    for( i=0; i < k; ++i ){
        plot_value_incr(fill);
        list = plot_new_pair(fill, list);
    }

    return list;

}

/* (list-tail list k)
 * returns sublist of list obtained by omitting the first k elements
 *
 * error if list has fewer than k elements
 */
struct plot_value * plot_func_pair_list_tail(struct plot_env *env, struct plot_value *args){
    return plot_runtime_error(plot_error_unimplemented, "unimplemented", "plot_func_pair_list_tail");
}

/* (list-ref list k)
 * returns kth element of list
 * equivalent to (car (list-tail list k))
 *
 * error if list has fewer than k elements
 * list is allowed to be circular
 */
struct plot_value * plot_func_pair_list_ref(struct plot_env *env, struct plot_value *args){
    return plot_runtime_error(plot_error_unimplemented, "unimplemented", "plot_func_pair_list_ref");
}

/* (list-set! list k obj)
 * stores obj in element k of list
 *
 * error if k is not a valid index into list
 */
struct plot_value * plot_func_pair_list_set(struct plot_env *env, struct plot_value *args){
    return plot_runtime_error(plot_error_unimplemented, "unimplemented", "plot_func_pair_list_set");
}

/* (memq obj list)
 * (memq obj list compare)
 *
 * returns sublist of list whose car is obj
 * otherwise returns #f
 *
 * compares using `eqq?`
 */
struct plot_value * plot_func_pair_memq(struct plot_env *env, struct plot_value *args){
    return plot_runtime_error(plot_error_unimplemented, "unimplemented", "plot_func_pair_memq");
}

/* (memv obj list)
 * (memv obj list compare)
 *
 * returns sublist of list whose car is obj
 * otherwise returns #f
 *
 * compares using `eqv?`
 */
struct plot_value * plot_func_pair_memv(struct plot_env *env, struct plot_value *args){
    return plot_runtime_error(plot_error_unimplemented, "unimplemented", "plot_func_pair_memv");
}

/* (member obj list)
 * (member obj list compare)
 *
 * returns sublist of list whose car is obj
 * otherwise returns #f
 *
 * compares using provided `compare` proc, otherwise uses `equal?`
 */
struct plot_value * plot_func_pair_member(struct plot_env *env, struct plot_value *args){
    return plot_runtime_error(plot_error_unimplemented, "unimplemented", "plot_func_pair_member");
}

/* (assq obj alist)
 * alist is an association list (list of pairs)
 *
 * assq finds the first set of pairs of car is obj and returns that pair
 * otherwise #f (if no pair has obj as car)
 *
 * compares using `eq?`
 */
struct plot_value * plot_func_pair_assq(struct plot_env *env, struct plot_value *args){
    return plot_runtime_error(plot_error_unimplemented, "unimplemented", "plot_func_pair_assq");
}

/* (assv obj alist)
 * alist is an association list (list of pairs)
 *
 * assv finds the first set of pairs of car is obj and returns that pair
 * otherwise #f (if no pair has obj as car)
 *
 * compares using `eqv?`
 */
struct plot_value * plot_func_pair_assv(struct plot_env *env, struct plot_value *args){
    return plot_runtime_error(plot_error_unimplemented, "unimplemented", "plot_func_pair_assv");
}

/* (assoc obj alist)
 * (assoc obj alist compare)
 * alist is an association list (list of pairs)
 *
 * assoc finds the first set of pairs of car is obj and returns that pair
 * otherwise #f (if no pair has obj as car)
 *
 * compares using provided `compare` proc, otherwise uses `equal?`
 */
struct plot_value * plot_func_pair_assoc(struct plot_env *env, struct plot_value *args){
    return plot_runtime_error(plot_error_unimplemented, "unimplemented", "plot_func_pair_assoc");
}

/* (list-copy obj)
 * returns a newly allocated copy of obj if it is a list
 * only pairs are copied, cars are the same (in the sense of eqv?)
 * if obj is improper then so is the result (and then the cdrs are the same)
 * an obj which is not a list is returned unchanged
 * error if obj is circular
 */
struct plot_value * plot_func_pair_list_copy(struct plot_env *env, struct plot_value *args){
    plot_value *head, **cur;
    plot_value *old;

    if( car(args)->type != plot_type_pair && car(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_pair or plot_type_null", "plot_func_pair_list_copy");
    }

    head = null;
    cur = &head;

    for( old = car(args); old->type != plot_type_null; old = cdr(old) ){
        *cur = cons(0, null);
        car(*cur) = car(old);
        plot_value_incr(car(old));
        cur = &cdr(*cur);
    }

    return head;
}


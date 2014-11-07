#include "pair.h"
#include "value.h"
#include "funcs.h" /* equal?, plot_truthy */
#include "eval.h" /* plot_eval_apply */
#include <stdio.h>

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

#define DEBUG 0

/* (pair? obj)
 * return #t iff obj is of type plot_type_pair
 */
struct plot_value * plot_func_pair_test(struct plot_env *env, struct plot_value *args){
    if( args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 argument", "plot_func_pair_test");
    }

    return plot_new_boolean( car(args)->type == plot_type_pair );
}

/* (cons v1 v2)
 * return a pair with v1 as car and v2 as cdr
 */
struct plot_value * plot_func_pair_cons(struct plot_env *env, struct plot_value *args){
    plot_value *l, *r;
    if( args->type != plot_type_pair || cdr(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 arguments", "plot_func_pair_cons");
    }

    l = car(args);
    r = car(cdr(args));

    plot_value_incr(l);

    plot_value_incr(r);

    return plot_new_pair(l, r);
}

/* (car obj)
 * return contents of car or error
 */
struct plot_value * plot_func_pair_car(struct plot_env *env, struct plot_value *args){
    plot_value *res;

    if( args->type == plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 argument", "plot_func_pair_car");
    }

    if( args->u.pair.car->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_pair", "plot_func_pair_car");
    }

    /* do not want to use car and cdr macro here in case we redirect
     * the macros to point to these functions
     */
    res = args->u.pair.car->u.pair.car;
    plot_value_incr(res);

    return res;
}

/* (cdr obj)
 * returns contents of cdr or error
 */
struct plot_value * plot_func_pair_cdr(struct plot_env *env, struct plot_value *args){
    plot_value *res;

    if( args->type == plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 argument", "plot_func_pair_cdr");
    }

    if( args->u.pair.car->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_pair", "plot_func_pair_cdr");
    }

    /* do not want to use car and cdr macro here in case we redirect
     * the macros to point to these functions
     */
    res = args->u.pair.car->u.pair.cdr;
    plot_value_incr(res);

    return res;
}

/* (set-car! pair obj)
 */
struct plot_value * plot_func_pair_set_car(struct plot_env *env, struct plot_value *args){
    plot_value *new;
    if( cdr(args)->type == plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 args", "plot_func_pair_set_cdr");
    }

    if( car(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not a pair", "plot_func_pair_set_cdr");
    }

    new = car(cdr(args));

    plot_value_decr(car(car(args)));
    car(car(args)) = new;
    plot_value_incr(new);

    return plot_new_unspecified();
}

/* (set-cdr! pair obj)
 */
struct plot_value * plot_func_pair_set_cdr(struct plot_env *env, struct plot_value *args){
    plot_value *new;
    if( cdr(args)->type == plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 args", "plot_func_pair_set_cdr");
    }

    if( car(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not a pair", "plot_func_pair_set_cdr");
    }

    new = car(cdr(args));

    plot_value_decr(cdr(car(args)));
    cdr(car(args)) = new;
    plot_value_incr(new);

    return plot_new_unspecified();
}

/* (null? obj)
 * return #t iff obj is the empty list, otherwise #f
 */
struct plot_value * plot_func_pair_null_test(struct plot_env *env, struct plot_value *args){
    return plot_new_boolean( car(args)->type == plot_type_null );
}

/* (list? obj)
 * return #t iff obj is a list, otherwise returns #f
 * FIXME generalise
 */
struct plot_value * plot_func_pair_list_test(struct plot_env *env, struct plot_value *args){
    plot_value *cur;

    for( cur = car(args); cur->type == plot_type_pair; cur = cdr(cur) );

    return plot_new_boolean(cur->type == plot_type_null);
}

/* (list obj ...)
 * list constructor
 * returns a newly allocated made up of all the objects specified
 * terminated by empty list
 */
struct plot_value * plot_func_pair_list(struct plot_env *env, struct plot_value *args){
    plot_value *head, **cur, *in;

    head = null;
    cur = &head;

    for( in = args; in->type == plot_type_pair; in = cdr(in) ){
        *cur = cons(0, null);
        car(*cur) = car(in);
        plot_value_incr(car(in));
        cur = &cdr(*cur);
    }

    return head;
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
 *
 * (define (flatten list)
 *      (apply append list))
 * (flatten '((1 2 3) (4 5 6))) ; => '(1 2 3 4 5 6)
 *
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
 * return new allocated list containing all the values from list in reverse order
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
    plot_value *cur;
    int k;

    if( args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 arguments", "plot_func_pair_list_tail");
    }

    if( car(args)->type != plot_type_pair && car(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "first argument was not a list", "plot_func_pair_list_tail");
    }

    if( car(cdr(args))->type != plot_type_number ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not a number", "plot_func_pair_list_tail");
    }

    cur = car(args);
    k = car(cdr(args))->u.number.val;

    for( ; k > 0; --k ){
        if( cur->type != plot_type_pair ){
            return plot_runtime_error(plot_error_bad_args, "k was too large an index for provided list", "plot_func_pair_list_tail");
        }
        cur = cdr(cur);
    }

    plot_value_incr(cur);
    return cur;
}

/* (list-ref list k)
 * returns kth element of list
 * equivalent to (car (list-tail list k))
 *
 * error if list has fewer than k elements
 * list is allowed to be circular
 */
struct plot_value * plot_func_pair_list_ref(struct plot_env *env, struct plot_value *args){
    int k;
    plot_value *cur;

    if( args->type != plot_type_pair || car(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args of type list and number", "plot_func_pair_list_ref");
    }

    k = car(cdr(args))->u.number.val;

    if( k < 0 ){
        return plot_runtime_error(plot_error_bad_args, "negative index provided is illegal", "plot_func_pair_list_ref");
    }

    for( cur = car(args); k > 0; --k ){
        cur = cdr(cur);
        if( cur->type != plot_type_pair ){
            return plot_runtime_error(plot_error_runtime, "provided k is not a valid index into provided list", "plot_func_pair_list_ref");
        }
    }

    /* k is now 0
     * args has to be a pair
     */
    plot_value_incr(car(cur));
    return car(cur);
}

/* (list-set! list k obj)
 * stores obj in element k of list
 *
 * error if k is not a valid index into list
 */
struct plot_value * plot_func_pair_list_set(struct plot_env *env, struct plot_value *args){
    int k;
    plot_value *cur;

    if( args->type != plot_type_pair || car(args)->type != plot_type_pair || car(cdr(args))->type != plot_type_number ){
        return plot_runtime_error(plot_error_bad_args, "expected 3 args of type list, number and any", "plot_func_pair_list_set");
    }

    k = car(cdr(args))->u.number.val;

    if( k < 0 ){
        return plot_runtime_error(plot_error_bad_args, "negative index provided is illegal", "plot_func_pair_list_set");
    }

    for( cur = car(args); k > 0; --k ){
        cur = cdr(cur);
        if( cur->type != plot_type_pair ){
            return plot_runtime_error(plot_error_runtime, "provided k is not a valid index into provided list", "plot_func_pair_list_set");
        }
    }

    /* k is now 0
     * args has to be a pair
     */
    plot_value_decr(car(cur));
    car(cur) = car(cdr(cdr(args)));
    plot_value_incr(car(cur));

    return plot_new_unspecified();
}

/* (memq obj list)
 *
 * returns sublist of list whose car is obj
 * otherwise returns #f
 *
 * compares using `eq?`
 *
 * (memq 'a '(a b c)) ; => (a b c)
 * (memq 'b '(a b c)) ; => (b c)
 * (memq 'a '(b c d)) ; => #f
 */
struct plot_value * plot_func_pair_memq(struct plot_env *env, struct plot_value *args){
    plot_value *func;
    plot_value *res;

    if( args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 args", "plot_func_pair_memq");
    }

    if( cdr(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 args", "plot_func_pair_memq");
    }

    if( cdr(cdr(args))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 args", "plot_func_pair_memq");
    }

    func = plot_new_form(plot_func_eq_test, 0);
    cdr(cdr(args)) = cons(func, null);

    res = plot_func_pair_member(env, args);

    plot_value_decr(cdr(cdr(args)));
    cdr(cdr(args)) = null;

    return res;
}

/* (memv obj list)
 *
 * returns sublist of list whose car is obj
 * otherwise returns #f
 *
 * compares using `eqv?`
 *
 * (memv 'a '(a b c)) ; => (a b c)
 * (memv 'b '(a b c)) ; => (b c)
 * (memv 'a '(b c d)) ; => #f
 */
struct plot_value * plot_func_pair_memv(struct plot_env *env, struct plot_value *args){
    plot_value *func;
    plot_value *res;

    if( args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 args", "plot_func_pair_memv");
    }

    if( cdr(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 args", "plot_func_pair_memv");
    }

    if( cdr(cdr(args))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 args", "plot_func_pair_memv");
    }

    func = plot_new_form(plot_func_eqv_test, 0);
    cdr(cdr(args)) = cons(func, null);

    res = plot_func_pair_member(env, args);

    plot_value_decr(cdr(cdr(args)));
    cdr(cdr(args)) = null;

    return res;
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
    /* current pos in list */
    plot_value *cur;
    /* func given or default */
    plot_value *func;
    /* result from a call */
    plot_value *res;
    /* arg to a call */
    plot_value *arg;
    /* my return value */
    plot_value *ret;

    if( args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected either 2 or 3 arguments", "plot_func_pair_member");
    }

    /* temporarily re-using arg */
    arg = cdr(args);

    if( arg->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected either 2 or 3 arguments, got 1", "plot_func_pair_member");
    }

    /* temporarily re-using arg */
    arg = cdr(arg);

    if( arg->type == plot_type_null ){
        /* default to `equal?` */
        func = plot_new_form(plot_func_equal_test, 0);
    } else if( arg->type == plot_type_pair ){
        /* then car(arg) is our function */
        func = car(arg);

        /* first incr is because we later decr it
         * we must decr it as in the default case of using `equal?`
         * plot_new_form will return a value with refcount of 1
         */
        plot_value_incr(func);

        /* check we received no more than 2 args */
        /* temporarily re-using arg */
        arg = cdr(arg);
        if( arg->type != plot_type_null ){
            return plot_runtime_error(plot_error_bad_args, "expected no more than 2 args", "plot_func_pair_member");
        }
    } else {
        return plot_runtime_error(plot_error_internal, "Error in argument passing", "plot_func_pair_member");
    }

    ret = plot_new_boolean(0);
    arg = cons(0, cons(0, null));
    /* supplied obj to compare to, inserting into later position to make for loop cheaper */
    car(cdr(arg)) = car(args);

    for( cur = car(cdr(args)); cur->type == plot_type_pair; cur = cdr(cur) ){
        /* object in list to compare */
        car(arg) = car(cur);

        res = plot_eval_apply(env, func, arg);
        if( plot_truthy(res) ){
            plot_value_decr(ret);
            ret = cur;
            plot_value_incr(ret);
            break;
        }
        /* plot_truthy no longer decrs */
        plot_value_decr(res);
    }

    /* avoid collecting any of the elements in the list */
    car(cdr(arg)) = 0;
    car(cdr(cdr(arg))) = 0;

    /* func only has a ref count of 1 so it gced for us by decr or arg */
    plot_value_decr(arg);
    return ret;
}

/* (assq obj alist)
 *
 * alist is an association list (list of pairs)
 *
 * assq finds the first set of pairs of car is obj and returns that pair
 * otherwise #f (if no pair has obj as car)
 *
 * compares using `eq?`
 */
struct plot_value * plot_func_pair_assq(struct plot_env *env, struct plot_value *args){
    plot_value *func;
    plot_value *res;

    if( args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 args", "plot_func_pair_assq");
    }

    if( cdr(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 args", "plot_func_pair_assq");
    }

    if( cdr(cdr(args))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 args", "plot_func_pair_assq");
    }

    func = plot_new_form(plot_func_eq_test, 0);
    cdr(cdr(args)) = cons(func, null);

    res = plot_func_pair_assoc(env, args);

    plot_value_decr(cdr(cdr(args)));
    cdr(cdr(args)) = null;

    return res;
}

/* (assv obj alist)
 *
 * alist is an association list (list of pairs)
 *
 * assv finds the first set of pairs of car is obj and returns that pair
 * otherwise #f (if no pair has obj as car)
 *
 * compares using `eqv?`
 */
struct plot_value * plot_func_pair_assv(struct plot_env *env, struct plot_value *args){
    plot_value *func;
    plot_value *res;

    if( args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 args", "plot_func_pair_assv");
    }

    if( cdr(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 args", "plot_func_pair_assv");
    }

    if( cdr(cdr(args))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 args", "plot_func_pair_assv");
    }

    func = plot_new_form(plot_func_eqv_test, 0);
    cdr(cdr(args)) = cons(func, null);

    res = plot_func_pair_assoc(env, args);

    plot_value_decr(cdr(cdr(args)));
    cdr(cdr(args)) = null;

    return res;
}

/* (assoc obj alist)
 * (assoc obj alist compare)
 *
 * alist is an association list (list of pairs)
 *
 * assoc finds the first set of pairs of car is obj and returns that pair
 * otherwise #f (if no pair has obj as car)
 *
 * compares using provided `compare` proc, otherwise uses `equal?`
 */
struct plot_value * plot_func_pair_assoc(struct plot_env *env, struct plot_value *args){
    /* current pos in list */
    plot_value *cur;
    /* func given or default */
    plot_value *func;
    /* result from a call */
    plot_value *res;
    /* arg to a call */
    plot_value *arg;
    /* my return value */
    plot_value *ret;

    if( args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected either 2 or 3 arguments", "plot_func_pair_member");
    }

    /* temporarily re-using arg */
    arg = cdr(args);

    if( arg->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected either 2 or 3 arguments, got 1", "plot_func_pair_member");
    }

    /* temporarily re-using arg */
    arg = cdr(arg);

    if( arg->type == plot_type_null ){
        /* default to `equal?` */
        func = plot_new_form(plot_func_equal_test, 0);
    } else if( arg->type == plot_type_pair ){
        /* then car(arg) is our function */
        func = car(arg);

        /* first incr is because we later decr it
         * we must decr it as in the default case of using `equal?`
         * plot_new_form will return a value with refcount of 1
         */
        plot_value_incr(func);

        /* check we received no more than 2 args */
        /* temporarily re-using arg */
        arg = cdr(arg);
        if( arg->type != plot_type_null ){
            return plot_runtime_error(plot_error_bad_args, "expected no more than 2 args", "plot_func_pair_member");
        }
    } else {
        return plot_runtime_error(plot_error_internal, "Error in argument passing", "plot_func_pair_member");
    }

    ret = plot_new_boolean(0);
    arg = cons(0, cons(0, null));
    /* supplied obj to compare to, inserting into later position to make for loop cheaper */
    car(cdr(arg)) = car(args);

    for( cur = car(cdr(args)); cur->type == plot_type_pair; cur = cdr(cur) ){
        /* object in list to compare
         * as this is an associative list we want
         * to compare against the 'key' (car)
         */
        car(arg) = car(car(cur));

        /* decr of res is handled by truthy */
        res = plot_eval_apply(env, func, arg);
        if( plot_truthy(res) ){
            plot_value_decr(ret);
            ret = car(cur);
            plot_value_incr(ret);
            break;
        }
    }

    /* avoid collecting any of the elements in the list */
    car(cdr(arg)) = 0;
    car(cdr(cdr(arg))) = 0;

    /* func only has a ref count of 1 so it gced for us by decr or arg */
    plot_value_decr(arg);
    return ret;
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


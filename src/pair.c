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

    if( args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "args was not of type plot_type_pair", "plot_func_pair_set_car");
    }

    /* args should be
     * ( pair ( obj null ) )
     */

    if( car(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not a pair", "plot_func_pair_set_car");
    }

    if( cdr(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "only got 1 argument, expected exactly 2 args", "plot_func_pair_set_car");
    }

    if( cdr(cdr(args))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "got too many arguments, expected exactly 2 args", "plot_func_pair_set_car");
    }

    /* grab our new value */
    new = car(cdr(args));

    /* decr our old value */
    plot_value_decr(car(car(args)));

    /* store and incr our new value */
    lcar(car(args)) = new;
    plot_value_incr(new);

    return plot_new_unspecified();
}

/* (set-cdr! pair obj)
 */
struct plot_value * plot_func_pair_set_cdr(struct plot_env *env, struct plot_value *args){
    plot_value *new;

    if( args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "args was not of type plot_type_pair", "plot_func_pair_set_cdr");
    }

    /* args should be
     * ( pair ( obj null ) )
     */
    if( car(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not a pair", "plot_func_pair_set_cdr");
    }

    if( cdr(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "only got 1 argument, expected exactly 2 args", "plot_func_pair_set_cdr");
    }

    if( cdr(cdr(args))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "got too many arguments, expected exactly 2 args", "plot_func_pair_set_cdr");
    }

    /* grab our new value */
    new = car(cdr(args));

    /* decr our old value */
    plot_value_decr(cdr(car(args)));

    /* store and incr our new value */
    lcdr(car(args)) = new;
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
        lcar(*cur) = car(in);
        plot_value_incr(car(in));
        cur = &lcdr(*cur);
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
 * as you can see a non-list as the last argument is fine
 * (append '(1 2 3) '(4) 5) ; => '(1 2 3 4 5)
 *
 * however a non-list as any non-last argument will throw an error
 * (append '(1 2 3) 4 5)    ; => the 4 will raise an error
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
        /* check the current argument is a list OR is the last argument:
         *
         *  - first we check if the cdr is null, if this is true
         *      then this arg is the last and so a non-list argument is allowed
         *  - if this arg is not the last argument then it must
         *  - be either a list (so either a pair of a null)
         */
        if( cdr(arg)->type != plot_type_null && /* check this is not the last arg*/
            car(arg)->type != plot_type_pair && /* check this arg is not a pair */
            car(arg)->type != plot_type_null ){ /* check this arg is not a null */
            /* if we are inside here it means that we found an argument
             * that was not the last
             * and is not a list (pair or null)
             * this violates r7rs section 6.4 page 42
             */

            /* reusing head for error value */
            head = plot_runtime_error(plot_error_bad_args, "TEMPORARY", "plot_func_pair_append");

            /* be a bit more verbose about error */
            puts("append called with args: ");
            display_error_expr(args);
            puts("found a non-last argument that was not a list: ");
            display_error_expr(car(arg));

            /* finally return the error object */
            return head;
        }

        /* keep iterating through args until we see a non-pair */
        for( elem = car(arg); elem->type == plot_type_pair; elem = cdr(elem) ){
            *outcur = cons(0, null);
            lcar(*outcur) = car(elem);
            plot_value_incr(car(elem));
            outcur = &lcdr(*outcur);
        }

        /* since we are here the above for has ended which means our arg is not a pair
         * if our arg is a null we skip over it
         * otherwise we want to keep it
         */
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

    /* args should be
     * ( list number )
     */
    if( args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "internal error, arguments not provided as a list", "plot_func_pair_list_ref");
    }

    if( car(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "first argument was not a pair", "plot_func_pair_list_ref");
    }

    if( cdr(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "only 1 argument provided, expected exactly 2", "plot_func_pair_list_ref");
    }

    if( car(cdr(args))->type != plot_type_number ){
        return plot_runtime_error(plot_error_bad_args, "second argument was not of type number", "plot_func_pair_list_ref");
    }

    if( cdr(cdr(args))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "received more than 2 arguments", "plot_func_pair_list_ref");
    }

    /* get 2nd arg as number
     * this is our index into the list
     * */
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
     * cur has to be a pair due to the check at the end of the for loop above
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

    /* args should be
     * ( list number obj )
     */
    if( args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "internal error, arguments not provided as a list", "plot_func_pair_list_set");
    }

    if( car(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "first argument was not a pair", "plot_func_pair_list_set");
    }

    if( cdr(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "only 1 argument provided, expected exactly 3", "plot_func_pair_list_set");
    }

    if( car(cdr(args))->type != plot_type_number ){
        return plot_runtime_error(plot_error_bad_args, "second argument was not of type number", "plot_func_pair_list_set");
    }

    if( cdr(cdr(args))->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "only 2 arguments provided, expected exactly 3", "plot_func_pair_list_set");
    }

    if( cdr(cdr(cdr(args)))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "received more than 3 arguments", "plot_func_pair_list_set");
    }


    /* get 2nd arg as number
     * this is our index into the list
     * */
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
     * cur has to be a pair due to test at end of for loop above
     * args has to be a pair due to args checking above
     */
    plot_value_decr(car(cur));
    lcar(cur) = car(cdr(cdr(args)));
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
    lcdr(cdr(args)) = cons(func, null);

    res = plot_func_pair_member(env, args);

    plot_value_decr(cdr(cdr(args)));
    lcdr(cdr(args)) = null;

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
    lcdr(cdr(args)) = cons(func, null);

    res = plot_func_pair_member(env, args);

    plot_value_decr(cdr(cdr(args)));
    lcdr(cdr(args)) = null;

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
    lcar(cdr(arg)) = car(args);

    for( cur = car(cdr(args)); cur->type == plot_type_pair; cur = cdr(cur) ){
        /* object in list to compare */
        lcar(arg) = car(cur);

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
    lcar(cdr(arg)) = 0;
    lcar(cdr(cdr(arg))) = 0;

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
    lcdr(cdr(args)) = cons(func, null);

    res = plot_func_pair_assoc(env, args);

    plot_value_decr(cdr(cdr(args)));
    lcdr(cdr(args)) = null;

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
    lcdr(cdr(args)) = cons(func, null);

    res = plot_func_pair_assoc(env, args);

    plot_value_decr(cdr(cdr(args)));
    lcdr(cdr(args)) = null;

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
    lcar(cdr(arg)) = car(args);

    for( cur = car(cdr(args)); cur->type == plot_type_pair; cur = cdr(cur) ){
        /* object in list to compare
         * as this is an associative list we want
         * to compare against the 'key' (car)
         */
        lcar(arg) = car(car(cur));

        res = plot_eval_apply(env, func, arg);
        if( plot_truthy(res) ){
            plot_value_decr(ret);
            ret = car(cur);
            plot_value_incr(ret);
            break;
        }
        /* plot_truthy no longer decrs */
        plot_value_decr(res);
    }

    /* avoid collecting any of the elements in the list */
    lcar(cdr(arg)) = 0;
    lcar(cdr(cdr(arg))) = 0;

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
        lcar(*cur) = car(old);
        plot_value_incr(car(old));
        cur = &lcdr(*cur);
    }

    return head;
}


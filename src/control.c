#include "control.h"
#include "value.h"
#include "plot.h"
#include "pair.h"
#include "eval.h"
#include "hash.h"
#include "env.h"

#define DEBUG 0

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

/* (procedure? obj)
 * FIXME should generalise if spec allows
 */
struct plot_value * plot_func_control_procedure_test(struct plot_env *env, struct plot_value *args){
    plot_value *val;

    #if DEBUG
    puts("inside plot_func_procedure_test");
    #endif

    val = car(args);

    if( ! val ){
        return plot_runtime_error(plot_error_bad_args, "first arg was null", "plot_func_procedure_test");
    }

    return plot_new_boolean( val->type == plot_type_lambda || val->type == plot_type_form  );
}

/* (apply proc args1 ... args)
 *   calls proc with the elements of the list
 *   (append (list arg1...) args) as the argument
 *
 * allows for both:
 *  (apply + '(3 4))   ; => 7
 *  (apply + 3 '(1 3)) ; => 7
 */
struct plot_value * plot_func_control_apply(struct plot_env *env, struct plot_value *args){
    /* function to apply */
    plot_value *func;
    /* newargs is our argument list (append (list arg1 ...) args) */
    plot_value *newargs;
    /* curna is a cursor into new args */
    plot_value **curna;
    /* arg is a cursor into args */
    plot_value *arg;

    /* new env for our lambda */
    plot_env *new_env;

    /* return value */
    plot_value *ret;

    /* r7rs spec says the argument should be
     * (append (list arg1...) args)
     *
     * this allows for
     *  (apply + 1 2 '(3 4 5)) ; =>15
     * but not
     *  (apply + 1 2) ; => error
     * nor
     *  (apply '(1 2 3) '(4 5 6)) ; => error
     */
    newargs = null;
    curna = &newargs;

    /* we only loop up to the 2nd to last pair (inclusive)
     * we want to save the last pair
     */
    for( arg = cdr(args); cdr(arg)->type == plot_type_pair; arg = cdr(arg) ){
        if( car(arg)->type == plot_type_pair ){
            return plot_runtime_error(plot_error_bad_args, "only the final arg to apply can be a list", "plot_func_control_apply");
        }
        *curna = cons(0, null);
        car(*curna) = car(arg);
        plot_value_incr(car(*curna));
        curna = &cdr(*curna);
    }

    newargs = plot_func_pair_append(env, cons(plot_func_pair_list(env, newargs), arg));

    func = car(args);

    switch(func->type){
        case plot_type_lambda:
            /* create new env for our lambda */
            new_env = plot_alloc_env(func->u.lambda.env);
            if( ! new_env ){
                plot_fatal_error("failed to allocate new env in apply");
            }

            /* bind arguments
             * for each parameter:
             *  grab an argument and bind
             * if no argument then error
             * if left over arguments then error
             */

            /* use arg as cursor into params (car of lambda body)
             * use curna as cursor into newargs
             */
            curna = &newargs;
            for( arg = car(func->u.lambda.body); arg->type != plot_type_null; arg = cdr(arg) ){

                /* too few args for lambda */
                if( (*curna)->type == plot_type_null ){
                    return plot_runtime_error(plot_error_runtime, "too few args supplied for lambda", "plot_func_control_apply");
                }

                if( car(arg)->type == plot_type_null ){
                    plot_fatal_error("apply: encountered null parameter");
                }

                if( ! plot_env_define(new_env, &(car(arg)->u.symbol), car(*curna)) ){
                    plot_fatal_error("apply: failed to define argument");
                }

                *curna = cdr(*curna);
            }

            /* too many args for lambda */
            if( (*curna)->type != plot_type_null ){
                return plot_runtime_error(plot_error_runtime, "too many args supplied for lambda", "plot_func_control_apply");
            }

            /* eval each part of body in new_env
             * return value of final expr
             * gc all intermediary exprs
             * check for intermediary errors
             */
            ret = 0;
            for( arg = cdr(func->u.lambda.body); arg->type != plot_type_null; arg = cdr(arg) ){
                if( ret ){
                    /* gc intermedary value */
                    plot_value_decr(ret);
                }
                ret = plot_eval_expr(new_env, car(arg));
                if( ret && ret->type == plot_type_error ){
                    puts("apply (lambda body)");
                    return ret;
                }
            }

            /* tidy up env */
            plot_env_decr(new_env);

            break;
        case plot_type_form:
            ret = func->u.form.func(env, newargs);
            break;
        default:
            return plot_runtime_error(plot_error_bad_args, "first argument is not a function", "plot_func_control_apply");
            break;
    }

    /* gc temporary resources
     * first 0 out cars as the cons does not incr
     * (and we dont want to accidentally decr
     */
    for( arg = newargs; arg->type != plot_type_null; arg = cdr(arg) ){
        car(arg) = 0;
    }
    plot_value_decr(newargs);

    return ret;
}

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
struct plot_value * plot_func_control_map(struct plot_env *env, struct plot_value *args){
    /* pair containing our proc as the car and targs as it's cdr */
    plot_value *proc;

    /* cursor through various plot lists */
    plot_value *cur;

    /* current step through args, and cursor within that step
     * args after the car is an array of arrays which we step through
     * each 'step' we are looking at one value in each array
     *
     * '(1 2) '(3 4) '(5 6) would take 2 steps:
     *  first step is over '(1 3 5)
     *  second step is over (2 4 6)
     *
     * stepcur is ** as we need to write through it
     */
    plot_value *step = 0;
    plot_value **stepcur = &step;

    /* temporary arguments we build and call the supplied procedure with
     * and cursor into it
     *
     * targscur is ** as we need to write through it
     */
    plot_value *targs = 0;
    plot_value **targscur = &targs;

    /* result list and cursor into it
     * rescur is ** as we need to write through it
     */
    plot_value *res = 0;
    plot_value **rescur = &res;


    /* check procedure argument */
    if( !args || args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "did not receive any arguments", "plot_func_control_map");
    }

    proc = car(args);
    if( proc->type != plot_type_lambda && proc->type != plot_type_form ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not a procedure", "plot_func_control_map");
    }

    /* we want proc to be passable to apply
     * the last argument of apply must be a list so we have to have
     * an extra cons as our cd
     */
    proc = cons( proc, cons(null, null) );


    /* copy over initial lists into step
     * this also allocates our pairs that we later use for each 'step'
     */
    for( cur = cdr(args); ; cur = cdr(cur) ){
        switch( cur->type ){
            case plot_type_pair:
                /* allocate and copy over supplied lists */
                *stepcur = cons( car(cur), null );
                stepcur = &cdr(*stepcur);

                /* allocate temporary arguments to ensure sufficient length */
                *targscur = cons( null, null );
                targscur = &cdr(*targscur);

                break;
            case plot_type_null:
                /* null means end of args */
                goto ARGFIN;
                break;
            default:
                return plot_runtime_error(plot_error_bad_args, "arg was not of correct type (expected either pair or null)", "plot_func_control_map");
                break;
        }
    }

    /* exit point for arg copying list */
ARGFIN:

    /* no args found, error */
    if( step == 0 ){
        return plot_runtime_error(plot_error_bad_args, "expected at least 1 list", "plot_func_control_map");
    }

    /* last arg of apply must be a list of arguments
     * so we must wrap targs in another cons
     */
    car(cdr(proc)) = targs;

    /* main loop */
    while( 1 ){
        targscur = &targs;
        stepcur = &step;


        for( cur = step; cur->type == plot_type_pair; cur = cdr(cur) ){
            switch( car(cur)->type ){
                case plot_type_pair:
                    break;
                case plot_type_null:
                    /* null means end of one list
                     * so end of our map
                     */
                    goto LISTFIN;
                    break;
                default:
                    return plot_runtime_error(plot_error_bad_args, "found an improper list", "plot_func_control_map");
                    break;
            }

            /* copy over into temporary args */
            car(*targscur) = car(car(cur));
            targscur = &cdr(*targscur);

            /* advance step */
            car(*stepcur) = cdr(car(cur));
            stepcur = &cdr(*stepcur);
        }


        /* allocate space for our result */
        *rescur = cons( null, null );

        /* call our function
         * here `proc` is a pair of (proc, args)
         */
        car(*rescur) = plot_func_control_apply(env, proc);

        if( ! car(*rescur) || car(*rescur)->type == plot_type_error ){
            puts("plot_func_control_map");
            return car(*rescur);
        }

        rescur = &cdr(*rescur);
    }

    /* exit point for main loop */
LISTFIN:

    /* zero-out targs
     * NB: cons does not incr, so we have to 0 out to avoid decr on the copied values
     */
    for( cur = targs; cur->type == plot_type_pair; cur = cdr(cur) ){
        car(cur) = 0;
    }

    /* zero-out car of proc to prevent gc of procedure */
    car(proc) = 0;

    /* gc conses used in proc and targs */
    plot_value_decr(proc);

    if( res == 0 ){
        /* no results, return empty list */
        res = null;
    }

    return res;
}

/* (string-map proc string1 string2 ...)
 * it is an error if proc does not take as many arguments as there are strings and return a single character
 *
 * string-map applies proc element-wise to the elements of the strings and returns a string of the results, in order.
 * (string-map char-foldcase "AbdEgH") ;; => "abdegh"
 */
struct plot_value * plot_func_control_string_map(struct plot_env *env, struct plot_value *args){
    /* pair containing our proc as the car and targs as it's cdr */
    plot_value *proc;

    /* cursor through various plot lists */
    plot_value *cur;

    /* temporary arguments we build and call the supplied procedure with
     * and cursor into it
     *
     * targscur is ** as we need to write through it
     */
    plot_value *targs = 0;
    plot_value **targscur = &targs;

    /* result list and cursor into it
     * rescur is ** as we need to write through it
     */
    plot_value *res = 0;
    plot_value **rescur = &res;

    /* return value from a call to proc */
    plot_value *tmp;

    /* index into strings */
    int i = 0;

    /* number of result chars, also size of final string (excl. null) */
    int len = 0;


    /* check procedure argument */
    if( !args || args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "did not receive any arguments", "plot_func_control_string_map");
    }

    proc = car(args);
    if( proc->type != plot_type_lambda && proc->type != plot_type_form ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not a procedure", "plot_func_control_string_map");
    }

    /* we want proc to be passable to apply
     * the last argument of apply must be a list so we have to have
     * an extra cons as our cd
     */
    proc = cons( proc, cons(null, null) );


    /* copy over initial lists into step
     * this also allocates our pairs that we later use for each 'step'
     */
    for( cur = cdr(args); ; cur = cdr(cur) ){
        switch( cur->type ){
            case plot_type_pair:
                /* allocate temporary arguments to ensure sufficient length */
                *targscur = cons( null, null );
                car(*targscur) = plot_new_character(0);
                targscur = &cdr(*targscur);

                break;
            case plot_type_null:
                /* null means end of args */
                goto ARGFIN;
                break;
            default:
                return plot_runtime_error(plot_error_bad_args, "arg was not of correct type (expected either pair or null)", "plot_func_control_string_map");
                break;
        }
    }

    /* exit point for arg copying list */
ARGFIN:

    /* no args found, error */
    if( targs == 0 ){
        return plot_runtime_error(plot_error_bad_args, "expected at least 1 list", "plot_func_control_map");
    }

    /* last arg of apply must be a list of arguments
     * so we must wrap targs in another cons
     */
    car(cdr(proc)) = targs;

    /* main loop */
    while( 1 ){
        targscur = &targs;

        for( cur = cdr(args); cur->type == plot_type_pair; cur = cdr(cur) ){
            if( car(cur)->type != plot_type_string ){
                return plot_runtime_error(plot_error_bad_args, "found unexpected type, expected string", "plot_func_control_string_map");
            }

            /* check i is inside string */
            if( i >= car(cur)->u.string.len - 1 ){
                /* end of one string reached, abort */
                goto LISTFIN; /* FIXME off by 1? */
            }

            /* copy over into temporary args */
            car(*targscur)->u.character.val = car(cur)->u.string.val[i];
            //car(*targscur) = car(car(cur));
            targscur = &cdr(*targscur);
        }

        /* advance step */
        ++ i;

        /* increment len */
        ++ len;

        /* allocate space for our result */
        *rescur = cons( plot_new_character(0), null );

        /* call our function
         * here `proc` is a pair of (proc, args)
         */
        tmp = plot_func_control_apply(env, proc);

        if( ! tmp || tmp->type == plot_type_error ){
            puts("plot_func_control_map");
            return tmp;
        }

        if( tmp->type != plot_type_character ){
            return plot_runtime_error(plot_error_bad_args, "function returned unexpected type, expected character", "plot_func_control_string_map");
        }

        car(*rescur)->u.character.val = tmp->u.character.val;
        plot_value_decr(tmp);

        rescur = &cdr(*rescur);
    }

    /* exit point for main loop */
LISTFIN:

    /* do not zero out args as we also want to collect the characters
     * that are stored in the cars
     */

    /* zero-out car of proc to prevent gc of procedure */
    car(proc) = 0;

    /* gc conses used in proc and targs */
    plot_value_decr(proc);

    if( res == 0 ){
        /* no results, return empty string */
        tmp = plot_new_string("", 1); /* FIXME is this a valid empty string? */
    } else {
        /* squash characters into string */

        /* + 1 for null terminator */
        tmp = plot_new_string( plot_alloc_string(len + 1), len + 1 );

        /* copy over characters */
        i = 0;
        for( rescur = &res; (*rescur)->type == plot_type_pair; rescur = &cdr(*rescur) ){
            tmp->u.string.val[i++] = (car(*rescur))->u.character.val;
        }
        tmp->u.string.val[i] = '\0';
    }

    /* gc res */
    plot_value_decr(res);

    return tmp;
}

/* (for-each proc list1 list2... )
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
struct plot_value * plot_func_control_for_each(struct plot_env *env, struct plot_value *args){
    /* pair containing our proc as the car and targs as it's cdr */
    plot_value *proc;

    /* cursor through various plot lists */
    plot_value *cur;

    /* current step through args, and cursor within that step
     * args after the car is an array of arrays which we step through
     * each 'step' we are looking at one value in each array
     *
     * '(1 2) '(3 4) '(5 6) would take 2 steps:
     *  first step is over '(1 3 5)
     *  second step is over (2 4 6)
     *
     * stepcur is ** as we need to write through it
     */
    plot_value *step = 0;
    plot_value **stepcur = &step;

    /* temporary arguments we build and call the supplied procedure with
     * and cursor into it
     *
     * targscur is ** as we need to write through it
     */
    plot_value *targs = 0;
    plot_value **targscur = &targs;

    /* used for return values for proc called
     */
    plot_value *tmp;

    /* check procedure argument */
    if( !args || args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "did not receive any arguments", "plot_func_control_for_each");
    }

    proc = car(args);
    if( proc->type != plot_type_lambda && proc->type != plot_type_form ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not a procedure", "plot_func_control_for_each");
    }

    /* we want proc to be passable to apply
     * the last argument of apply must be a list so we have to have
     * an extra cons as our cd
     */
    proc = cons( proc, cons(null, null) );


    /* copy over initial lists into step
     * this also allocates our pairs that we later use for each 'step'
     */
    for( cur = cdr(args); ; cur = cdr(cur) ){
        switch( cur->type ){
            case plot_type_pair:
                /* allocate and copy over supplied lists */
                *stepcur = cons( car(cur), null );
                stepcur = &cdr(*stepcur);

                /* allocate temporary arguments to ensure sufficient length */
                *targscur = cons( null, null );
                targscur = &cdr(*targscur);

                break;
            case plot_type_null:
                /* null means end of args */
                goto ARGFIN;
                break;
            default:
                return plot_runtime_error(plot_error_bad_args, "arg was not of correct type (expected either pair or null)", "plot_func_control_for_each");
                break;
        }
    }

    /* exit point for arg copying list */
ARGFIN:

    /* no args found, error */
    if( step == 0 ){
        return plot_runtime_error(plot_error_bad_args, "expected at least 1 list", "plot_func_control_for_each");
    }

    /* last arg of apply must be a list of arguments
     * so we must wrap targs in another cons
     */
    car(cdr(proc)) = targs;

    /* main loop */
    while( 1 ){
        targscur = &targs;
        stepcur = &step;


        for( cur = step; cur->type == plot_type_pair; cur = cdr(cur) ){
            switch( car(cur)->type ){
                case plot_type_pair:
                    break;
                case plot_type_null:
                    /* null means end of one list
                     * so end of our map
                     */
                    goto LISTFIN;
                    break;
                default:
                    return plot_runtime_error(plot_error_bad_args, "found an improper list", "plot_func_control_for_each");
                    break;
            }

            /* copy over into temporary args */
            car(*targscur) = car(car(cur));
            targscur = &cdr(*targscur);

            /* advance step */
            car(*stepcur) = cdr(car(cur));
            stepcur = &cdr(*stepcur);
        }

        /* call our function
         * here `proc` is a pair of (proc, args)
         */
        tmp = plot_func_control_apply(env, proc);

        if( ! tmp || tmp->type == plot_type_error ){
            puts("plot_func_control_for_each");
            return tmp;
        }

        plot_value_decr(tmp);
    }

    /* exit point for main loop */
LISTFIN:

    /* zero-out targs
     * NB: cons does not incr, so we have to 0 out to avoid decr on the copied values
     */
    for( cur = targs; cur->type == plot_type_pair; cur = cdr(cur) ){
        car(cur) = 0;
    }

    /* zero-out car of proc to prevent gc of procedure */
    car(proc) = 0;

    /* gc conses used in proc and targs */
    plot_value_decr(proc);

    return plot_new_unspecified();
}

/* (string-for-each proc string1 string2 ...)
 * it is an error if proc does not take as may argumnets as there are string and return a single character
 *
 * string-for-each applies proc element wise to the elements of the strings and returns a string of the results in order.
 *
 * return value is unspecified
 */
struct plot_value * plot_func_control_string_for_each(struct plot_env *env, struct plot_value *args){
    /* pair containing our proc as the car and targs as it's cdr */
    plot_value *proc;

    /* cursor through various plot lists */
    plot_value *cur;

    /* temporary arguments we build and call the supplied procedure with
     * and cursor into it
     *
     * targscur is ** as we need to write through it
     */
    plot_value *targs = 0;
    plot_value **targscur = &targs;

    /* return value from a call to proc */
    plot_value *tmp;

    /* index into strings */
    int i = 0;

    /* number of result chars, also size of final string (excl. null) */
    int len = 0;


    /* check procedure argument */
    if( !args || args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "did not receive any arguments", "plot_func_control_string_map");
    }

    proc = car(args);
    if( proc->type != plot_type_lambda && proc->type != plot_type_form ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not a procedure", "plot_func_control_string_map");
    }

    /* we want proc to be passable to apply
     * the last argument of apply must be a list so we have to have
     * an extra cons as our cd
     */
    proc = cons( proc, cons(null, null) );


    /* copy over initial lists into step
     * this also allocates our pairs that we later use for each 'step'
     */
    for( cur = cdr(args); ; cur = cdr(cur) ){
        switch( cur->type ){
            case plot_type_pair:
                /* allocate temporary arguments to ensure sufficient length */
                *targscur = cons( null, null );
                car(*targscur) = plot_new_character(0);
                targscur = &cdr(*targscur);

                break;
            case plot_type_null:
                /* null means end of args */
                goto ARGFIN;
                break;
            default:
                return plot_runtime_error(plot_error_bad_args, "arg was not of correct type (expected either pair or null)", "plot_func_control_string_map");
                break;
        }
    }

    /* exit point for arg copying list */
ARGFIN:

    /* no args found, error */
    if( targs == 0 ){
        return plot_runtime_error(plot_error_bad_args, "expected at least 1 list", "plot_func_control_map");
    }

    /* last arg of apply must be a list of arguments
     * so we must wrap targs in another cons
     */
    car(cdr(proc)) = targs;

    /* main loop */
    while( 1 ){
        targscur = &targs;

        for( cur = cdr(args); cur->type == plot_type_pair; cur = cdr(cur) ){
            if( car(cur)->type != plot_type_string ){
                return plot_runtime_error(plot_error_bad_args, "found unexpected type, expected string", "plot_func_control_string_map");
            }

            /* check i is inside string */
            if( i >= car(cur)->u.string.len - 1 ){
                /* end of one string reached, abort */
                goto LISTFIN; /* FIXME off by 1? */
            }

            /* copy over into temporary args */
            car(*targscur)->u.character.val = car(cur)->u.string.val[i];
            //car(*targscur) = car(car(cur));
            targscur = &cdr(*targscur);
        }

        /* advance step */
        ++ i;

        /* increment len */
        ++ len;

        /* call our function
         * here `proc` is a pair of (proc, args)
         */
        tmp = plot_func_control_apply(env, proc);

        if( ! tmp || tmp->type == plot_type_error ){
            puts("plot_func_control_map");
            return tmp;
        }

        /* for-each ignores return values, so just decr */
        plot_value_decr(tmp);
    }

    /* exit point for main loop */
LISTFIN:

    /* do not zero out args as we also want to collect the characters
     * that are stored in the cars
     */

    /* zero-out car of proc to prevent gc of procedure */
    car(proc) = 0;

    /* gc conses used in proc and targs */
    plot_value_decr(proc);

    return plot_new_unspecified();
}



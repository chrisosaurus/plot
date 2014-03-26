#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "value.h"
#include "hash.h"
#include "env.h"
#include "funcs.h"
#include "eval.h"
#include "plot.h"

#define DEBUG 0

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

/********* equivalent predicates *********/

/* (equal? obj1 obj2)
 * in plot equal?, eqv? and eq? are equivalent.
 * FIXME should generalise
 */
struct plot_value * plot_func_equal_test(struct plot_env *env, struct plot_value *args){
    plot_value *o1, *o2;
    plot_value *t;
    int r;

    #if DEBUG
    puts("inside plot_func_equal");
    #endif

    if( args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 args", "plot_func_equal_test");
    }
    o1 = car(args);

    if( ! o1 ){
        return plot_runtime_error(plot_error_bad_args, "first arg was null", "plot_func_equal_test");
    }

    if( cdr(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_pair", "plot_func_equal_test");
    }

    o2 = car(cdr(args));

    if( ! o2 ){
        return plot_runtime_error(plot_error_bad_args, "second arg was null", "plot_func_equal_test");
    }

    if( o1->type != o2->type ){
        #if DEBUG
        puts("\targs were not of the same type");
        #endif
        return plot_new_boolean(false);
    }

    switch( o1->type ){
        case plot_type_eof:
            return plot_new_boolean(1);
            break;
        case plot_type_textual_port:
            /* FIXME */
            return plot_runtime_error(plot_error_unimplemented, "textual port equality is not yet implemented", "plot_func_equal_test");
            break;
        case plot_type_number:
            return plot_new_boolean( o1->u.number.val == o2->u.number.val );
            break;
        case plot_type_symbol:
            return plot_new_boolean( ! strcmp( o1->u.symbol.val, o2->u.symbol.val ) );
            break;
        case plot_type_lambda:
            return plot_new_boolean(
                o1->u.lambda.env == o2->u.lambda.env
                &&
                o1->u.lambda.body == o2->u.lambda.body
            );
            break;
        case plot_type_form:
            return plot_new_boolean( o1->u.form.func == o2->u.form.func );
            break;
        case plot_type_null:
            /* both arguments are of the same type, '() = '() */
            return plot_new_boolean(true);
            break;
        case plot_type_pair:
            /* check the cars of o1 and o2 are equal */
            t = cons( car(o1), cons( car(o2), null ) );
            r = plot_truthy(plot_func_equal_test(env, t));

            /* if they are, also check the cdrs are equal */
            if( r ){
                car(t) = cdr(o1);
                car(cdr(t)) = cdr(o2);
                r = plot_truthy(plot_func_equal_test(env, t));
            }

            /* make sure to clean up
             * NB: cons doesn't incr (wrapper for plot_new_pair, which doesn't incr)
             *     so we have to 0 car and cdr to prevent accidental decr of our stored values
             */
            car(t) = 0;
            car(cdr(t)) = 0;
            plot_value_decr(t);

            return plot_new_boolean(r);
            break;
        case plot_type_error:
            plot_fatal_error("plot_func_equal: saw plot_type_error");
            break;
        case plot_type_string:
            return plot_new_boolean( ! strcmp( o1->u.string.val, o2->u.string.val ) );
            break;
        case plot_type_boolean:
            return plot_new_boolean( o1->u.boolean.val == o2->u.boolean.val );
            break;
        case plot_type_character:
            return plot_new_boolean( o1->u.character.val == o2->u.boolean.val );
            break;
        case plot_type_reclaimed:
            plot_fatal_error("plot_func_equal: saw plot_type_reclaimed");
            break;
        default:
            plot_fatal_error("plot_func_equal: impossible o1->type");
            break;
    }

    plot_fatal_error("plot_func_equal: out of switch, impossible args[0]->type");
    return 0; /* IMPOSSIBLE as plot_fatal_error does not return */
}

/* (eqv? obj1 obj2)
 * in plot equal?, eqv? and eq? are equivalent.
 * FIXME need to generalise
 */
struct plot_value * plot_func_eqv_test(struct plot_env *env, struct plot_value *args){
    return plot_func_equal_test(env, args);
}

/* (eq? obj1 obj2)
 * in plot equal?, eqv? and eq? are equivalent.
 * FIXME need to generalise
 */
struct plot_value * plot_func_eq_test(struct plot_env *env, struct plot_value *args){
    return plot_func_equal_test(env, args);
}


/********* value testing functions ********/

/* (boolean? obj)
 * FIXME should generalise if spec allows
 */
struct plot_value * plot_func_boolean_test(struct plot_env *env, struct plot_value *args){
    plot_value *val;

    #if DEBUG
    puts("inside plot_func_boolean_test");
    #endif

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_boolean_test");
    }

    val = car(args);

    if( ! val ){
        return plot_runtime_error(plot_error_bad_args, "first arg was null", "plot_func_boolean_test");
    }

    return plot_new_boolean( val->type == plot_type_boolean );
}

/* (symbol? obj)
 * FIXME should generalise if spec allows
 */
struct plot_value * plot_func_symbol_test(struct plot_env *env, struct plot_value *args){
    plot_value *val;

    #if DEBUG
    puts("inside plot_func_symbol_test");
    #endif

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_symbol_test");

    }

    val = car(args);

    if( ! val ){
        return plot_runtime_error(plot_error_bad_args, "first arg was null", "plot_func_symbol_test");
    }

    return plot_new_boolean( val->type == plot_type_symbol );
}

/* (symbol=? obj1 obj2)
 * returns #t iff obj1 and obj2 are both symbols and are considered equal
 * otherwise returns #f
 */
struct plot_value * plot_func_symbol_equal_test(struct plot_env *env, struct plot_value *args){
    plot_value *o1, *o2;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_pair || cdr(cdr(args))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 args", "plot_func_symbol_equal_test");
    }

    o1 = car(args);
    if( o1->type != plot_type_symbol ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_symbol", "plot_func_symbol_equal_test");
    }

    o2 = car(cdr(args));
    if( o2->type != plot_type_symbol ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_symbol", "plot_func_symbol_equal_test");
    }

    return plot_new_boolean( ! strcmp( o1->u.symbol.val, o2->u.symbol.val ));
}

/* returns 1 if value is considered truthy
 * returns 0 if falsy
 *
 * does not modify value nor value's refcount
 */
int plot_truthy(plot_value *val){
    int ret = 1;
    if( ! val )
        return 0;

    if( val->type == plot_type_boolean && val->u.boolean.val == false )
        ret =  0;

    return ret;
}

/* (and obj1 obj2 ...)
 * logical and of all arguments
 */
struct plot_value * plot_func_and(struct plot_env *env, struct plot_value *args){
    plot_value *cur;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "insufficient arguments, expected at least 2", "plot_func_add");
    }

    for( cur = args; cur->type == plot_type_pair; cur = cdr(cur) ){
        if( ! plot_truthy(car(cur)) ){
            return plot_new_boolean(false);
        }
    }

    return plot_new_boolean(true);
}

/* (or obj1 obj2 ...)
 * logical or of all arguments
 */
struct plot_value * plot_func_or(struct plot_env *env, struct plot_value *args){
    plot_value *cur;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "insufficient arguments, expected at least 2", "plot_func_or");
    }

    for( cur = args; cur->type == plot_type_pair; cur = cdr(cur) ){
        if( plot_truthy(car(cur)) ){
            return plot_new_boolean(true);
        }
    }

    return plot_new_boolean(false);
}

/* (not obj)
 * logical not of single argument
 */
struct plot_value * plot_func_not(struct plot_env *env, struct plot_value *args){
    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "incorrect arguments: expected exactly 1", "plot_func_not");
    }

    return plot_new_boolean( ! plot_truthy(car(args)) );
}

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
struct plot_value * plot_func_exit(struct plot_env *env, struct plot_value *args){
    /* return required to keep compiler happy */
    return plot_func_emergency_exit(env, args);
}

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
struct plot_value * plot_func_emergency_exit(struct plot_env *env, struct plot_value *args){
    if( args->type == plot_type_null ){
        exit(0);
    }

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected either 0 or 1 arguments", "plot_func_ememgency_exit");
    }

    if( car(args)->type == plot_type_boolean && car(args)->u.boolean.val == true ){
        exit(0);
    }
    if( car(args)->type == plot_type_number && car(args)->u.number.val == 0 ){
        exit(0);
    }
    exit(1);
}

/* (force promise)
 */
struct plot_value * plot_func_force(struct plot_env *env, struct plot_value *args){
    plot_value *val;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_force");
    }

    val = car(args);

    if( val->type != plot_type_promise ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_promise", "plot_func_force");
    }

    if( ! val->u.promise.value ){
        val->u.promise.value = plot_eval_expr(env, val->u.promise.expr);
    }

    plot_value_incr(val->u.promise.value);
    return val->u.promise.value;
}

/* (promise? obj)
 * returns #t iff obj is of type promise
 * otherwise returns #f
 */
struct plot_value * plot_func_promise_test(struct plot_env *env, struct plot_value *args){
    if( !args || args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_promise_test");
    }

    return plot_new_boolean( car(args)->type == plot_type_promise );
}

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
struct plot_value * plot_func_make_promise(struct plot_env *env, struct plot_value *args){
    plot_value *val;

    if( !args || args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_make_promise");
    }

    /* do not modify if already a promise */
    if( car(args)->type == plot_type_promise ){
        return car(args);
    }

    val = plot_new_promise(env, car(args));

    /* as we are a procedure (rather than syntax)
     * the provided arg has already been eval'd
     *
     * set value to prevent re-evaluation
     */
    val->u.promise.value = car(args);
    plot_value_incr(val->u.promise.value);

    return val;
}



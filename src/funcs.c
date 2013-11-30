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

/* internal routine for displaying a value
 * returns 0 on failure, 1 on success
 */
static plot_value * plot_func_display_value(plot_env *env, plot_value *val){
    if( ! val )
        return 0;

    switch(val->type){
        case plot_type_boolean:
            printf("#%c", val->u.boolean.val ? 't' : 'f' );
            break;
        case plot_type_number:
            printf("%d", val->u.number.val);
            break;
        case plot_type_string:
            printf("%s", val->u.string.val);
            break;
        case plot_type_symbol:
            printf("%s", val->u.symbol.val);
            break;
        case plot_type_character:
            printf("%c", val->u.character.val);
            break;
        case plot_type_null:
            fputs("()", stdout);
            break;
        case plot_type_pair:
            fputs("(", stdout);
            plot_func_display_value(env, val->u.pair.car);
            fputs(" ", stdout);
            plot_func_display_value(env, val->u.pair.cdr);
            fputs(")", stdout);
            break;
        case plot_type_legacy:
            puts("Unable to print a legacy builtin function at this point in time");
            break;
        case plot_type_form:
            puts("Unable to print a form at this point in time");
            break;
        case plot_type_error:
            return plot_runtime_error(plot_error_internal, "trying to print an error value", "plot_func_display_value");
            break;
        case plot_type_lambda:
            puts("Unable to print a lambda value at this point in time");
            break;
        case plot_type_unspecified:
            puts("<unspecified>");
            break;
        case plot_type_reclaimed:
            puts("ERROR: you are trying to display a garbage collected value, most likely an error in the GC");
            exit(1);
            break;
        default:
            return plot_runtime_error(plot_error_internal, "impossible type for val->type", "plot_func_display_value");
            break;
    }

    return plot_new_unspecified();
}

/* (display obj)
 * print value to stdout
 */
plot_value * plot_func_display(plot_env *env, plot_value *args){
    plot_value *arg;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_display");
    }

    arg = car(args);

    if( ! arg ){
        return plot_runtime_error(plot_error_bad_args, "first arg was null", "plot_func_display");
    }

    return plot_func_display_value(env, arg);
}

/* (newline)
 * print a newline to stdout
 */
plot_value * plot_func_newline(plot_env *env, plot_value *args){
    /* do not allow any arguments */
    if( args->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 0 arguments", "plot_func_newline");
    }

    puts("");

    return plot_new_unspecified();
}

/********* equivalent predicates *********/

/* (equal? obj1 obj2)
 * in plot equal?, eqv? and eq? are equivalent.
 * FIXME should generalise
 */
struct plot_value * plot_func_equal_test(struct plot_env *env, struct plot_value *args){
    plot_value *o1, *o2;

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
        return plot_runtime_error(plot_error_bad_args, "first arg was null", "plot_func_equal_test");
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
        case plot_type_legacy:
            return plot_new_boolean( o1->u.legacy.func == o2->u.legacy.func );
            break;
        case plot_type_form:
            return plot_new_boolean( o1->u.form.func == o2->u.form.func );
            break;
        case plot_type_null:
            /* both arguments are of the same type, '() = '() */
            return plot_new_boolean(true);
            break;
        case plot_type_pair:
            return plot_runtime_error(plot_error_unimplemented, "pair and list equality is not yet implemented", "plot_func_equal_test");
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
 * will decr supplied val
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
struct plot_value * plot_func_and(struct plot_env *env, struct plot_value **args, int argc){
    int i;

    if( argc < 2 ){
        return plot_runtime_error(plot_error_bad_args, "insufficient arguments, expected 2", "plot_func_add");
    }

    for( i=0; i<argc; ++i ){
        if( ! plot_truthy(args[i]) ){
            return plot_new_boolean(false);
        }
    }

    return plot_new_boolean(true);
}

/* (or obj1 obj2 ...)
 * logical or of all arguments
 */
struct plot_value * plot_func_or(struct plot_env *env, struct plot_value **args, int argc){
    int i;

    if( argc < 2 ){
        return plot_runtime_error(plot_error_bad_args, "insufficient arguments, expected 2", "plot_func_or");
    }

    for( i=0; i<argc; ++i ){
        if( plot_truthy(args[i]) ){
            return plot_new_boolean(true);
        }
    }

    return plot_new_boolean(false);
}

/* (not obj)
 * logical not of single argument
 */
struct plot_value * plot_func_not(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "incorrect arguments: expected exactly 1", "plot_func_not");
    }

    return plot_new_boolean( ! plot_truthy(args[0]) );
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
struct plot_value * plot_func_exit(struct plot_env *env, struct plot_value **args, int argc){
    /* return required to keep compiler happy */
    return plot_func_emergency_exit(env, args, argc);
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
struct plot_value * plot_func_emergency_exit(struct plot_env *env, struct plot_value **args, int argc){
    if( argc == 0 ){
        exit(0);
    }
    if( argc == 1 ){
        if( args[0]->type == plot_type_boolean && args[0]->u.boolean.val == true ){
            exit(0);
        }
        if( args[0]->type == plot_type_number && args[0]->u.number.val == 0 ){
            exit(0);
        }
    }
    exit(1);
}

/* (force promise)
 */
struct plot_value * plot_func_force(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *val;
    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_force");
    }

    val = args[0];

    if( val->type != plot_type_promise ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_promise", "plot_func_force");
    }

    if( ! val->u.promise.value ){
        val->u.promise.value = plot_eval_expr(env, val->u.promise.expr);
    }

    plot_value_incr(val->u.promise.value);
    return val->u.promise.value;
}



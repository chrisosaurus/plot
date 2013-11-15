#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "value.h"
#include "types.h"
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
        case plot_type_pair:
            fputs("(", stdout);
            plot_func_display_value(env, val->u.pair.car);
            fputs(" ", stdout);
            plot_func_display_value(env, val->u.pair.cdr);
            fputs(")", stdout);
            break;
        case plot_type_builtin:
            puts("Unable to print a builtin function at this point in time");
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
plot_value * plot_func_display(plot_env *env, plot_value **args, int argc){
    plot_value *arg;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_display");
    }

    arg = args[0];

    if( ! arg ){
        return plot_runtime_error(plot_error_bad_args, "first arg was null", "plot_func_display");
    }

    return plot_func_display_value(env, arg);
}

/* (newline)
 * print a newline to stdout
 */
plot_value * plot_func_newline(plot_env *env, plot_value **args, int argc){
    /* FIXME currently ignores arguments, only there to match plot_func interface
     */
    puts("");

    return plot_new_unspecified();
}

/********* equivalent predicates *********/

/* (equal? obj1 obj2)
 * FIXME should generalise
 */
struct plot_value * plot_func_equal_test(struct plot_env *env, struct plot_value **args, int argc){
    #if DEBUG
    puts("inside plot_func_equal");
    #endif

    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 args", "plot_func_equal_test");
    }

    if( ! args[0] ){
        return plot_runtime_error(plot_error_bad_args, "first arg was null", "plot_func_equal_test");
    }

    if( ! args[1] ){
        return plot_runtime_error(plot_error_bad_args, "second arg was null", "plot_func_equal_test");
    }

    if( args[0]->type != args[1]->type ){
        #if DEBUG
        puts("\targs were not of the same type");
        #endif
        return plot_new_boolean(false);
    }

    switch( args[0]->type ){
        case plot_type_number:
            return plot_new_boolean( args[0]->u.number.val == args[1]->u.number.val );
            break;
        case plot_type_symbol:
            return plot_new_boolean( ! strcmp( args[0]->u.symbol.val, args[1]->u.symbol.val ) );
            break;
        case plot_type_lambda:
            return plot_new_boolean(
                args[0]->u.lambda.env == args[1]->u.lambda.env
                &&
                args[0]->u.lambda.body == args[1]->u.lambda.body
            );
            break;
        case plot_type_builtin:
            return plot_new_boolean( args[0]->u.builtin.func == args[1]->u.builtin.func );
            break;
        case plot_type_error:
            plot_fatal_error("plot_func_equal: saw plot_type_error");
            break;
        case plot_type_string:
            return plot_new_boolean( ! strcmp( args[0]->u.string.val, args[1]->u.string.val ) );
            break;
        case plot_type_boolean:
            return plot_new_boolean( args[0]->u.boolean.val == args[1]->u.boolean.val );
            break;
        case plot_type_character:
            return plot_new_boolean( args[0]->u.character.val == args[1]->u.boolean.val );
            break;
        case plot_type_reclaimed:
            plot_fatal_error("plot_func_equal: saw plot_type_reclaimed");
            break;
        default:
            plot_fatal_error("plot_func_equal: impossible args[0]->type");
            break;
    }

    plot_fatal_error("plot_func_equal: out of switch, impossible args[0]->type");
    return 0; /* IMPOSSIBLE as plot_fatal_error does not return */
}

/********* value testing functions ********/

/* (boolean? obj)
 * FIXME should generalise if spec allows
 */
struct plot_value * plot_func_boolean_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *val;

    #if DEBUG
    puts("inside plot_func_boolean_test");
    #endif

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_boolean_test");
    }

    val = args[0];

    if( ! val ){
        return plot_runtime_error(plot_error_bad_args, "first arg was null", "plot_func_boolean_test");
    }

    return plot_new_boolean( val->type == plot_type_boolean );
}

/* (symbol? obj)
 * FIXME should generalise if spec allows
 */
struct plot_value * plot_func_symbol_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *val;

    #if DEBUG
    puts("inside plot_func_symbol_test");
    #endif

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_symbol_test");

    }

    val = args[0];

    if( ! val ){
        return plot_runtime_error(plot_error_bad_args, "first arg was null", "plot_func_symbol_test");
    }

    return plot_new_boolean( val->type == plot_type_symbol );
}

/* (procedure? obj)
 * FIXME should generalise if spec allows
 */
struct plot_value * plot_func_procedure_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *val;

    #if DEBUG
    puts("inside plot_func_procedure_test");
    #endif

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_procedure_test");
    }

    val = args[0];

    if( ! val ){
        return plot_runtime_error(plot_error_bad_args, "first arg was null", "plot_func_procedure_test");
    }

    return plot_new_boolean( val->type == plot_type_builtin || val->type == plot_type_lambda );
}

/* returns 1 if value is considered truthy
 * returns 0 if falsy
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
 * FIXME not standard compliant, see r7rs page 59 section 6.14
 *
 * exits normally iff obj is #t or 0
 * otherwise is equiv to (exit 1)
 *
 */
struct plot_value * plot_func_exit(struct plot_env *env, struct plot_value **args, int argc){
    if( argc == 1){
        if( args[0]->type == plot_type_boolean && args[0]->u.boolean.val == true ){
            exit(0);
        }
        if( args[0]->type == plot_type_number && args[0]->u.number.val == 0 ){
            exit(0);
        }
    }
    exit(1);
}



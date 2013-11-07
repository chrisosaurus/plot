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

/* print value to stdout
 */
plot_value * plot_func_display(plot_env *env, plot_value **args, int argc){
    plot_value *arg;

    if( argc != 1 ){
        #if DEBUG
        puts("incorrect number of args to plot_func_display");
        #endif
        return 0; /* FIXME error */
    }

    arg = args[0];

    if( ! arg ){
        puts("argument was NULL");
        return 0;
    }

    return plot_func_display_value(env, arg);
}

/* print a newline to stdout
 */
plot_value * plot_func_newline(plot_env *env, plot_value **args, int argc){
    /* FIXME currently ignores arguments, only there to match plot_func interface
     */
    puts("");

    return plot_new_unspecified();
}

/********* equivalent predicates *********/

/* equal?
 */
struct plot_value * plot_func_equal_test(struct plot_env *env, struct plot_value **args, int argc){
    #if DEBUG
    puts("inside plot_func_equal");
    #endif

    if( ! env ){
        #if DEBUG
        puts("\tenv if NULL");
        #endif
        return 0;
    }

    if( argc != 2 ){
        #if DEBUG
        puts("\tinvalid number of args (expected 2)");
        #endif
        return 0;
    }

    if( ! args[0] || ! args[1] ){
        #if DEBUG
        puts("\targs were NULL");
        #endif
        return 0;
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

/* boolean?
 */
struct plot_value * plot_func_boolean_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *val;

    #if DEBUG
    puts("inside plot_func_boolean_test");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    if( argc != 1 ){
        #if DEBUG
        puts("incorrect number of args to plot_func_boolean_test");
        #endif
        return 0; /* FIXME error */
    }

    val = args[0];

    if( ! val ){
        #if DEBUG
        puts("call to plot_eval_expr returned NULL");
        #endif
        return 0; /* FIXME error */
    }

    return plot_new_boolean( val->type == plot_type_boolean );
}

/* symbol?
 */
struct plot_value * plot_func_symbol_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *val;

    #if DEBUG
    puts("inside plot_func_symbol_test");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    if( argc != 1 ){
        #if DEBUG
        puts("incorrect number of args to plot_func_symbol_test");
        #endif
        return 0; /* FIXME error */
    }

    val = args[0];

    if( ! val ){
        #if DEBUG
        puts("call to plot_eval_expr returned NULL");
        #endif
        return 0; /* FIXME error */
    }

    return plot_new_boolean( val->type == plot_type_symbol );
}

/* function?
 */
struct plot_value * plot_func_procedure_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *val;

    #if DEBUG
    puts("inside plot_func_procedure_test");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    if( argc != 1 ){
        #if DEBUG
        puts("incorrect number of args to plot_func_procedure_test");
        #endif
        return 0; /* FIXME error */
    }

    val = args[0];

    if( ! val ){
        #if DEBUG
        puts("call to plot_eval_expr returned NULL");
        #endif
        return 0; /* FIXME error */
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

/* logical and of all arguments */
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

/* logical or of all arguments */
struct plot_value * plot_func_or(struct plot_env *env, struct plot_value **args, int argc){
    int i;

    if( argc < 2 ){
        return plot_runtime_error(plot_error_bad_args, "insufficient arguments, expected 2", "plot_func_or");
    }

    for( i=0; i<argc; ++i ){
        if( plot_truthy(args[i]) ){
            return plot_new_boolean(false);
        }
    }

    return plot_new_boolean(false);
}

/* logical not of single argument */
struct plot_value * plot_func_not(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "incorrect arguments: expected exactly 1", "plot_func_not");
    }

    return plot_new_boolean( ! plot_truthy(args[0]) );
}



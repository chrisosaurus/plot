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
    plot_value *ret;

    if( ! val )
        return 0;

    ret = plot_new_value();
    if( ! ret ){
        plot_fatal_error("plot_func_display_value: failed call to plot_new_value()");
    }

    ret->type = plot_type_unspecified;

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
            puts("Unable to print a symbol at this point in time");
            break;
        case plot_type_character:
            printf("%c", val->u.character.val);
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

    return ret;
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
    plot_value *ret = plot_new_value();
    if( ret )
        ret->type = plot_type_unspecified;

    /* FIXME currently ignores arguments, only there to match plot_func interface
     */
    puts("");

    return ret;
}

/********* equivalent predicates *********/

/* equal?
 */
struct plot_value * plot_func_equal(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

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

    res = plot_new_value();
    if( ! res ){
        #if DEBUG
        puts("\tfailed to allocated result value");
        #endif
        return 0;
    }

    if( ! args[0] || ! args[1] ){
        #if DEBUG
        puts("\targs were NULL");
        #endif
        return 0;
    }

    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    if( args[0]->type != args[1]->type ){
        #if DEBUG
        puts("\targs were not of the same type");
        #endif
        return res;
    }

    switch( args[0]->type ){
        case plot_type_number:
            res->u.boolean.val = args[0]->u.number.val == args[1]->u.number.val;
            break;
        case plot_type_symbol:
            res->u.boolean.val = ! strcmp( args[0]->u.symbol.val, args[1]->u.symbol.val );
            break;
        case plot_type_lambda:
            res->u.boolean.val =
                args[0]->u.lambda.env == args[1]->u.lambda.env
                &&
                args[0]->u.lambda.body == args[1]->u.lambda.body;
            break;
        case plot_type_builtin:
            res->u.boolean.val = args[0]->u.builtin.func == args[1]->u.builtin.func;
            break;
        case plot_type_error:
            plot_fatal_error("plot_func_equal: saw plot_type_error");
            break;
        case plot_type_string:
            res->u.boolean.val = ! strcmp( args[0]->u.string.val, args[1]->u.string.val );
            break;
        case plot_type_boolean:
            res->u.boolean.val = args[0]->u.boolean.val == args[1]->u.boolean.val;
            break;
        case plot_type_character:
            res->u.boolean.val = args[0]->u.character.val == args[1]->u.boolean.val;
            break;
        case plot_type_reclaimed:
            plot_fatal_error("plot_func_equal: saw plot_type_reclaimed");
            break;
        default:
            plot_fatal_error("plot_func_equal: impossible args[0]->type");
            break;
    }
    return res;
}

/********* value testing functions ********/

/* boolean?
 */
struct plot_value * plot_func_boolean_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;
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

    res = plot_new_value();
    if( ! res ){
        #if DEBUG
        puts("called to plot_new_value failed");
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

    res->type = plot_type_boolean;
    if( val->type == plot_type_boolean ){
        res->u.boolean.val = true;
        return res;
    } else {
        res->u.boolean.val = false;
        return res;
    }
}

/* symbol?
 */
struct plot_value * plot_func_symbol_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;
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

    res = plot_new_value();
    if( ! res ){
        #if DEBUG
        puts("called to plot_new_value failed");
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

    res->type = plot_type_boolean;
    if( val->type == plot_type_symbol ){
        res->u.boolean.val = true;
        return res;
    } else {
        res->u.boolean.val = false;
        return res;
    }
}

/* function?
 */
struct plot_value * plot_func_procedure_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;
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

    res = plot_new_value();
    if( ! res ){
        #if DEBUG
        puts("called to plot_new_value failed");
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

    res->type = plot_type_boolean;
    if( val->type == plot_type_builtin || val->type == plot_type_lambda ){
        res->u.boolean.val = true;
        return res;
    } else {
        res->u.boolean.val = false;
        return res;
    }
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
    plot_value *ret;

    if( argc < 2 ){
        return plot_runtime_error(plot_error_bad_args, "insufficient arguments, expected 2", "plot_func_add");
    }

    ret = plot_new_value();
    ret->type = plot_type_boolean;
    ret->u.boolean.val = false;

    for( i=0; i<argc; ++i ){
        if( ! plot_truthy(args[i]) ){
            return ret;
        }
    }

    ret->u.boolean.val = true;
    return ret;
}

/* logical or of all arguments */
struct plot_value * plot_func_or(struct plot_env *env, struct plot_value **args, int argc){
    int i;
    plot_value *ret;

    if( argc < 2 ){
        return plot_runtime_error(plot_error_bad_args, "insufficient arguments, expected 2", "plot_func_or");
    }

    ret = plot_new_value();
    ret->type = plot_type_boolean;
    ret->u.boolean.val = true;

    for( i=0; i<argc; ++i ){
        if( plot_truthy(args[i]) ){
            return ret;
        }
    }

    ret->u.boolean.val = false;
    return ret;
}

/* logical not of single argument */
struct plot_value * plot_func_not(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *ret;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "incorrect arguments: expected exactly 1", "plot_func_not");
    }

    ret = plot_new_value();
    ret->type = plot_type_boolean;

    if( plot_truthy(args[0]) ){
        ret->u.boolean.val = false;
        return ret;
    } else {
        ret->u.boolean.val = true;
        return ret;
    }
}



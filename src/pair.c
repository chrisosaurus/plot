#include "value.h"
#include "pair.h"

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
 */
struct plot_value * plot_func_pair_length(struct plot_env *env, struct plot_value **args, int argc){
    return plot_runtime_error(plot_error_unimplemented, "unimplemented", "plot_func_pair_length");
}

/* (append list ...)
 * returns a newly allocated list made up on the concatenation of all provided lists
 */
struct plot_value * plot_func_pair_append(struct plot_env *env, struct plot_value **args, int argc){
    return plot_runtime_error(plot_error_unimplemented, "unimplemented", "plot_func_pair_append");
}

/* (reverse list)
 * return new allocated list containing all the values form list in reverse order
 */
struct plot_value * plot_func_pair_reverse(struct plot_env *env, struct plot_value **args, int argc){
    return plot_runtime_error(plot_error_unimplemented, "unimplemented", "plot_func_pair_reverse");
}


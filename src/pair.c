#include "value.h"
#include "pair.h"

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

#define DEBUG 0

/* (pair? obj)
 * return #t iff obj is of type plot_type_pair
 */
struct plot_value * plot_func_pair_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 argument", "plot_func_pair_test");
    }

    res = plot_alloc_value();
    res->type = plot_type_boolean;
    res->u.boolean.val = args[0]->type == plot_type_pair;
    return res;
}

/* (cons v1 v2)
 * return a pair with v1 as car and v2 as cdr
 */
struct plot_value * plot_func_pair_cons(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 arguments", "plot_func_pair_cons");
    }

    res = plot_alloc_value();
    res->type = plot_type_pair;

    res->u.pair.car = args[0];
    plot_value_incr(args[0]);

    res->u.pair.cdr = args[1];
    plot_value_incr(args[1]);

    return res;
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



#include "value.h"
#include "plot.h"
#include "input.h"

#define DEBUG 0

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

/* (eof-object)
 * returns an end-of-file object
 * not necessarily unique
 */
struct plot_value * plot_func_input_eof_object(struct plot_env *env, struct plot_value *args){
    return plot_get_eof_constant();
}

/* (eof-object? obj)
 * returns #t iff obj is an end-of-file object
 * otherwise returns #f
 */
struct plot_value * plot_func_input_eof_object_test(struct plot_env *env, struct plot_value *args){
    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_input_eof_object_test");
    }

    return plot_new_boolean(car(args)->type == plot_type_eof);
}



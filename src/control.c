#include "control.h"
#include "value.h"
#include "plot.h"
#include "funcs.h"
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

    return plot_new_boolean( val->type == plot_type_legacy || val->type == plot_type_lambda || val->type == plot_type_form  );
}

/* (apply proc args1 ... args)
 * FIXME not r7rs compliant as we currently call func with argument set to (append arg1 ... argn)
 * where spec says:
 *   calls proc with the elements of the list
 *   (append (list arg1...) args) as the argument
 *
 * spec allows for both:
 *  (apply + '(3 4))   ; => 7
 *  (apply + 3 '(1 3)) ; => 7
 */
struct plot_value * plot_func_control_apply(struct plot_env *env, struct plot_value *args){
    plot_value *func;
    plot_value *newargs;

    /* FIXME not r7rs compliant
     * spec says the argument should be
     * (append (list arg1...) args)
     *
     * this allows for
     *  (apply + 1 2 '(3 4 5)) ; =>15
     * but not
     *  (apply + 1 2) ; => error
     * nor
     *  (apply '(1 2 3) '(4 5 6)) ; => error
     */
    newargs = plot_func_pair_append(env, args);

    func = car(args);

    switch(func->type){
        case plot_type_lambda:
            return plot_runtime_error(plot_error_bad_args, "apply is not yet implemented for lambda functions", "plot_func_control_apply");
            break;
        case plot_type_form:
            return func->u.form.func(env, newargs);
            break;
        case plot_type_legacy:
            return plot_runtime_error(plot_error_bad_args, "apply is not implemented for legacy builtins (legacy builtins are slowly being removed)", "plot_func_control_apply");
            break;
        default:
            return plot_runtime_error(plot_error_bad_args, "first argument is not a function", "plot_func_control_apply");
            break;
    }

    return 0;
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
 * (map cadr '((a b) (d e) (g h)) ; => (b e h)
 */
struct plot_value * plot_func_control_map(struct plot_env *env, struct plot_value *args){
    return plot_runtime_error(plot_error_unimplemented, "pending implementation", "plot_func_control_");
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
    return plot_runtime_error(plot_error_unimplemented, "pending implementation", "plot_func_control_");
}



#include <stdlib.h>

#include "value.h"
#include "hash.h"
#include "env.h"
#include "funcs.h"

plot_value * plot_func_add(plot_env *env, plot_value *v1, plot_value *v2){
    plot_value *res;

    if( !env || !v1 || !v2 )
        return 0; /* ERROR */


    if( ! (v1->type == plot_type_number) )
        return 0; /* ERROR */

    if( ! (v2->type == plot_type_number) )
        return 0; /* ERROR */

    res = calloc(1, sizeof *res);
    if( !res )
        return 0; /* ERROR */

    res->type = plot_type_number;
    res->u.number.val = v1->u.number.val + v2->u.number.val;
    return res;
}

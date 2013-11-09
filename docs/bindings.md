bindings
========

build/generate_bindings.h is run during the build process in order to generate `src/bindings.h`

this script will inspect certain header files (list is in source) and generate a the bindings
from scheme functions to their c implementations.

for example:

    /* (display obj)
     * print value to stdout
     */
    struct plot_value * plot_func_display(struct plot_env *env, struct plot_value **args, int argc);

when inspected will generate a bindings between `display` and `plot_func_display`



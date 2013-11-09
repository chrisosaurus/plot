bindings
========

build/generate_bindings.h is a perl script that when run from the root of the
project will inspect appropriate header files in order to extract information
to facilitate bindings between scheme functions and their c implementations.

for example:

    /* (display obj)
     * print value to stdout
     */
    struct plot_value * plot_func_display(struct plot_env *env, struct plot_value **args, int argc);

when inspected will generate a bindings between `display` and `plot_func_display`

Eventually this will be made part of the build process in order to replace the static list inside src/plot.c


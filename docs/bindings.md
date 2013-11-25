bindings
========

build/generate_bindings.h is run during the build process in order to generate `src/bindings.h`

this script will inspect certain header files (list is in source) and generate a the bindings
from scheme functions to their c implementations.

for example:

    /* (display obj) -library
     * print value to stdout
     */
    struct plot_value * plot_func_display(struct plot_env *env, struct plot_value **args, int argc);

when inspected will generate a bindings between `display` and `plot_func_display`

Notice the `- library` this indicated an argument to generate_bindings, `-library` means this function
is belongs to a library and should not be bound to the default global namespace.

Possible arguments and their meanings:

* -core - bound in the global namespace of every plot instance
* -library - bound to a library, only accessible via import
* -syntax - syntactical form, takes arguments unevaluated

You can combine `syntax` either either of the other two arguments, e.g `-core -syntax`.

If no argument is found the default is `library`.

argument interface
------------------

a function is given it's arguments as a plot list (chain of pairs where the final cdr is a null, cars contain data)

a form received a plot list of it's arguments pre-eval


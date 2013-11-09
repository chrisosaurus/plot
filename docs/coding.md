Scheme procedures that has hyphens will instead have underscores in their c function names.

`(char-toupper ...)` => `plot_func_char_toupper`

Procedures that end in a question mark in scheme should have the suffix `_test` in their
c implementation function names.

`(char-alphabetic? ...)` => `plot_func_char_alphabetic_test`


Procedures that us arrow in their scheme name should use `_to_` in their c function names

`(char->integer ...)` => `plot_func_char_to_integer`


every c function that should be exposed to the scheme runtime must be preceded by a function
declaring it's c binding:

    /* (display obj)
     * print value to stdout
     */
    struct plot_value * plot_func_display(struct plot_env *env, struct plot_value **args, int argc);

see `docs/bindings.md` for more information.


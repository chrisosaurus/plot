Scheme procedures that has hyphens will instead have underscores in their c function names.

`(char-toupper ...)` => `plot_func_char_toupper`

Procedures that end in a question mark in scheme should have the suffix `_test` in their
c implementation function names.

`(char-alphabetic? ...)` => `plot_func_char_alphabetic_test`


Procedures that us arrow in their scheme name should use `_to_` in their c function names

`(char->integer ...)` => `plot_func_char_to_integer`



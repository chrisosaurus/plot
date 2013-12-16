TODO(s):
=====

Update code comments to reference r7rs.

operations:
-----------
* string-ci<?
* string-ci>?
* string-ci<=?
* string-ci>=?
* map
* string-map
* for-each
* string-for-each
* input-port-open?
* output-port-open?
* read-string
* read-line
* read-char
* peek-char
* char-ready?

forms:
------
* case
* case-lambda

garbage collection:
-------------------
* implement arena growth
* merge arenas
* need to work out how to deal with constants - for now we will not gc anything allocated at parse time

performance:
------------
* more performant env structure
* consider caching of booleans (similar to null and unspecified)

misc:
-----

parse:
------
* parse should record line numbers in AST, all errors should reference these
* allow parsing of negative numbers
* needs to be able to ask for 'more' input (if current input does not properly end a token)
* needs to be able to communicate 'failure', e.g. miss matching of brackets in `(define a 5]`

eval:
-----
* tidy up eval_form variables

read:
-----
* general tidyup

testing:
--------
* error testing suite should test generation of runtime errors
* need more complete testing of input/output/ports

features:
---------
* quoting of improper lists, pairs e.g. '(a.b)
* write repl front-end
* macro system
* variable arguments
* begin work on library system
* play around with `display_error_expr` as it is proving very useful, should probably give it a more general name

runtime:
---------
* eval should keep a stack of the internal functions it uses (for internal debugging)
* eval should keep a stack of the programs functions (for program debugging)
* tail call optimisation

bugs:
-----
* parsing of # is silly
* string do not currently support escaping
* escaping in strings will copy over the escape character and include it in size/len
* plot runtime errors may leak previously allocated values, need to decr before throwing error.
* plot_new_string and plot_alloc_string are not really related, as the latter allocates an array and the former a value
* plot_eval_form should only return 0 on error, otherwise it is a runtime error
* parsing feedback is terrible
* plot_runtime_errors should probably use scheme name (rather than c implementation name)
* look very carefully at gc around force (incr when generated vs incr when returned)
* escape characters not behaving, e.g. (display "\t") => segfault


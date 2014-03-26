TODO(s):
=====

Update code comments to reference r7rs.

priorities:
-----------
* library support
* plot unit testing library
* plot introspection library
* better I/O facilities
* repl

library support:
----------------
* import functionality
* export rename functionality
* adding defined library to list

operations:
-----------
* string-ci<?
* string-ci>?
* string-ci<=?
* string-ci>=?
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
* strings should also be inside gc (currently malloc-ed)

performance:
------------
* more performant env structure
* consider caching of booleans (similar to null and unspecified) - more generally we should extend caching for arbitrary objects (rather than just shoving them on the plot_instance value)

misc:
-----

parse:
------
* parse should record line numbers in AST, all errors should reference these
* allow parsing of negative numbers
* needs to be able to ask for 'more' input (if current input does not properly end a token) - should only require counting `(`, `)`, `"`, `"` and `\` (for escaping of other symbols)
* needs to be able to communicate 'failure', e.g. miss matching of brackets in `(define a 5]`

eval:
-----
* tidy up eval_form variables
* eval form should also use apply (possibly evaluating args before call)

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
* play around with `display_error_expr` as it is proving very useful, should probably give it a more general name

runtime:
---------
* eval should keep a stack of the internal functions it uses (for internal debugging)
* eval should keep a stack of the programs functions (for program debugging)
* tail call optimisation

bugs:
-----
* need to better document return values of eval functions
* string size/len is silly, too many magic numbers (+1 to store, -1 to use... ugly)
* plot pair cons and decons are not symmetrical, cons will NOT incr but decons does decr
* strings do not correctly support escaping
* escaping in strings will copy over the escape character and include it in size/len
* escape characters not behaving, e.g. (display "\t") => segfault
* parsing of # is silly
* plot runtime errors may leak previously allocated values, need to decr before throwing error (sadly must be done at each call site).
* plot_runtime_errors should generally use scheme name (rather than c implementation name)
* plot_eval_form should only return 0 on error, otherwise it is a runtime error
* plot_new_string and plot_alloc_string are not really related, as the latter allocates an array and the former a value - more generally we need to be able to distinguish between internal and external (i.e. routines that deal with c objects, routines that deal with plot objects, and routines that are exposed at runtime)
* parsing feedback is terrible - need to track source line and even text (in case of transformations, e.g. 'a -> (quote a))
* look very carefully at gc around force (incr when generated vs incr when returned)
* look very carefully at ref counting around various values and storage - current system is very brittle

limitations:
------------
* garbage collection of an object may trigger further garbage collection of other objects (see value.c:plot_value_decons), this could quickly exhaust the C stack height, consider switching to a work list model.
* refcount could overflow (currently an int, as we use < 1 for not-managed)
* many temporaries do not increase refcounts (e.g. forms.c:plot_form_define_library), this is generally okay in a single-threaded env but does not scale.


TODO(s):
=====

Update code comments to reference r7rs.

priorities:
-----------
* eval/assoc bug -> apply/eval refactor (see docs/func_call.md)
* library support
* plot unit testing library
* plot introspection library
* better I/O facilities
* repl
* break src/ up, possibly into src/userspace and src/internal

library support:
----------------
* basic import functionality
* ability to bind to `(scheme base)`, `(srfi 1)`, etc.
* check for existing library on `define-library`
* export rename functionality
* cond-expand
* more complex import (only, except, prefix, rename)
* includes (ci, library-declaration, normal)

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
* lambda rest args forms `(lambda args body ...)` and `(lambda (arg1 . args) body ...)`
* define rest args form `(define (foo . args) body ...)`

design:
-------
* should all forms be in forms.[ch] ?
* funcs.[ch] seems to specific, consider breaking down

garbage collection:
-------------------
* implement arena growth
* merge arenas
* need to work out how to deal with constants - for now we will not gc anything allocated at parse time
* strings should also be inside gc (currently malloc-ed)

performance:
------------
* more performant env structure
* more generally we should extend caching for arbitrary objects (rather than just shoving them on the plot_instance value)

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
* see docs/func_call.md

read:
-----
* general tidyup

testing:
--------
* error testing suite should test generation of runtime errors (negative testing suite, see `docs/negative_testing.md`)
* need more complete testing of input/output/ports

features:
---------
* quoting of improper lists, pairs e.g. '(a.b)
* lambda list args `((lambda x x) 3 4 5 6)` see bugs/lambda.md
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
* see bugs/ for test cases producing bugs
* stack trace showing > Unable to print a form at this point in time
* strings do not correctly support escaping
* escaping in strings will copy over the escape character and include it in size/len
* escape characters not behaving, e.g. (display "\t") => segfault
* lack of key incr in plot_hash_set
* plot_hash_set is stupid, hash_entry reuse value is plain wrong
* value.c car and cdr do not incr, check usage

hints:
-----
* all plot errors that feature 'not of type ...' should also mention the type that it was
* should detect define rest args form `(define (foo . args) body ...)` and trigger a plot_error_unimplemented bugs/args.scm
* need to better document return values of eval functions
* string size/len is silly, too many magic numbers (+1 to store, -1 to use... ugly)
* plot pair cons and decons are not symmetrical, cons will NOT incr but decons does decr
* parsing of # is silly
* plot runtime errors may leak previously allocated values, need to decr before throwing error (sadly must be done at each call site).
* plot_runtime_errors should generally use scheme name (rather than c implementation name)
* plot_eval_form should only return 0 on error, otherwise it is a runtime error
* plot_new_string and plot_alloc_string are not really related, as the latter allocates an array and the former a value - more generally we need to be able to distinguish between internal and external (i.e. routines that deal with c objects, routines that deal with plot objects, and routines that are exposed at runtime)
* parsing feedback is terrible - need to track source line and even text (in case of transformations, e.g. 'a -> (quote a))
* look very carefully at gc around force (incr when generated vs incr when returned)
* look very carefully at ref counting around various values and storage - current system is very brittle
* plot_eval_truthy -> plot_truthy removed implicit decr, found some leaks that were caused by reliance on this removed functionality, check all calls to plot_truthy correctly decr the value (if no longer needed)
* refactor out arg/type checking

limitations:
------------
* garbage collection of an object may trigger further garbage collection of other objects (see value.c:plot_value_decons), this could quickly exhaust the C stack height, consider switching to a work list model.
* refcount could overflow (currently an int, as we use < 1 for not-managed)
* many temporaries do not increase refcounts (e.g. forms.c:plot_form_define_library), this is generally okay in a single-threaded env but may not scale.


TODO(s):
=====

misc:
-----
* write stack (for runtime and internal stacks)
* break math and comparison funcs into own files (.h and .c)
* plot_handle_error should not be in funcs, funcs are things that can be assigned to plot_values
* define does not currently allow redefinition (even in smaller scope)

parse:
------
* remove hard coded limit on number of children (max_children for program(100) and sexpr(15))
* needs to be able to ask for 'more' input (if current input does not properly end a token)
* needs to be able to communicate 'failure', e.g. miss matching of brackets in `(define a 5]`

eval:
-----
* better form unit testing
* define eval pointer ownership (currently returned values are a mess....)
* eval documentation
* add some heavier unit testing coverage for eval (test each individual func)

read:
-----
* write...

features:
---------
* write repl front-end

runtime:
---------
* eval should keep a stack of the internal functions it uses (for internal debugging)
* eval should keep a stack of the programs functions (for program debugging)
* need an init routine to create initial env (including loading all built-in functions in)
* devise error handling strategy - should functions call plot_handle_error or return plot_values (or type plot_error) ?
* think a bit more about memory allocation and ref counting / garbage collection
* improve plot_error
* use plot_error

Milestones:
===========
arbitrary groupings follow

first milestone: 'core'
----------------
* basic parse and eval
* integers and ops: +, -, /, *, remainder, <, <=, >, >=, =
* symbol data type
* display and newline
* define
* string datatype
* boolean datatype
* value testing functions: string? symbol? integer? number? function? boolean?
* if form
* lambda forms
* eq? and equal?
* pair datatype, car, cdr, cons and list constructors
* quoting and display of quoted data
* display of all values

second milestone: 'building blocks'
-----------------
* syntax-rules
* let-syntax
* define-syntax

third milestone: 'plumbling'
---------------
* memory allocation and garbage collection
* error handling
* internal stack tracing
* plot stack tracing

fourth milestone: 'polish'
----------------
* repl
* front-end


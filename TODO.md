TODO(s):
=====

misc:
-----
* write stack (for runtime and internal stacks)
* refactor func unit testing
* break math and comparison funcs into own files (.h and .c)

parse:
------
* add comments
* remove hard coded limit on number of children (max_children for program and sexpr)
* needs to be able to ask for 'more' input (if current input does not properly end a token)
* needs to be able to communicate 'failure', e.g. miss matching of brackets in `(define a 5]`

eval:
-----
* value testing functions: string? symbol? integer? number? function? boolean?
* if form
* define eval pointer ownership (currently returned values are a mess....)
* eval documentation
* add some heavier unit testing coverage for eval (test each individual func)

read:
-----
* write...

features:
---------
* add lambda form
* add value testing functions: string? symbol? number? function?
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
* define (no func form)
* string datatype
* boolean datatype
* value testing functions: string? symbol? integer? number? function? boolean?
* if form
* lambda forms
* pair datatype, car, cdr, cons and list constructors
* quoting and display of quoted data

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


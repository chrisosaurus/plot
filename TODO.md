TODO(s):
=====

operations:
-----------
* substring
* string-append
* string-ci=?

experimental:
-------------
* remove creation logic from parse and tests - centralise in function (will probably require merging of value and symbols in hash/env and gc interface)
* use the above func in parse, plot.c and tests
* refactor plot_is_form, plot_eval_form and plot_eval_sexpr to merge the former 2 (try and check value)

garbage collection:
-------------------
* implement arena growth
* need to work out how to deal with constants - for now we will not gc anything allocated at parse time
* merge arenas

performance:
------------
* recursion is currently very expensive (both in terms of stack and memory) -> need tail call optimisation
* reduction of strcmp usage
* more performant env structure

misc:
-----
* 'Testing parse error handling TODO' error
* write stack (for runtime and internal stacks)
* break math and comparison funcs into own files (.h and .c)
* all math functions should check argument count
* plot_handle_error should not be in funcs, funcs are things that can be assigned to plot_values
* probably want to move plot bindings out of plot.c
* simplify defining of plot bindings (counting of chars = lame)
* too much repeated handling for stack unwinding in eval
* want to also print context for errors (e.g. expr being evalled when error occured, expr at each stage of stack unwinding)

parse:
------
* parse should record line numbers in AST, all errors should reference these
* allow parsing of negative numbers
* remove hard coded limit on number of children (max_children for program(100) and sexpr(15))
* needs to be able to ask for 'more' input (if current input does not properly end a token)
* needs to be able to communicate 'failure', e.g. miss matching of brackets in `(define a 5]`

eval:
-----
* define eval pointer ownership (currently returned values are a mess....)
* eval documentation
* add some heavier unit testing coverage for eval (test each individual func)
* plot runtime error checks should be everywhere!

read:
-----
* write...

testing:
--------
* error testing suite should test generation of runtime errors

features:
---------
* write repl front-end
* macro system
* quoting
* variable arguments

runtime:
---------
* eval should keep a stack of the internal functions it uses (for internal debugging)
* eval should keep a stack of the programs functions (for program debugging)
* need an init routine to create initial env (including loading all built-in functions in)
* devise error handling strategy - should functions call plot_handle_error or return plot_values (or type plot_error) ?
* think a bit more about memory allocation and ref counting / garbage collection
* improve plot_error
* use plot_error
* tail call optimisation
* garbage collector

bugs:
-----
* parsing of # is silly
* remove value-creation logic from parse and unit tests, instead expose functions that encapsulate it
* plot_get_env vs plot_env_get, could change former to plot_get_global_env
* handling of val and func within func_call, if they are unique then val isn't always collected
* function lookup fail can cause segfault
* string do not currently support escaping
* everything parse

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
* equal?
* char type
* port type
* basic io
* pair datatype, car, cdr, cons and list constructors
* variable arguments - (define (a . b)) and (lambda args)
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


TODO(s):
=====

Update code comments to reference r7rs.

operations:
-----------
* string-ci<?
* string-ci>?
* string-ci<=?
* string-ci>=?
* string->list
* list->string
* null?
* list
* list?
* length
* append
* reverse
* eq?
* eqv?

forms:
------
* quote
* begin
* cond
* case
* case-lambda
* delay
* force

garbage collection:
-------------------
* implement arena growth
* need to work out how to deal with constants - for now we will not gc anything allocated at parse time
* merge arenas

performance:
------------
* recursion is currently very expensive (both in terms of stack and memory) -> need tail call optimisation
* more performant env structure

misc:
-----
* 'Testing parse error handling TODO' error
* write stack (for runtime and internal stacks)
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
* eval documentation
* add some heavier unit testing coverage for eval (test each individual func)

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
* want a way of printing literal syntax (without attempting to eval)
* begin work on library system
* allow c functions ('builtin's) to be syntactic

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
* string do not currently support escaping
* everything parse
* escaping in strings will copy over the escape character and include it in size/len
* plot runtime errors may leak previously allocated values, need to decr before throwing error.
* break integration.scm tests into separate tests for each section
* plot_new_string and plot_alloc_string are not really related, as the latter allocates an array and the former a value
* need to update number.c to use plot_runtime_error system
* plot_eval_form should only return 0 on error, otherwise it is a runtime error
* implementation of define function shorthand is ugly

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
* value testing functions: string? symbol? integer? number? procedure? boolean?
* if form
* lambda forms
* equal?
* character type
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


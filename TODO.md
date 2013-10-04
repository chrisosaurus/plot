TODO(s):
=====

eval:
-----
* define eval pointer ownership (currently returned values are a mess....)
* eval documentation
* add some heavier unit testing coverage for eval (test each individual func)

features:
---------
* add define form (values only, no define function short-hand)
* add lambda form
* add display and newline functions
* add string data type
* add value testing functions: string? symbol? number? function?
* write repl front-end
* add * and - functions

runtime:
---------
* devise error handling strategy - should functions call plot_handle_error or return plot_values (or type plot_error) ?
* need an init routine to create initial env (including loading all built-in functions in)
* think a bit more about memory allocation and ref counting / garbage collection
* improve plot_error
* use plot_error


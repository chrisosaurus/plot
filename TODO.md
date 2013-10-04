TODO(s):
=====

misc:
-----
* refactor test_main.c, remove silly duplicatation and use a macro (defined in test.h)
* fix 'DEFINE incorrect number of nchildren' error
* test_full needs to add display and newline to env

parse:
------
* test_parse unit test needs to better test the output

eval:
-----
* define eval pointer ownership (currently returned values are a mess....)
* eval documentation
* add some heavier unit testing coverage for eval (test each individual func)
* remove plot_is_form, plot_eval_form can be tried first (and fail)

features:
---------
* add define form (values only, no define function short-hand)
* add lambda form
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


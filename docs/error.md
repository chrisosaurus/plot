Errors
======

Plot has 2 classes of errors, runtime and 'fatal' (usually pre-runtime).

Fatal errors are caused by a call to `plot_fatal_error`, this will print out
error information and then terminate plot via `exit(1)`.
Fatal errors are primarily used pre-runtime or in other circumstances where
returning an error value is not ideal.

Runtime errors are handled by calling `plot_runtime_error` which will return
a plot_value * which the caller then returns, each function along the call
chain is expected to check the type of every value generated, if a runtime
error is encountered then a function should print it's name and then return
the value further up the chain.



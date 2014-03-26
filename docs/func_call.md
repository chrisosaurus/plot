Function call interface
=======================

Internal routines for calling procedures are still in flux, currently there is both `plot_func_control_apply` and `plot_eval_form`.

This document is more a work-in-progress.


Musings
-------------

a call to a function looks like

    (func arg1 args ...)

where func is one of:

* lambda - scheme function
* form - c function
* syntactic form - c function which takes args pre-evaluation

all 3 of these will shortly be handled by `(apply proc args)` which will not perform
further evaluation on it's `args`, therefore for either `lambda` or `form` the caller
must first evaluate the args before calling `apply`.

currently internal callers of apply have to cons up the arguments twice

    cons( proc, cons( args, null ) )

it may be worthwhile to create an internal apply (that the user land apply is a wrapper of)

    internal_apply(env, proc, args)

as this saves at least the outer cons

Function call interface
=======================

Internal routines for calling procedures are still in flux, currently there is both `plot_func_control_apply` and `plot_eval_form`.

There is `plot_eval_apply` which is currently used by `plot_func_control_apply` and will eventually be used by `plot_eval_form`.

This document is more a work-in-progress.

Which should you use?
---------------------

* Calling function from within plot/scheme `(apply func args...)` (`plot_func_control_apply`)
* Calling form from within c userspace `plot_eval_apply(env, func, args)` (prevent re-evaluation of arguments)
* Calling form from within c implementation-land, either `plot_eval_form(env, form)` or `plot_eval_apply(env, func, args)`, former will evaluate args and latter will not.

Musings
-------------

a call to a function looks like

    (func arg1 args ...)

where func is one of:

* lambda - scheme function
* form - c function
* syntactic form - c function which takes args pre-evaluation

Plan
------
for `(apply + '(1 2))` the arg given to apply is currently:

    (cons func (cons '(1 2) '()))

this is fine from userland, but from implementation-land it is a PITA


instead we should move logic out of both `plot_func_control_apply` and `plot_eval_form` into a new `plot_eval_apply` which has a signature

    plot_eval_apply(env, func, args)

this saves 2 cons and is much easier to work with in c, compare:

    apply( env, (cons func (cons '(1 2) '())) )

becomes

    eval_apply( env, func, '(1 2) )


plot_eval_apply will NOT perform further evaluation on it's argument so it safe to call from userspace,

using plot_eval_apply fixed our previous assoc bug (fixed in cc930111)

see bugs/assoc.scm and bugs/eval


Both `plot_eval_form` and `plot_func_control_apply` would become wrappers around this

    plot_eval_form(env, form):
        func = car(form) +/- resolution
        args = cdr(form)
        if func is non-syntactic (normal form or lambda) evaluate args
        return plot_apply_internal(env, func, args)

    plot_func_control_apply(env, arg):
        func = car(arg) # resolution is handled by eval_form when apply is called
        args = append(cdr(arg))
        return plot_apply_internal(env, func, args)



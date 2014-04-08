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

Plan
------
for `(apply + '(1 2))` the arg given to apply is currently:

    (cons func (cons '(1 2) '()))

this is fine from userland, but from implementation-land it is a PITA


instead we should move logic out of both `plot_func_control_apply` and `plot_eval_form` into a new `plot_apply_internal` which has a signature

    plot_apply_internal(env, func, args)

this saves 2 cons and is much easier to work with in c, compare:

    apply( env, (cons func (cons '(1 2) '())) )

becomes

    apply( env, func, '(1 2) )


plot_apply_internal will NOT perform further evaluation on it's argument so it safe to call from userspace,
this solves our current 'assoc eval bug' (see bugs/assoc.scm and bugs/eval)


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



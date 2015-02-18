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
see `historical bugs/eval documentation` section below


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


historical bugs/eval documentation
-----------------------------------

below is the work-in-progress historical documentation of a 'bug', the above being the implementation
plan for a fix

this plan has been rolled out and below now justifies every remaining usage of plot_eval_*


the problem is non-syntactic forms calling eval on non-syntactic forms
as this causes args to be re-evaled

this is to say the error is called when a c function is called via an eval function,
when this c function is non-syntactic that means its' arguments are first evaluated and
then passed to it
if this function then in turn uses another non-syntactic c function via eval
passing the same arguments
these arguments will further be evaluted again (by eval_) before being passed to the called
function


some fixed cases:

in commit cc930111

    control.c:140:                ret = plot_eval_expr(new_env, car(arg));

became

    control.c:85:    ret = plot_eval_apply(env, func, newargs);


in commit cc930111

    pair.c:567:        res = plot_eval_form(env, arg);
    pair.c:729:        res = plot_eval_form(env, arg);

became

    pair.c:649:        res = plot_eval_apply(env, func, arg);
    pair.c:812:        res = plot_eval_apply(env, func, arg);


finding some further broken cases:

start with

    chris@Ox1b src(master)-> grep -Rni 'plot_eval' [^e]*.c

removing any that are plot_eval_apply (now listed above as examples)

we then further remove any where the function declaration features a comment declaring it a syntactic form

for example

    forms.c:240:        ret = plot_eval_sexpr(in, cur);

is inside the function

    /* (define-library (library name) body...) -core -syntax
     * define a library
     *
     * library definitions are of the form
     * (define-library <library-name>
     *      <library-declaration> ...)
     *
     * where a <library-declaration> is any of:
     *      (export <export spec> ...)
     *      (import <import set> ...)
     *      (begin <command or defintion> ...)
     *      (include <filename1> <filename2> ...)
     *      (include-ci <filename1> <filename2> ...)
     *      (include-library-declaration <filename1> <filename2> ...)
     *      (cond-expand <ce-clause1> <ce-clause2> ...)
     */
    struct plot_value * plot_form_define_library(struct plot_env *env, struct plot_value *sexpr);

due to the -syntax we can safely ignore this (as they are syntactic functions and so their arguments
are not pre-evaluated before call, this is used by build/bindings.pl to generate src/bindings.h,
see `make src/bindings.h` and docs/bindings.md)

further stripped out non problems (other syntactic functions)

    begin forms.c:399:        value = plot_eval_expr(env, car(cur));
    define forms.c:458:        value = plot_eval_expr(env, car(body));
    if forms.c:595:    value = plot_eval_expr(env, cond);
    if forms.c:612:        value = plot_eval_expr(env, if_expr);
    if forms.c:629:        value = plot_eval_expr(env, else_expr);
    cond forms.c:731:            res = plot_eval_expr(env, tmp);
    cond forms.c:756:            res = plot_eval_expr(env, tmp);
    set forms.c:791:    value = plot_eval_expr(env, expr);
    and forms.c:895:        value = plot_eval_expr(env, car(cur));
    or forms.c:930:        value = plot_eval_expr(env, car(cur));

remaining problem children:

    force funcs.c:311:        val->u.promise.value = plot_eval_expr(env, val->u.promise.expr);

however this is also fine as it is only ever running eval_expr over a promise.expr
which is only ever set by `delay` which itself is a syntactic form
so this eval in force can never cause a double-eval


see docs/func_call.md for a resolution

see commit cc930111 for previous work to fix


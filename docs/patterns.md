patterns
==================

there are a few patters used throughout plot that may seem dangerous at first glance,
here we attempt to explain them.


Building up an in-order list
-----------------------------
This can be found in eval.c:plot_eval_form (`val` and `vals`) and forms.c:plot_form_define_library (`defintions` and `defcur`, `exports` and `expcur`).

The code is usually of the form:

    plot_value *val = null;
    plot_value **vals = &val;

    for(...; ...; ...){
        *vals = cons(..., null);
        ... /* often inserting into car(*vals) */
        *vals = &cdr(*vals);
    }

this is safe as `null` is a constant and so we do not have to worry about it's reference count;
each `cons` we create comes out with a reference count of 1 and we immediately store it, so no
additional references are created (and thus the count of 1 is correct).


this idea is also often expressed as:

    plot_value *val = 0;
    plot_value **vals = &val;

    for(...; ...; ...){
        *vals = cons(0, null);
        ... /* inserting into car(*vals) */
        *vals = &cdr(*vals);
    }

these two forms are effectively equivalent.



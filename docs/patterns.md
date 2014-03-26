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


Consuming a list in-order
--------------------------
For usage see forms.c:plot_form_define_library (`body`, `cur` and `item`).

Say we have a list `body`

    '((foo bar)
      (fooo barr)
      (baz barf))

and we wan't to iterate through it

    for( cur = body; cur->type == plot_type_pair; cur = cdr(cur) ){
        item = car(cur);
        ...
    }

at each step `cur` refers to the remaining unprocessed section of the list,
and `item` refers to the current item to process

step 1:

    cur = body
    '((foo bar)
      ((fooo barr)
       ((baz barf))))

    item = car(cur)
    '(foo bar)

step 2:

    cur = cdr(body)
    '((fooo barr)
     ((baz barf)))

    item = car(cur)
    '(fooo barr)

step 3:

    cur = cdr(cdr(body))

    '((baz barf))

    item = car(cur)
    '(baz barf)

step 4:

    cur = cdr(cdr(cdr(body)))
    '() ;; termination as cur->type == plot_type_null (violated for condition)



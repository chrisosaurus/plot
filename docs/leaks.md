leaks
------

we need to check for leaks in all cases of `cons` and `plot_new_pair`

for example the following code will leak the pair from the call to `plot_new_pair`
unless `plot_new_lambda` decr's it's second argument,
which would of course be a terrible interface (relying on the called function to have side effects is ugly)

        args = cdr(name);
        name = car(name);

        value = plot_new_lambda(env, plot_new_pair( args, body ));


instead this should be rewritten as (taken from `plot_form_define`):

        args = plot_new_pair( cdr(name), body );
        name = car(name);

        value = plot_new_lambda(env, args);
        plot_value_decr(args);


notice that we do not zero out the args pair before decr-ing it
this is because plot_new_lambda keeps a copy around

usually we would have to

    lcar(args) = null;
    lcdr(args) = null;
    plot_value_decr(args);

to avoid accidentally decr-ing the contained variables


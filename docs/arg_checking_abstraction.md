argument checking abstraction
------------------------------

notes for considering possible arg checking abstraction:


+    if( args->type != plot_type_pair ){
+        return plot_runtime_error(plot_error_bad_args, "internal error, arguments not provided as a list", "plot_func_pair_list_set");
+    }
+
+    if( car(args)->type != plot_type_pair ){
+        return plot_runtime_error(plot_error_bad_args, "first argument was not a pair", "plot_func_pair_list_set");
+    }



+    if( cdr(args)->type != plot_type_pair ){
+        return plot_runtime_error(plot_error_bad_args, "only 1 argument provided, expected exactly 3", "plot_func_pair_list_set");
+    }
+
+    if( car(cdr(args))->type != plot_type_number ){
+        return plot_runtime_error(plot_error_bad_args, "second argument was not of type number", "plot_func_pair_list_set");
+    }

plot_arg_expect_3('plot_func_pair_list_set', args);

// must always be preceeded by a plot_arg_expect_n where n is >= m in plot_arg_m
// otherwise this may perform an illegal deref
plot_arg_2('plot_func_pair_list_set', args, plot_type_symbol);



this can be a set of functions rather than macros, and they can have
terribly performant _n versions which use a loop

the _n versions could even dispatch to the appropraite o(1)
version!


can use vararg

plot_check_args('location of me', args, plot_type_number, plot_type_number, plot_type_string);

will check that args contains exactly 3 arguments, the first 2 of type plot_type_number,
and the last of plot_type_string

however we do want the failure case to be handled as a runtime exception (rather than a fatal error)

so we would need

plot_value err = plot_check_args(...);
if( err ) return err;

or could have a global plot_instance->err

and then have a macro

plot_check_args(...);

that expands to

plot_instance->err = plot_internal_check_args(...);
if( plot_instance->err ) return plot_instance->err;



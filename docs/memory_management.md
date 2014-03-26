Memory management
=================

the implementation of the GC is still in progress, thus this document is a work in progress.


Basics
----------

new objects are allocated manually (by the runtime sysytem) via calls to plot_alloc_foo:

    * plot_alloc_value(void)
    * plot_alloc_env(plot_env *parent)
    * plot_alloc_hash_entry(void)
    * plot_alloc_constant(void) /* mostly-deprecated in favour of plot_value_constantify */

However you most likely want to be using the typed value creation functions to do a lot of the heavy lifting
(they internally call plot_alloc_value for you):

    plot_value * plot_new_unspecified(void);
    plot_value * plot_new_number(int value);
    plot_value * plot_new_boolean(int value);
    plot_value * plot_new_character(char value);
    plot_value * plot_new_symbol(char *val, int len);
    plot_value * plot_new_string(char * val, int len);
    plot_value * plot_new_pair(struct plot_value *car, struct plot_value *cdr);
    plot_value * plot_new_null(void);
    plot_value * plot_new_error(plot_error_type type, const char *msg, const char *location);
    plot_value * plot_new_promise(struct plot_env *env, struct plot_value *expr);
    plot_value * plot_new_lambda(struct plot_env *env, struct plot_value *body);
    plot_value * plot_new_form( struct plot_value * (*func)(struct plot_env *env, struct plot_value *sexpr), int syntactic);

whenever an object pointer is copied it must be incremented:

    * plot_value_incr(plot_value *)
    * plot_env_incr(plot_env *)
    * plot_he_incr(plot_hash_entry *)

whenever an object pointer goes out of scope it must be decremented, this may
trigger garbage collection of it (and anything it holds a reference to):

    * plot_value_decr(plot_value *)
    * plot_env_decr(plot_env *)
    * plot_he_decr(plot_hash_entry *)

plot's gc has a concept of constants, currently these are just objects with a 
refcount of -1; you can turn an object into a constant via a call to

    void plot_value_constantify(plot_value *val);

there is also a mostly-deprecated `plot_alloc_constant`, this has mostly been superseded by `plot_value_constantify`.


Auto incr/decr functions
---------------------------
Some functions modify the refcounts of their supplied arguments or returned values

non-authoritative list of some functions that modify refcounts and how they modify it:

    plot_new_promise - incrs supplied env and expr
    plot_new_lambda - incrs supplied env and bidy
    plot_hash_get - incrs fetched value
    plot_hash_set - incrs stored value
    plot_func_force - incrs returns value (from evaluating promise body)
    plot_func_make_promise - incrs generated value before storing
    plot_env_init - incrs parent env
    plot_func_pair_cons - incrs both car and cdr before storing
    plot_func_pair_car - incrs result before returning
    plot_func_pair_cdr - incrs result before returning
    plot_func_pair_set_car - incrs new value
    plot_func_pair_set_cdr - incrs new value
    plot_func_pair_list - incrs each supplied value before storing
    plot_func_pair_append - incrs each supplied value before storing
    plot_func_pair_reverse - incrs each supplied value as it stores them in a new (reversed) list
    plot_func_pair_make_list - incrs fill for each new item
    plot_func_pair_list_tail - incrs returned (sub) list
    plot_func_pair_list_ref - incrs returned value
    plot_func_pair_list_set - incrs new value
    plot_func_pair_list_copy - incrs values before copying to new list

    plot_value_decons - decrements objected referenced by collected object
    plot_hash_cleanup - decrs stored objects and hash elements on cleanup
    plot_hash_set - decrs overwritten value
    plot_func_pair_set_car - decrs old value
    plot_func_pair_set_cdr - decrs old value
    plot_func_pair_list_set - decrs old value



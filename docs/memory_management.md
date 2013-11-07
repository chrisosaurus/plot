memory management
=================

the implementation of the GC is still in progress, thus this document is a work in progress.


interface
----------

new objects are allocated manually (by the runtime sysytem) via calls to plot_alloc_foo:

    * plot_alloc_value(void)
    * plot_alloc_env(plot_env *parent)
    * plot_alloc_hash_entry(void)

However you most likely want to be using the typed value creation functions to do a lot of the heavy lifting
(they internally call plot_alloc_value for you):

    * plot_value * plot_new_number(int value);
    * plot_value * plot_new_boolean(int value);
    * plot_value * plot_new_character(char value);
    * plot_value * plot_new_symbol(char *val, int len);
    * plot_value * plot_new_string(char * val, int len);
    * plot_value * plot_new_pair(struct plot_value *car, struct plot_value *cdr);
    * plot_value * plot_new_error(plot_error_type type, const char *msg, const char *location);
    * plot_value * plot_new_lambda(struct plot_env *env, struct plot_sexpr *body);
    * plot_value * plot_new_builtin( struct plot_value * (*func)(struct plot_env *env, struct plot_value **args, int argc) );

whenever an object pointer is copied it must be incremented:

    * plot_value_incr(plot_value *)
    * plot_env_incr(plot_env *)
    * plot_he_incr(plot_hash_entry *)

whenever an object pointer goes out of scope it must be decremented, this may
trigger garbage collection of it (and anything it holds a reference to):

    * plot_value_decr(plot_value *)
    * plot_env_decr(plot_env *)
    * plot_he_decr(plot_hash_entry *)




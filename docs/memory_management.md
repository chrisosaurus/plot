memory management
=================

the implementation of the GC is still in progress, thus this document is a work in progress.


interface
----------

new objects are allocated manually (by the runtime sysytem) via calls to plot_new_foo:

    * plot_new_value(void)
    * plot_new_env(plot_env *parent)
    * plot_new_hash_entry(void)

whenever an object pointer is copied it must be incremented:

    * plot_value_incr(plot_value *)
    * plot_env_incr(plot_env *)
    * plot_he_incr(plot_hash_entry *)

whenever an object pointer goes out of scope it must be decremented, this may
trigger garbage collection of it (and anything it holds a reference to):

    * plot_value_decr(plot_value *)
    * plot_env_decr(plot_env *)
    * plot_he_decr(plot_hash_entry *)




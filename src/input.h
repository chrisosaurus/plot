#ifndef PLOT_INPUT_H
#define PLOT_INPUT_H

/* header for 6.13.1 input procedures */

/* (eof-object)
 * returns an end-of-file object
 * not necessarily unique
 */
struct plot_value * plot_func_input_eof_object(struct plot_env *env, struct plot_value *args);

/* (eof-object? obj)
 * returns #t iff obj is an end-of-file object
 * otherwise returns #f
 */
struct plot_value * plot_func_input_eof_object_test(struct plot_env *env, struct plot_value *args);

/* (open-input-file string)
 * takes a string for an existing file name and returns a textual output port that
 * is capable of delivering data from the file.
 *
 * if the file does not exist or cannot be opened an error that satisfies `file-error?` is signaled
 */
struct plot_value * plot_func_input_open_input_file(struct plot_env *env, struct plot_value *args);


#endif

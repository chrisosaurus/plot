#ifndef PLOT_PORTS_H
#define PLOT_PORTS_H

/* (open-input-file string)
 * takes a string for an existing file and returs a textual input port
 * that is apable of delivering data from the file.
 * if the file does not exist or cannot be opened, an error
 * that satisfies `file-error?` is signaled
 */
struct plot_value * plot_func_ports_open_input_file(struct plot_env *env, struct plot_value *args);

/* (open-output-file string)
 * takes a string naming an output file to be created
 * returns a textual output port that is capable of writing data to the new file
 *
 * if a file already exists the effect is unspecified by r7rs, plot will append
 * if the file cannot be opened an error satifying `file-error?` is returned
 */
struct plot_value * plot_func_ports_open_output_file(struct plot_env *env, struct plot_value *args);

/* (close-port port)
 * closes the resource associated with port, rendering the port incapable of delivering or accepting data
 * routine has no effect if port is already closed
 */
struct plot_value * plot_func_ports_close_port(struct plot_env *env, struct plot_value *args);

/* (close-output-port port)
 * closes the resource associated with port, rendering the port incapable of delivering or accepting data
 * routine has no effect if port is already closed
 *
 * throws a runtime exception if the port is not an output port
 */
struct plot_value * plot_func_ports_close_output_port(struct plot_env *env, struct plot_value *args);

/* (close-input-port port)
 * closes the resource associated with port, rendering the port incapable of delivering or accepting data
 * routine has no effect if port is already closed
 *
 * throws a runtime exception if the port is not an input port
 */
struct plot_value * plot_func_ports_close_input_port(struct plot_env *env, struct plot_value *args);

/* (input-port? obj)
 * returns #t iff obj is an input port
 * otherwise returns #f
 */
struct plot_value * plot_func_ports_input_port_test(struct plot_env *env, struct plot_value *args);

/* (output-port? obj)
 * returns #t iff obj is an output port
 * otherwise returns #f
 */
struct plot_value * plot_func_ports_output_port_test(struct plot_env *env, struct plot_value *args);

/* (textual-port? obj)
 * returns #t iff obj is a textual port
 * otherwise returns #f
 */
struct plot_value * plot_func_ports_textual_port_test(struct plot_env *env, struct plot_value *args);

/* (binary-port? obj)
 * returns #t iff obj is a binary port
 * otherwise returns #f
 */
struct plot_value * plot_func_ports_binary_port_test(struct plot_env *env, struct plot_value *args);

/* (port? obj)
 * returns #t iff obj is a port
 * otherwise returns #f
 */
struct plot_value * plot_func_ports_port_test(struct plot_env *env, struct plot_value *args);

/* (input-port-open? port)
 * return #t iff port is still open and capable of performing input
 * otherwise #f
 */
struct plot_value * plot_func_ports_input_port_open_test(struct plot_env *env, struct plot_value *args);

/* (output-port-open? port)
 * return #t iff port is still open and capable of performing output
 * otherwise #f
 */
struct plot_value * plot_func_ports_output_port_open_test(struct plot_env *env, struct plot_value *args);

#endif

#ifndef PLOT_PORTS_H
#define PLOT_PORTS_H

/* (open-output-file string)
 * takes a string naming an output file to be created
 * returns a textual output port that is capable of writing data to the new file
 *
 * if a file already exists the effect is unspecified by r7rs, plot will append
 * if the file cannot be opened an error satifying `file-error?` is returned
 */
struct plot_value * plot_func_ports_open_output_file(struct plot_env *env, struct plot_value *args);

/* (close-port port)
 * closes the resource assosiated with port, rendering the port incapable of delivering or accepting data
 * routine has no effect is port is already closed
 */
struct plot_value * plot_func_ports_close_port(struct plot_env *env, struct plot_value *args);

#endif

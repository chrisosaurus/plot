#include "value.h"
#include "plot.h"
#include "ports.h"

#define DEBUG 0

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

/* (open-input-file string)
 * takes a string for an existing file and returs a textual input port
 * that is apable of delivering data from the file.
 * if the file does not exist or cannot be opened, an error
 * that satisfies `file-error?` is signaled
 */
struct plot_value * plot_func_ports_open_input_file(struct plot_env *env, struct plot_value *args){
    FILE *file;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_ports_open_input_file");
    }

    if( car(args)->type != plot_type_string ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_string", "plot_func_ports_open_input_file");
    }

    file = fopen(car(args)->u.string.val, "r");

    if( ! file ){
        /* FIXME need to decode error */
        /* FIXME current error will not satisfy `file-error?` */
        return plot_runtime_error(plot_error_internal, "failed to open file", "plot_func_ports_open_input_file");
    }

    return plot_new_textual_port(plot_port_in, file);
}

/* (open-output-file string)
 * takes a string naming an output file to be created
 * returns a textual output port that is capable of writing data to the new file
 *
 * if a file already exists the effect is unspecified by r7rs, plot will append
 * if the file cannot be opened an error satifying `file-error?` is returned
 */
struct plot_value * plot_func_ports_open_output_file(struct plot_env *env, struct plot_value *args){
    FILE *file;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_ports_open_output_file");
    }

    if( car(args)->type != plot_type_string ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_string", "plot_func_ports_open_output_file");
    }

    file = fopen(car(args)->u.string.val, "a");

    if( ! file ){
        /* FIXME need to decode error */
        /* FIXME current error will not satisfy `file-error?` */
        return plot_runtime_error(plot_error_internal, "failed to open file", "plot_func_ports_open_output_file");
    }

    return plot_new_textual_port(plot_port_out, file);
}
/* (close-port port)
 * closes the resource assosiated with port, rendering the port incapable of delivering or accepting data
 * routine has no effect is port is already closed
 */
struct plot_value * plot_func_ports_close_port(struct plot_env *env, struct plot_value *args){
    plot_value *port;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_ports_close_port");
    }

    port = car(args);
    switch( port->type ){
        case plot_type_textual_port:
            /* make sure to only call fclose if file is open
             * as it's behavior is unspecified otherwise
             */
            if( port->u.textport.status == plot_port_open ){
                port->u.textport.status = plot_port_closed;
                fclose( port->u.textport.file );
                port->u.textport.file = 0;
            }
            break;
        case plot_type_binary_port:
            /* make sure to only call fclose if file is open
             * as it's behavior is unspecified otherwise
             */
            if( port->u.binport.status == plot_port_open ){
                port->u.binport.status = plot_port_closed;
                fclose( port->u.binport.file );
                port->u.binport.file = 0;
            }
            break;
        default:
            return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_textual_port", "plot_func_ports_close_port");
            break;
    }

    return plot_new_unspecified();
}

/* (input-port? obj)
 * returns #t iff obj is an input port
 * otherwise returns #f
 */
struct plot_value * plot_func_ports_input_port_test(struct plot_env *env, struct plot_value *args){
    plot_value *port;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_ports_input_port_test");
    }

    port = car(args);
    switch( port->type ){
        case plot_type_textual_port:
            return plot_new_boolean( port->u.textport.direction == plot_port_in );
            break;
        case plot_type_binary_port:
            return plot_new_boolean( port->u.binport.direction == plot_port_in );
            break;
        default:
            return plot_new_boolean( 0 );
            break;
    }
}

/* (output-port? obj)
 * returns #t iff obj is an output port
 * otherwise returns #f
 */
struct plot_value * plot_func_ports_output_port_test(struct plot_env *env, struct plot_value *args){
    plot_value *port;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_ports_output_port_test");
    }

    port = car(args);
    switch( port->type ){
        case plot_type_textual_port:
            return plot_new_boolean( port->u.textport.direction == plot_port_out );
            break;
        case plot_type_binary_port:
            return plot_new_boolean( port->u.binport.direction == plot_port_out );
            break;
        default:
            return plot_new_boolean( 0 );
            break;
    }
}

/* (textual-port? obj)
 * returns #t iff obj is a textual port
 * otherwise returns #f
 */
struct plot_value * plot_func_ports_textual_port_test(struct plot_env *env, struct plot_value *args){
    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_ports_textual_port_test");
    }

    return plot_new_boolean( car(args)->type == plot_type_textual_port );
}

/* (binary-port? obj)
 * returns #t iff obj is a binary port
 * otherwise returns #f
 */
struct plot_value * plot_func_ports_binary_port_test(struct plot_env *env, struct plot_value *args){
    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_ports_binary_port_test");
    }

    return plot_new_boolean( car(args)->type == plot_type_binary_port );
}

/* (port? obj)
 * returns #t iff obj is a port
 * otherwise returns #f
 */
struct plot_value * plot_func_ports_port_test(struct plot_env *env, struct plot_value *args){
    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_ports_port_test");
    }

    return plot_new_boolean( car(args)->type == plot_type_textual_port || car(args)->type == plot_type_binary_port );
}

/* (input-port-open? port)
 * return #t iff port is still open and capable of performing input
 * otherwise #f
 */
struct plot_value * plot_func_ports_input_port_open_test(struct plot_env *env, struct plot_value *args){
    plot_value *val;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_ports_input_port_open_test");
    }

    val = car(args);

    if( val->type == plot_type_textual_port ){
        return plot_new_boolean( val->u.textport.status == plot_port_open && val->u.textport.direction == plot_port_in );
    } else if( val->type == plot_type_binary_port ){
        return plot_new_boolean( val->u.binport.status == plot_port_open && val->u.binport.direction == plot_port_in );
    } else {
        return plot_new_boolean( 0 );
    }
}

/* (output-port-open? port)
 * return #t iff port is still open and capable of performing output
 * otherwise #f
 */
struct plot_value * plot_func_ports_output_port_open_test(struct plot_env *env, struct plot_value *args){
    plot_value *val;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_ports_output_port_open_test");
    }

    val = car(args);

    if( val->type == plot_type_textual_port ){
        return plot_new_boolean( val->u.textport.status == plot_port_open && val->u.textport.direction == plot_port_out );
    } else if( val->type == plot_type_binary_port ){
        return plot_new_boolean( val->u.binport.status == plot_port_open && val->u.binport.direction == plot_port_out );
    } else {
        return plot_new_boolean( 0 );
    }
}



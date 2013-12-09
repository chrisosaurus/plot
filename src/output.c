#include <stdlib.h> /* exit */

#include "value.h"
#include "hash.h"
#include "env.h"
#include "output.h"
#include "plot.h"

#define DEBUG 0

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

/* internal routine for displaying a value
 * returns 0 on failure, 1 on success
 */
static plot_value * plot_func_display_value(FILE *file, plot_env *env, plot_value *val){
    if( ! val )
        return 0;

    switch(val->type){
        case plot_type_boolean:
            fprintf(file, "#%c", val->u.boolean.val ? 't' : 'f' );
            break;
        case plot_type_number:
            fprintf(file, "%d", val->u.number.val);
            break;
        case plot_type_string:
            fprintf(file, "%s", val->u.string.val);
            break;
        case plot_type_symbol:
            fprintf(file, "%s", val->u.symbol.val);
            break;
        case plot_type_character:
            fprintf(file, "%c", val->u.character.val);
            break;
        case plot_type_null:
            fputs("()", file);
            break;
        case plot_type_pair:
            fputs("(", file);
            plot_func_display_value(file, env, val->u.pair.car);
            fputs(" ", file);
            plot_func_display_value(file, env, val->u.pair.cdr);
            fputs(")", file);
            break;
            break;
        case plot_type_form:
            puts("Unable to print a form at this point in time");
            break;
        case plot_type_error:
            return plot_runtime_error(plot_error_internal, "trying to print an error value", "plot_func_display_value");
            break;
        case plot_type_lambda:
            puts("Unable to print a lambda value at this point in time");
            break;
        case plot_type_textual_port:
            return plot_runtime_error(plot_error_internal, "trying to print a textual port", "plot_func_display_value");
            break;
        case plot_type_eof:
            fputs("<eof>", file);
            break;
        case plot_type_unspecified:
            fputs("<unspecified>", file);
            break;
        case plot_type_reclaimed:
            puts("ERROR: you are trying to display a garbage collected value, most likely an error in the GC");
            exit(1);
            break;
        default:
            return plot_runtime_error(plot_error_internal, "impossible type for val->type", "plot_func_display_value");
            break;
    }

    return plot_new_unspecified();
}

/* (display obj)
 * (display obj port)
 * print value to provided port or to stdout if no port is provided
 */
plot_value * plot_func_display(plot_env *env, plot_value *args){
    plot_value *arg;
    /* FIXME need to use current output rather than just stdout */
    FILE *file = stdout;

    if( args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected either 1 or 2 args", "plot_func_display");
    }

    arg = cdr(args);
    if( arg->type == plot_type_pair && car(arg)->type == plot_type_textual_port ){
        arg = car(arg);
        if( arg->u.textport.status != plot_port_open ){
            return plot_runtime_error(plot_error_bad_args, "supplied port was not open", "plot_func_display");
        }
        if( arg->u.textport.direction != plot_port_out ){
            return plot_runtime_error(plot_error_bad_args, "supplied port was not an output port", "plot_func_display");
        }
        file = arg->u.textport.file;
    } else if( arg->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "second arg was of unexpected type", "plot_func_display");
    }

    arg = car(args);

    if( ! arg ){
        return plot_runtime_error(plot_error_bad_args, "first arg was null", "plot_func_display");
    }

    return plot_func_display_value(file, env, arg);
}

/* (newline)
 * (newline port)
 * print a newline to provided port or to stdout if no port is provided
 */
plot_value * plot_func_newline(plot_env *env, plot_value *args){
    /* FIXME need to use current output rather than just stdout */
    FILE *file = stdout;
    plot_value *arg;

    if( args->type == plot_type_pair && cdr(args)->type == plot_type_null ){
        arg = car(args);
        if( arg->type != plot_type_textual_port ){
            return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_textual_port", "plot_func_newline");
        }
        if( arg->u.textport.status != plot_port_open ){
            return plot_runtime_error(plot_error_bad_args, "supplied port was not open", "plot_func_display");
        }
        if( arg->u.textport.direction != plot_port_out ){
            return plot_runtime_error(plot_error_bad_args, "supplied port was not an output port", "plot_func_display");
        }
        file = arg->u.textport.file;
    } else if( args->type == plot_type_null ){
    } else {
        return plot_runtime_error(plot_error_bad_args, "expected either 0 or 1 arguments", "plot_func_newline");
    }

    fputs("\n", file);

    return plot_new_unspecified();
}

/* (write-char char)
 * (write-char char port)
 * writes the character char (not an external representation of the char) to the given textual output port
 * returns an unspecified value
 */
struct plot_value * plot_func_output_write_char(struct plot_env *env, struct plot_value *args){
    /* FIXME need to use current output rather than just stdout */
    FILE *file = stdout;

    if( args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected either 1 or 2 args", "plot_func_output_write_char");
    }

    if( car(args)->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_output_write_char");
    }

    if( cdr(args)->type == plot_type_pair ){
        if( cdr(cdr(args))->type != plot_type_null ){
            return plot_runtime_error(plot_error_bad_args, "too many args, expected either 1 or 2", "plot_func_output_write_char");
        }
        if( car(cdr(args))->type != plot_type_textual_port ){
            return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_textual_port", "plot_func_output_write_char");
        }
        file = car(cdr(args))->u.textport.file;
    }

    fprintf(file, "%c", car(args)->u.character.val);
    return plot_new_unspecified();
}

/* (write-string string)
 * (write-string string port)
 * (write-string string port start)
 * (write-string string port start end)
 * writes the characters of string from start to end in left-to-right order to the textual
 * output port provided (or current-output-port)
 * if start of end are not provided then 0 and string-length are used
 */
struct plot_value * plot_func_output_write_string(struct plot_env *env, struct plot_value *args){
    char *str;
    FILE *file = stdout;
    int start = 0;
    int end = 0;
    plot_value *arg;

    if( args->type != plot_type_pair ){
        return plot_runtime_error(plot_error_bad_args, "expected at least 1 argument, received none", "plot_func_output_write_string");
    }

    if( car(args)->type != plot_type_string ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_string", "plot_func_output_write_string");
    }

    str = car(args)->u.string.val;
    end = car(args)->u.string.len - 1; /* \0 terminator */

    arg = cdr(args);
    if( arg->type != plot_type_null ){
        if( car(arg)->type != plot_type_textual_port ){
            return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_textual_port", "plot_func_output_write_string");
        }
        /* grab port */
        file = car(arg)->u.textport.file;
        arg = cdr(arg);

        if( arg->type != plot_type_null ){
            if( car(arg)->type != plot_type_number ){
                return plot_runtime_error(plot_error_bad_args, "third arg was not of type plot_type_number", "plot_func_output_write_string");
            }
            /* grab start */
            start = car(arg)->u.number.val;
            arg = cdr(arg);

            if( arg->type != plot_type_null ){
                if( car(arg)->type != plot_type_number ){
                    return plot_runtime_error(plot_error_bad_args, "fourth arg was not of type plot_type_number", "plot_func_output_write_string");
                }
                /* grab end */
                end = car(arg)->u.number.val;
                arg = cdr(arg);

                if( arg->type != plot_type_null ){
                    return plot_runtime_error(plot_error_bad_args, "expected 1 to 4 argument, received more", "plot_func_output_write_string");
                }
            }
        }
    }

    if( start < 0 ){
        return plot_runtime_error(plot_error_bad_args, "start must be positive", "plot_func_output_write_string");
    }

    if( end < 0 || end >= car(args)->u.string.len ){
        return plot_runtime_error(plot_error_bad_args, "end is not a valid index into string", "plot_func_output_write_string");
    }

    for( ; start < end; ++start ){
        fprintf( file, "%c", str[start] );
    }

    return plot_new_unspecified();
}


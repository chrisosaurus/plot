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

/* (read-string k)
 * (reading-string k port)
 * reads in the next k characters, or as manu are available before eof,
 * from the textual input port into the newly allocated string in left-to-right order
 * and returns the string.
 *
 * if no characters are available before the eof, and eof object is returned
 */
struct plot_value * plot_func_input_read_string(struct plot_env *env, struct plot_value *args);

/* (read-line)
 * (read-line port)
 * returns the next line of text available from the textual input port
 * will either return string or eof, string may not contain an end-of-line char if
 * eof was encountered before end-of-line
 */
struct plot_value * plot_func_input_read_line(struct plot_env *env, struct plot_value *args);

/* (read-char)
 * (read-char port)
 * returns the next character available from the textual input port
 * returns eof object if no more characters are available
 */
struct plot_value * plot_func_input_read_char(struct plot_env *env, struct plot_value *args);

/* (peek-char)
 * (peek-char port)
 * returns the next character available from the textual input port without
 * updating the file pointer
 * returns eof if no more characters are available
 */
struct plot_value * plot_func_input_peek_char(struct plot_env *env, struct plot_value *args);

/* (char-ready?)
 * (char-ready? port)
 * return #t if a character is ready on the textual input port
 * otherwise returns #f
 *
 * if char-ready? returns #t then the next read-char operation on the given port is
 * guranteed not to hang.
 *
 * if port is at eof then char-ready? returns #t
 */
struct plot_value * plot_func_input_char_ready_test(struct plot_env *env, struct plot_value *args);

#endif

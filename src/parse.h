#ifndef PLOT_PARSE_H
#define PLOT_PARSE_H

struct plot_value;

/* provides an interface to convert a string to a plot_value (list)
 * return a plot_value * or 0 for error
 */
struct plot_value * plot_parse(const char *source);

/* plot_parse_expr will consume a token upto a separator (but will not consume the separator)
 * *upto is an offset into the source
 * *upto represents where plot_parse_sexpr starts and it will update it to match where it got up to
 * *expr is pre-allocated location to save parsed expr
 * return a plot_value * (list) or 0 for errors
 * */
struct plot_value * plot_parse_expr(const char *source, size_t *upto);

/* plot_parse_sexpr will consume a token upto and including the matching close paren
 * *upto is an offset into the source
 * *upto represents where plot_parse_sexpr starts and it will update it to match where it got up to
 * *sexpr is the allocated location to save parsed s_expr
 * return the plot_value * (list) or 0 for errors
 */
struct plot_value * plot_parse_sexpr(const char *source, size_t *upto);

#endif

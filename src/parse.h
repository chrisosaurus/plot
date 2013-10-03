#ifndef PLOT_PARSE_H
#define PLOT_PARSE_H

struct plot_program;
struct plot_expr;
struct plot_sexpr;

/* provides an interface to convert a string to a plot_program (AST)
 * return a plot_program* or 0 for error
 */
struct plot_program * plot_parse(char *source);

/* plot_parse_expr will consume a token upto a separator (but will not consume the separator)
 * *upto is an offset into the source
 * *upto represents where plot_parse_sexpr starts and it will update it to match where it got up to
 * *expr is pre-allocated location to save parsed expr
 * return a plot_expr* (same as *expr) or 0 for errors
 * */
struct plot_expr * plot_parse_expr(struct plot_expr *expr, char *source, int *upto);

/* plot_parse_sexpr will consume a token upto and including the matching close paren
 * *upto is an offset into the source
 * *upto represents where plot_parse_sexpr starts and it will update it to match where it got up to
 * *sexpr is the allocated location to save parsed s_expr
 * return the plot_sexpr* (same as *sexpr) or 0 for errors
 */
struct plot_sexpr * plot_parse_sexpr(struct plot_sexpr *sexpr, char *source, int *upto);

#endif

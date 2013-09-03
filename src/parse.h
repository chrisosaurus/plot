#ifndef PLOT_PARSE_H
#define PLOT_PARSE_H

#include "types.h"

/* provides an interface to convert a string to an AST */
/* return an plot_program* or 0 for error */
plot_program * plot_parse(char *source);

/* plot_parse_sexpr will consume a token upto a the matching close paren and will consume that close paren */
plot_sexpr * plot_parse_sexpr(char *source, int *upto);

/* plot_parse_expr will consume a token upto a separator but will not consume the separator */
plot_expr * plot_parse_expr(char *source, int *upto);

#endif

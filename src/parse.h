#ifndef PLOT_PARSE_H
#define PLOT_PARSE_H

#include "types.h"

/* provides an interface to convert a string to an AST */
/* return an AST* or 0 for error */
plot_program * plot_parse(char *source);

plot_sexpr * plot_parse_sexpr(char *source, int *upto);

plot_expr * plot_parse_expr(char *source, int *upto);

#endif

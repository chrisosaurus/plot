#ifndef PLOT_TYPES_H
#define PLOT_TYPES_H

/* declaration of types used for parsing and execution */

#include "value.h"

/* type of expression */
typedef enum plot_expr_type {
    plot_expr_sexpr,
    plot_expr_value,
} plot_expr_type;


struct plot_expr;

/* an s-expr is list of expr */
typedef struct plot_sexpr {
    struct plot_expr * subforms;
} plot_sexpr;

/* a single expression may either be a value of an sexpr */
typedef struct plot_expr {
    plot_expr_type type;
    union {
        plot_sexpr sexpr;
        plot_value value;
    } u;
} plot_expr;

/* a program is list of s-expressions */
typedef struct plot_program {
    plot_sexpr * sexprs;
    int nelems;
} plot_program;

#endif

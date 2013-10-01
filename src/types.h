#ifndef PLOT_TYPES_H
#define PLOT_TYPES_H

/* requires value.h */


/* declaration of types used for parsing and execution */

/* type of expression */
typedef enum plot_expr_type {
    plot_expr_sexpr,
    plot_expr_value,
} plot_expr_type;


struct plot_expr;

/* an s-expr is list of expr */
typedef struct plot_sexpr {
    struct plot_expr * subforms;
    int nchildren;
} plot_sexpr;

/* a single expression may either be a value of an sexpr */
typedef struct plot_expr {
    plot_expr_type type;
    union {
        plot_sexpr sexpr;
        plot_value value;
    } u;
} plot_expr;

/* a program is list of expressions */
typedef struct plot_program {
    plot_expr * exprs;
    int nchildren;
    int max_children;
} plot_program;

#endif

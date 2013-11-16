#ifndef PLOT_FORMS_H
#define PLOT_FORMS_H

struct plot_env;
struct plot_sexpr;
struct plot_value;

/* (begin body...)
 */
struct plot_value * plot_form_begin(struct plot_env *env, struct plot_sexpr *sexpr);

/* (define what value)
 */
struct plot_value * plot_form_define(struct plot_env *env, struct plot_sexpr *sexpr);

/* (lambda args body...)
 */
struct plot_value * plot_form_lambda(struct plot_env *env, struct plot_sexpr *sexpr);

/* (if cond if-expr else-expr)
 * (if cond if-expr)
 */
struct plot_value * plot_form_if(struct plot_env *env, struct plot_sexpr *sexpr);

/* (set! variable value)
 */
struct plot_value * plot_form_set(struct plot_env *env, struct plot_sexpr *sexpr);

#endif

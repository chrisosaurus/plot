#ifndef PLOT_FORMS_H
#define PLOT_FORMS_H

struct plot_env;
struct plot_sexpr;
struct plot_value;

/* (define-library <library-definition> body...) -core -syntax
 * define a library
 */
struct plot_value * plot_form_define_library(struct plot_env *env, struct plot_sexpr *sexpr);

/* (import <import-set> ...) -core -syntax
 * import some libraries
 */
struct plot_value * plot_form_import(struct plot_env *env, struct plot_sexpr *sexpr);

/* (export identifier ... ) -core -syntax
 * export some identifiers
 */
struct plot_value * plot_form_export(struct plot_env *env, struct plot_sexpr *sexpr);

/* (plot-bind identifier ... ) -syntax
 * looks through plot's internal bindings and binds
 * identifiers to current scope
 *
 * eventually exported by (plot internal)
 */
struct plot_value * plot_form_plot_bind(struct plot_env *env, struct plot_sexpr *sexpr);

/* (begin body...) -syntax
 */
struct plot_value * plot_form_begin(struct plot_env *env, struct plot_sexpr *sexpr);

/* (define what value) -syntax
 */
struct plot_value * plot_form_define(struct plot_env *env, struct plot_sexpr *sexpr);

/* (lambda args body...) -syntax
 */
struct plot_value * plot_form_lambda(struct plot_env *env, struct plot_sexpr *sexpr);

/* (if cond if-expr else-expr) -syntax
 * (if cond if-expr)
 */
struct plot_value * plot_form_if(struct plot_env *env, struct plot_sexpr *sexpr);

/* (set! variable value) -syntax
 */
struct plot_value * plot_form_set(struct plot_env *env, struct plot_sexpr *sexpr);

/* (quote expr) -syntax
 */
struct plot_value * plot_form_quote(struct plot_env *env, struct plot_sexpr *sexpr);

/* (delay expr) -syntax
 */
struct plot_value * plot_form_delay(struct plot_env *env, struct plot_sexpr *sexpr);


#endif

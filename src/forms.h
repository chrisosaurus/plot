#ifndef PLOT_FORMS_H
#define PLOT_FORMS_H

struct plot_env;
struct plot_value;

/* (define-library (library name) body...) -core -syntax
 * define a library
 *
 * library definitions are of the form
 * (define-library <library-name>
 *      <library-declaration> ...)
 *
 * where a <library-declaration> is any of:
 *      (export <export spec> ...)
 *      (import <import set> ...)
 *      (begin <command or defintion> ...)
 *      (include <filename1> <filename2> ...)
 *      (include-ci <filename1> <filename2> ...)
 *      (include-library-declaration <filename1> <filename2> ...)
 *      (cond-expand <ce-clause1> <ce-clause2> ...)
 */
struct plot_value * plot_form_define_library(struct plot_env *env, struct plot_value *sexpr);

/* (import <import-set> ...) -core -syntax
 * import some libraries
 */
struct plot_value * plot_form_import(struct plot_env *env, struct plot_value *sexpr);

/* (export identifier ... ) -core -syntax
 * export some identifiers
 */
struct plot_value * plot_form_export(struct plot_env *env, struct plot_value *sexpr);

/* (plot-bind identifier ... ) -syntax
 * looks through plot's internal bindings and binds
 * identifiers to current scope
 *
 * eventually exported by (plot internal)
 */
struct plot_value * plot_form_plot_bind(struct plot_env *env, struct plot_value *sexpr);

/* (begin body...) -syntax
 */
struct plot_value * plot_form_begin(struct plot_env *env, struct plot_value *sexpr);

/* (define what value) -syntax
 */
struct plot_value * plot_form_define(struct plot_env *env, struct plot_value *sexpr);

/* (lambda args body...) -syntax
 */
struct plot_value * plot_form_lambda(struct plot_env *env, struct plot_value *sexpr);

/* (if cond if-expr else-expr) -syntax
 * (if cond if-expr)
 */
struct plot_value * plot_form_if(struct plot_env *env, struct plot_value *sexpr);

/* (cond (<test> <expression>)...) -syntax
 *
 * (cond ((> 3 2) 'greater)
 *       ((< 3 2) => 'less)
 *       (else 'equal))
 *
 * a cond is a set of <test> <expression> pairs which are tried in order:
 *   if a <test> results in #t (or a truthy value) then expression is eval-ed
 *     and the result of the cond expression is the result of this expression.
 *
 *   else is always true.
 *
 *   if all <test>s are tried and found to be false then the value of the cond
 *     is unspecified.
 *
 *   possible forms:
 *      ((< 3 2) 'greater)
 *      (else 'equal)
 *      ((> 3 2) => 'less)
 *
 */
struct plot_value * plot_form_cond(struct plot_env *env, struct plot_value *sexpr);

/* (set! variable value) -syntax
 */
struct plot_value * plot_form_set(struct plot_env *env, struct plot_value *sexpr);

/* (quote expr) -syntax
 */
struct plot_value * plot_form_quote(struct plot_env *env, struct plot_value *sexpr);

/* (delay expr) -syntax
 */
struct plot_value * plot_form_delay(struct plot_env *env, struct plot_value *sexpr);


#endif

#ifndef PLOT_VALUE_H
#define PLOT_VALUE_H

#include <stdbool.h>

typedef enum plot_value_type{
    plot_type_number,
    plot_type_symbol,
    /* a function written in plot */
    plot_type_lambda,
    /* a syntactic form written in c */
    plot_type_form,
    plot_type_error,
    plot_type_string,
    plot_type_boolean,
    plot_type_character,
    plot_type_pair,
    plot_type_promise,
    /* null does NOT have a matching union member */
    plot_type_null,
    /* this type represents expressions that do not yield a value
     * '(if #f "hello")' => unspecified
     * '(define a "world")' => unspecified
     *
     * this type does NOT have a matching
     * union member
     */
    plot_type_unspecified,
    plot_type_reclaimed /* FIXME useful for testing garbage collection */
#if 0
    plot_type_vector
    plot_type_port
#endif
} plot_value_type;

typedef enum plot_error_type{
    /* allocation failed */
    plot_error_alloc_failed,
    /* arguments provided were incorrect */
    plot_error_bad_args,
    /* unknown internal error, in theory an impossible state */
    plot_error_internal,
    /* unbound symbol encountered */
    plot_error_unbound_symbol,
    /* runtime error in source program */
    plot_error_runtime,

    /* function not implemented yet */
    plot_error_unimplemented,
    /* symbol failed to resolved */
    plot_error_undefined
} plot_error_type;

typedef struct plot_error {
    plot_error_type type;
    const char *msg;
    const char *location;
} plot_error;

typedef struct plot_character {
    char val;
} plot_character;

typedef struct plot_number {
    int val;
} plot_number;

typedef struct plot_symbol {
    const char *val;
    /* len is number of characters used
     * including \0 terminator
     */
    int len;
    /* size is number of characters allocated */
    int size;
    /* hash for this symbol's value, 0 means unhashed */
    unsigned long long hash;
} plot_symbol;

typedef struct plot_string {
    char *val;
    /* len is number of characters used
     * including \0 terminator
     */
    int len;
    /* size is number of characters allocated */
    int size;
} plot_string;

typedef struct plot_boolean {
    bool val;
} plot_boolean;

typedef struct plot_pair {
    struct plot_value *car;
    struct plot_value *cdr;
} plot_pair;

/* FIXME may want to move to functions that check types
 */
#define cons(l,r) (plot_new_pair(l,r))
#define car(v) ((v)->u.pair.car)
#define cdr(v) ((v)->u.pair.cdr)
#define null (plot_new_null())

#if 0

typedef struct plot_vector {
} plot_vector;

typedef struct plot_port {
} plot_port;

#endif

struct plot_expr;
struct plot_sexpr;
struct plot_env;

typedef struct plot_lambda {
    struct plot_env *env;
    struct plot_value *body;
} plot_lambda;

typedef struct plot_form {
    /* env to evaluate within
     * args is a plot list
     * argc is the number of them
     */
    struct plot_value * (*func)(struct plot_env *env, struct plot_value *sexpr);
    /* flag is 1 is this form is syntactic, otherwise 0
     * a syntactic form receives it's argument un-evaluated
     */
    int syntactic;
} plot_form;

/* a promise is the value type returned by delay
 * value is set IFF the body has already been evaluated
 * the body must only be evaluated once and value must be set to the result
 * subsequent calls return value
 */
typedef struct plot_promise {
    struct plot_env *env;
    struct plot_value *expr;
    struct plot_value *value;
} plot_promise;

#include "plot.h" /* needed for plot_gc */

typedef struct plot_value {
    struct plot_gc gc;
    plot_value_type type;
    union {
        plot_number    number;
        plot_symbol    symbol;
        plot_promise   promise;
        plot_lambda    lambda;
        plot_form      form;
        plot_error     error;
        plot_string    string;
        plot_boolean   boolean;
        plot_character character;
        plot_pair      pair;
#if 0
        plot_vector    vector;
        plot_plot      port;
#endif
    } u;
} plot_value;

/* plot_value deconstructor
 * correctly decr any values held within value
 */
void plot_value_decons(plot_value *value);

/* functions to allocate and setup a new plot_value
 */
plot_value * plot_new_unspecified(void);
plot_value * plot_new_number(int value);
plot_value * plot_new_boolean(int value);
plot_value * plot_new_character(char value);
plot_value * plot_new_symbol(char *val, int len);
plot_value * plot_new_string(char * val, int len);
plot_value * plot_new_pair(struct plot_value *car, struct plot_value *cdr);
plot_value * plot_new_null(void);
plot_value * plot_new_error(plot_error_type type, const char *msg, const char *location);
plot_value * plot_new_promise(struct plot_env *env, struct plot_value *expr);
plot_value * plot_new_lambda(struct plot_env *env, struct plot_value *body);
plot_value * plot_new_form( struct plot_value * (*func)(struct plot_env *env, struct plot_value *sexpr), int syntactic);

/* turn an existing plot_value into a constant
 */
void plot_value_constantify(plot_value *val);


/* function to hash symbols
 *
 * a fatal error will be thrown if an invalid symbol is passed in
 *
 * function will not modify existing hash
 */
void plot_hash_symbol(plot_symbol *val);

void display_type(plot_value *val);
void display_expr(plot_value * sexpr);
void display_error_expr(plot_value *expr);

#endif

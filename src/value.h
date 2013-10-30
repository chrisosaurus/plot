#ifndef PLOT_VALUE_H
#define PLOT_VALUE_H

#include <stdbool.h>

typedef enum plot_value_type{
    plot_type_number,
    plot_type_symbol,
    plot_type_lambda,
    plot_type_builtin,
    plot_type_error,
    plot_type_string,
    plot_type_boolean,
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
    plot_type_character
    plot_type_pair
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

    plot_error_unimplemented
} plot_error_type;

typedef struct plot_error {
    plot_error_type type;
    const char *msg;
    const char *location;
} plot_error;

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
} plot_symbol;

typedef struct plot_string {
    /* FIXME scheme strings are mutable */
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

#if 0
typedef struct plot_character {
    char val;
} plot_character;

typedef struct plot_pair {
    struct plot_value *left;
    struct plot_value *right;
} plot_pair;

#endif

struct plot_expr;
struct plot_sexpr;
struct plot_env;

typedef struct plot_lambda {
    struct plot_env *env;
    const struct plot_sexpr *body;
} plot_lambda;

typedef struct plot_builtin {
    /* env to evaluate within
     * args is an array of plot_value *(s) to apply function to
     * argc is the number of them
     */
    struct plot_value * (*func)(struct plot_env *env, struct plot_value **args, int argc);
} plot_builtin;

#include "plot.h" /* needed for plot_gc */

typedef struct plot_value {
    struct plot_gc gc;
    plot_value_type type;
    union {
        plot_number    number;
        plot_symbol    symbol;
        plot_lambda    lambda;
        plot_builtin   builtin;
        plot_error     error;
        plot_string    string;
        plot_boolean   boolean;
#if 0
        plot_character character;
        plot_pair      pair;
#endif
    } u;
} plot_value;

#endif

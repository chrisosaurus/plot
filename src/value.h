#ifndef PLOT_VALUE_H
#define PLOT_VALUE_H

#include <stdbool.h>

typedef enum plot_value_type{
    plot_type_number,
    plot_type_symbol,
    plot_type_function,
    plot_type_error
#if 0
    plot_type_boolean
    plot_type_character
    plot_type_string
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
    plot_error_unbound_symbol
} plot_error_type;

typedef struct plot_error {
    plot_error_type type;
    char *msg;
} plot_error;

typedef struct plot_number {
    int val;
} plot_number;

typedef struct plot_symbol {
    char *val;
    int len;
    int size;
} plot_symbol;

#if 0
typedef struct plot_boolean {
    bool val;
} plot_boolean;

typedef struct plot_character {
    char val;
} plot_character;

typedef struct plot_string {
    char *val;
    int len;
    int size;
} plot_string;

typedef struct plot_pair {
    struct plot_value *left;
    struct plot_value *right;
} plot_pair;

#endif

struct plot_value;
struct plot_expr;

typedef struct plot_function {
    struct plot_env *env;
    /* TODO FIXME
     * need to support arbitrary number of arguments
     * need to also support plot-defined functions (via lambda)
     */
    const struct plot_value * (*func)(struct plot_env *env, const struct plot_expr *args, int argc);
} plot_function;

typedef struct plot_value {
    plot_value_type type;
    union {
        plot_number    number;
        plot_symbol    symbol;
        plot_function  function;
        plot_error     error;
#if 0
        plot_boolean   boolean;
        plot_character character;
        plot_string    string;
        plot_pair      pair;
#endif
    } u;
} plot_value;

#endif

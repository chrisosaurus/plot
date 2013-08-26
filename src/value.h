#ifndef PLOT_VALUE_H
#define PLOT_VALUE_H

#include <stdbool.h>

typedef enum plot_value_type{
    plot_type_boolean,
    plot_type_number,
    plot_type_character,
    plot_type_string,
    plot_type_symbol,
    plot_type_pair,
    plot_type_function
} plot_value_type;

typedef struct plot_boolean {
    bool val;
} plot_boolean;

typedef struct plot_number {
    int val;
} plot_number;

typedef struct plot_character {
    char val;
} plot_character;

typedef struct plot_string {
    char *val;
    int len;
    int size;
} plot_string;

typedef struct plot_symbol {
    char *val;
    int len;
    int size;
} plot_symbol;

typedef struct plot_pair {
    struct plot_value *left;
    struct plot_value *right;
} plot_pair;

typedef struct plot_function {
} plot_function;

typedef struct plot_value {
    plot_value_type type;
    union {
        plot_boolean   boolean;
        plot_number    number;
        plot_character character;
        plot_string    string;
        plot_symbol    symbol;
        plot_pair      pair;
        plot_function  function;
    } u;
} plot_value;

#endif

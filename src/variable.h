#ifndef PLOT_VARIABLE_H
#define PLOT_VARIABLE_H

#include "value.h"

typedef struct plot_variable {
    char *name;
    plot_value *val;
} plot_variable;

#endif

#ifndef PLOT_VARIABLE_H
#define PLOT_VARIABLE_H

#include "value.h"

/* FIXME should plot_value be a pointer or a value?
 * do we want to share and have proper ref counting?
 * is it needed in such a language?
 * it is possible for 2 names to refer to the same value
 * e.g. when passing a variable as an argument we are really
 * giving the same value a new name (as the old name can be
 * assigned to a new value)
 */
typedef struct plot_variable {
    char *name;
    plot_value *val;
} plot_variable;

#endif

#ifndef PLOT_ENVIRONMENT_H
#define PLOT_ENVIRONMENT_H

#include "variable.h"

/* an environment is a tree of variables */
typedef struct plot_environment {
    struct plot_environment *parent;
    plot_variable v;
} plot_environment;

#endif

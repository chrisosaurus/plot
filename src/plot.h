#ifndef PLOT_H
#define PLOT_H

struct plot_env;

typedef struct plot {
    struct plot_env *env;
} plot;

plot * plot_init(void);

void plot_cleanup(plot * p);

#endif

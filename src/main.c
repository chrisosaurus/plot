#include <stdio.h> /* puts */
#include <stdlib.h> /* exit */
#include <string.h> /* strcmp */

#include "read.h"
#include "parse.h"
#include "eval.h"

#include "value.h"
#include "funcs.h"
#include "value.h"
#include "plot.h"

void usage(void);

int main(int argc, char **argv){
    char *source, *filename;
    plot_value *prog;

    if( argc < 2 ){
        puts("No scheme source file specified");
        usage();
        exit(1);
    } else if( argc > 2 ){
        puts("Too many arguments specified");
        usage();
        exit(1);
    } else if( ! strcmp(argv[1], "-h") || ! strcmp(argv[1], "--help") ){
        usage();
        exit(0);
    }

    if( ! plot_init() ){
        /* FIXME error */
        puts("error in plot_init");
        exit(1);
    }

    filename = argv[1];
    source = plot_read(filename);
    if( ! source ){
        /* FIXME error */
        puts("error occurred in plot_read");
        exit(1);
    }

    prog = plot_parse(source);
    if( ! prog ){
        /* FIXME error */
        puts("error occured in plot_parse");
        exit(1);
    }

    if( ! plot_eval(plot_get_global_env(), prog) ){
        /* FIXME error */
        puts("error occurred in plot_eval");
        exit(1);
    }

    free(source);
    free(prog);
    plot_cleanup();

    return 0;
}

void usage(void){
    puts("plot only takes 1 argument specifying the source file to interpret");
}

#include <stdio.h> /* puts */
#include <stdlib.h> /* exit */
#include <string.h> /* strcmp */

#include "read.h"
#include "parse.h"
#include "execute.h"

void usage(void);

int main(int argc, char **argv){
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

    return 0;
}

void usage(void){
    puts("plot only takes 1 argument specifying the source file to interpret");
}

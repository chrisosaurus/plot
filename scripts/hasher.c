#include <stdio.h>
#include <string.h> /* strlen */

#include "../src/value.h" /* plot_hash_symbol */

/*
 * small helper script which takes exactly 1 argument and prints (to stdout)
 * the plot hash value that corresponds with that string
 */

int main(int argc, char **argv){
    plot_symbol sym;

    if( argc != 2 ){
        puts("Invalid usage, first and only arg should be string to hash");
        return 1;
    }

    printf("hashing: '%s'\n", argv[1]);

    /* 1 + strlen as sym.len includes \0 */
    sym.size = sym.len = 1 + strlen(argv[1]);
    sym.val = argv[1];

    sym.hash = 0;
    plot_hash_symbol(&sym);

    printf("%llu\n", sym.hash);

    return 0;
}

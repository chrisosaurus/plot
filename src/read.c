#include <stdlib.h> /* malloc */
#include <stdio.h> /* fopen, fclose, fread */

#include "read.h"

/* given a filename to read returns an allocated string
 * containing content
 * or 0 on error
 *
 * return string is owned by caller
 */
char * plot_read(const char * const filename){
    /* FIXME
     * for now this will read entire file into single buffer
     * if file is larger than memory we may be in trouble
     * fix later by chunking and having parse request more
     */
    FILE *f;
    long fsize;
    unsigned long ufsize;
    size_t read;
    char * buffer;

    /* open file */
    f = fopen(filename, "r");
    if( f == NULL ){
        puts("ERROR: fopen");
        perror("fopen error:");
        return 0;
    }

    /* find file size */
    if( fseek(f, 0, SEEK_END) ){
        fputs("ERROR: fseek SEEK_END", stderr);
        fclose(f);
        return 0;
    }
    fsize = ftell(f);
    if( fsize < 0 ){
        perror("ftell error:");
        fclose(f);
        return 0;
    }
    /* guranteed to be >= 0 */
    ufsize = fsize;

    if( fseek(f, 0, SEEK_SET) ){
        fputs("ERROR: fseek SEEK_SET", stderr);
        fclose(f);
        return 0;
    }

    /* allocate buffer */
    buffer = malloc(ufsize + 1);
    if( ! buffer ){
        perror("ERROR: malloc failed:");
        fclose(f);
        return 0;
    }

    /* make sure it looks like a string */
    buffer[ufsize] = '\0';

    /* read file */
    read = fread(buffer, 1, ufsize, f);
    if( read != ufsize ){
        printf("ERROR: fread read '%zd' when we expected '%lu'\n", read, ufsize);
        free(buffer);
        fclose(f);
        return 0;
    }

    if( fclose(f) ){
        fputs("ERROR: fclose", stderr);
        free(buffer);
        return 0;
    }

    return buffer;
}

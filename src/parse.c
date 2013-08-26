#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* we need a way for _sexpr and _expr to express 'here is the result, and her is where I got up to'
 * so we also need for them to take an int* which is one past the place they finished reading
 */

/* return an AST* or 0 for error */
plot_program * plot_parse(char *source){
    int i=0;
    void *r;
    while( source[i] != '\0' ){
        switch( source[i] ){
            case '(':
                r = plot_parse_sexpr(source, &i);
                if( ! r ){
                    puts("\t\t error in parse_sexpr, breaking");
                    break;
                }
                break;
            case ' ':
            case '\n':
            case '\t':
                /* skip whitespace */
                ++i;
                break;
            default:
                printf("UNKNOWN CASE char '%c' at '%d'\n", source[i], i);
                return 0;
                break;
        }
    }
    return 0;
}

plot_sexpr * plot_parse_sexpr(char *source, int *upto){
    plot_sexpr *sexpr = calloc(1, sizeof(*sexpr));
    if( ! sexpr )
        return 0;

    int start = *upto;
    int matching = 1; /* brackets are matches when 0 */
    if( source[start] != '(' ){
        printf("\t\tERROR: asked to consume token which is not an sexpr, char is '%c' and start is '%d'\n", source[start], start);
        return 0;
    }

    ++ *upto;
    while( matching ){
        if( source[*upto] == '(' )
            ++ matching;
        else if( source[*upto] == ')' )
            -- matching;
        ++ *upto;
    }
    char *ch = calloc( (*upto) - start + 1, 1);
    strncpy(ch, &source[start], (*upto) - start);
    printf("\t\tConsumed sexpr '%s'\n", ch);

    return sexpr;
}

plot_expr * plot_parse_expr(char *source, int *upto){
    return 0;
}


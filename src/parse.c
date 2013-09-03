#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* we need a way for _sexpr and _expr to express 'here is the result, and here is where I got up to'
 * so we also need for them to take an int* which is one past the place they finished reading
 */

/* return an plot_program* or 0 for error */
plot_program * plot_parse(char *source){
    int i=0;
    plot_sexpr *r;

    /* a plot_program is a colletion of s-expressions */
    while( source[i] != '\0' ){
        switch( source[i] ){
            case '(':
                r = plot_parse_sexpr(source, &i);
                if( ! r ){
                    puts("\t\t error in plot_parse calling plot_parse_sexpr, breaking");
                    break;
                }
                break;
            case ' ':
            case '\r':
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
    plot_expr *expr;
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
        switch( source[ *upto ] ){
            case '\0':
                /* end of the line */
                printf("found end of line, matching '%d'\n", matching);
                matching = 0;
                break;
                return 0;
            case '(':
                ++ matching;
                ++ *upto;
                break;
            case ')':
                -- matching;
                ++ *upto;
                break;
            case ' ':
            case '\r':
            case '\t':
            case '\n':
                /* skip whitespace */
                ++ *upto;
                break;
            default:
                expr = plot_parse_expr(source, upto);
                if( ! expr ){
                    puts("\t\t error in plot_parse_sexpr calling plot_parse_expr, breaking");
                    break;
                }
                break;
        }
    }
    char *ch = calloc( (*upto) - start + 1, 1);
    strncpy(ch, &source[start], (*upto) - start);
    printf("\t\tConsumed sexpr '%s'\n", ch);
    free(ch);

    return 0;
    // return sexpr; // FIXME should only return non-0 when properly allocating children
}

plot_expr * plot_parse_expr(char *source, int *upto){
    int start = *upto;

    int cont = 1;
    while( cont ){
        switch( source[ *upto ] ){
            case '\0':
                /* end of the line */
                cont = 0;
                break;
            case ' ':
            case '\r':
            case '\t':
            case '\n':
                /* whitespace is the end of a token, leave it for parent and return*/
                cont = 0;
                break;
            case ')':
                /* ) is the end of a token, leave it for parent to consume */
                cont = 0;
                break;
            default:
                /* otherwise keep going */
                ++ *upto;
                break;
        }
    }

    char *ch = calloc( (*upto) - start + 1, 1);
    strncpy(ch, &source[start], (*upto) - start);
    printf("\t\tConsumed expr '%s'\n", ch);
    free(ch);

    return 0;
}


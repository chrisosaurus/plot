#include <stdio.h>
#include <stdlib.h> /* strtol */
#include <string.h>
#include <ctype.h> /* isdigit */

#include "value.h"
#include "types.h"
#include "parse.h"
#include "plot.h"

#define DEBUG 0

void plot_parse_comment(const char *source, size_t *upto);

/* we need a way for _sexpr and _expr to express 'here is the result, and here is where I got up to'
 * so we also need for them to take an int* which is one past the place they finished reading
 */

/* provides an interface to convert a string to a plot_program (AST)
 * return a plot_program* or 0 for error
 */
plot_program * plot_parse(const char *source){
    size_t i=0;
    plot_program *prog = calloc(1, sizeof(*prog));
    prog->nchildren = 0;
    prog->max_children = 100;
    prog->exprs = calloc(prog->max_children, sizeof(*(prog->exprs))); /* FIXME fixed size */

    /* a plot_program is a colletion of s-expressions */
    while( source[i] != '\0' ){
        switch( source[i] ){
            case ';':
                plot_parse_comment(source, &i);
                break;
            case '[':
            case '(':
                if( prog->nchildren >= prog->max_children ){
                    puts("\t\t error in plot_parse, max_children exceeded, returning");
                    return 0;
                }
                if( ! plot_parse_expr(&(prog->exprs[prog->nchildren++]),source, &i) ){
                    puts("\t\t error in plot_parse calling plot_parse_expr, returning");
                    return 0;
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
                printf("UNKNOWN CASE char '%c' at '%zu'\n", source[i], i);
                return 0;
                break;
        }
    }
    return prog;
}

/* plot_parse_expr will consume a token upto a separator (but will not consume the separator)
 * *upto is an offset into the source
 * *upto represents where plot_parse_sexpr starts and it will update it to match where it got up to
 * return a plot_expr* or 0 for errors
 * */
plot_expr * plot_parse_expr(plot_expr *expr, const char *source, size_t *upto){
    int start;
    int cont = 1;
    /* set to true if we are within a value
     * if we are inside a value then only numbers or characters are allowed
     */
    int inside_value = 0;
    int inside_string = 0;
    char *tmp;

    if( source[ *upto ] == '"' ){
        ++ *upto;
        inside_string = 1;
    }

    /* we don't want to include the " or ' inside our copied string */
    start = *upto;

    while( cont ){
        switch( source[ *upto ] ){
            case ';':
                /* valid character within string */
                if( inside_string ){
                    break;
                }
                /* if inside value then end of current value */
                if( inside_value ){
                    cont =0;
                    break;
                }
                plot_parse_comment(source, upto);
                break;
            case '#':
                /* valid character within string */
                if( inside_string ){
                    break;
                }
                /* valid character inside symbol */
                if( inside_value ){
                    break;
                }

                inside_value = 1;
                /* otherwise this is the start of a boolean */
                /* parsing is handled below */
                ++ *upto;
                cont = 0;
                break;
            case '\'':
                /* if we are inside a string then this is just part of the string */
                if( inside_string){
                    ++ *upto;
                } else {
                    /* FIXME error! */
                    /* start of next quoted token */
                    puts("quoting unimplemented.. sorry");
                    exit(1);
                }
                break;
            case '"':
                /* if we are inside a string then this may be the end */
                if( inside_string ){
                    cont = 0;
                    break;
                }
                /* if we are inside a value then ' and " are not valid chars to consume
                 * and must therefore be start of the nexttoken
                 */
                if( inside_value ){
                    cont = 0;
                    break;
                }
                /* otherwise this is an error */
                puts("\t\tError in plot_parse_expr; unexpected quote token\n");
                return 0;
                break;
            case '\0':
                /* end of the line */
                cont = 0;
                break;
            case ' ':
            case '\r':
            case '\t':
            case '\n':
                /* if we are inside a string then consume whitespace as part of string */
                if( inside_string ){
                    ++ *upto;
                    break;
                }
                /* otherwise whitespace is the end of a token, leave it for parent and return */
                cont = 0;
                break;
            case '(':
            case '[':
                /* if we are inside a string then this is just part of the string */
                if( inside_string ){
                    break;
                }
                /* if we are inside a value then ( is not a valid char to consume
                 * it must be the start of the next token
                 */
                if( inside_value ){
                    cont = 0;
                    break;
                }
                /* ( start of s_expr */
                expr->type = plot_expr_sexpr;
                if( ! plot_parse_sexpr(&(expr->u.sexpr), source, upto) ){
                    puts("\t\t Error in plot_parse_expr when calling plot_parse_sexpr, returning\n");
                    return 0;
                }
                break;
            case ']':
            case ')':
                /* if we are inside a string then this is just part of string */
                if( inside_string ){
                    break;
                }
                /* ) is the end of a token, leave it for parent to consume */
                cont = 0;
                break;
            default:
                /* either symbol, number or string
                 * keep going *
                 */
                if( ! inside_string ){
                    inside_value = 1;
                }
                ++ *upto;
                break;
        }
    }

    /* if inside_string then string was consumed */
    if( inside_string ){
        /* our quote symbol has not yet been stepped over
         * so this logic is the same as that for symbols
         * + an additional step (++ *upto) to step over quote
         */
        int len = (*upto) - start + 1;
        expr->type = plot_expr_value;
        expr->u.value = plot_new_constant();
        expr->u.value->u.string.val = tmp = calloc(len, sizeof(char));
        expr->u.value->u.string.len = len;
        expr->u.value->u.string.size = len;
        strncpy(tmp, &source[start], (*upto) - start);
        expr->u.value->type = plot_type_string;
        ++ *upto;
    }
    /* if inside_value then a value was consumed */
    else if( inside_value ){
        char *invalid;
        expr->type = plot_expr_value;
        expr->u.value = plot_new_constant();
        if( isdigit(source[start]) ){ /* FIXME source[start] may not be correct, want to really look at first non-whitespace */
            /* if digit then number */
            expr->u.value->u.number.val = strtol( &source[start], &invalid, 10 );
            expr->u.value->type = plot_type_number;
            if( invalid != &source[*upto] ){
                puts("ERROR: conversion of token to t via strtol encountered an error\n");
                return 0;
            }
        } else if( source[start] == '#' ){ /* FIXME source[start] may not be correct, want to really look at first non-whitespace */
            /* boolean */
            if( source[*upto] == 't' ){
                expr->u.value->u.boolean.val = true;
            } else if( source[*upto] == 'f' ){
                expr->u.value->u.boolean.val = false;
            } else {
                puts("ERROR: invalid boolean in plot_parse_expr");
                return 0;
            }
            expr->u.value->type = plot_type_boolean;
            ++ *upto;
        } else {
            /* otherwise it is a symbol */
            int len = (*upto) - start + 1;
            expr->u.value->u.symbol.val = tmp = calloc(len, sizeof(char));
            expr->u.value->u.symbol.len = len;
            expr->u.value->u.symbol.size = len;
            strncpy(tmp, &source[start], (*upto) - start);
            expr->u.value->type = plot_type_symbol;
        }
    }

#if debug
    /* debugging only */
    char *ch = calloc( (*upto) - start + 1, 1);
    strncpy(ch, &source[start], (*upto) - start);
    printf("\t\tConsumed expr '%s'\n", ch);
    free(ch);
#endif

    return expr;
}

/* plot_parse_sexpr will consume a token upto and including the matching close paren
 * *upto is an offset into the source
 * *upto represents where plot_parse_sexpr starts and it will update it to match where it got up to
 * *sexpr is the allocated location to save parsed s_expr
 * return the plot_sexpr* (same as *sexpr) or 0 for errors
 */
plot_sexpr * plot_parse_sexpr(plot_sexpr *sexpr, const char *source, size_t *upto){
    int start = *upto;
    int cont = 1; /* continue */

    int nchildren = 0;
    int max_children = 10;
    plot_expr *children = calloc(max_children, sizeof(*children)); /* FIXME fixed size */

    /* opening bracket we encountered, if ( then closing bracket must be ), if [ the closing must be ] */
    char bracket;

    bracket = source[start];

    if( bracket != '(' && bracket != '[' ){
        printf("\t\tERROR: asked to consume token which is not an sexpr, char is '%c' and start is '%d'\n", source[start], start);
        return 0;
    }

    ++ *upto;
    while( cont ){
        switch( source[ *upto ] ){
            case ';':
                plot_parse_comment(source, upto);
                break;
            case '\0':
                /* FIXME error, can not have \0 when looking for closing bracket */
                /* end of the line */
                cont = 0;
                break;
                return 0;
            case ']':
            case ')':
                cont = 0;
                break;
            case ' ':
            case '\r':
            case '\t':
            case '\n':
                /* skip whitespace */
                ++ *upto;
                break;
            default:
                /* ( and [ also use this path (as they are compound sexpressions) */
                if( ! plot_parse_expr(&(children[nchildren++]), source, upto) ){
                    puts("\t\tError in plot_parse_sexpr, called to plot_parse_expr failed\n");
                    return 0;
                }
                break;
        }
    }

    /* check last char to see if bracket matches */
    if( bracket == '(' ){
        if( source[ *upto ] != ')' ){
            /* FIXME bracket miss match */
        }
    } else if( bracket == '[' ){
        if( source[ *upto ] != ']' ){
            /* FIXME bracket miss match */
        }
    } else {
        /* FIXME impossibru */
    }
    ++ *upto;

    /* only modufy *sexpr if there were no errors */
    sexpr->nchildren = nchildren;
    sexpr->subforms = children;

#if DEBUG
    /* debugging only */
    char *ch = calloc( (*upto) - start + 1, 1);
    strncpy(ch, &source[start], (*upto) - start);
    printf("\t\tConsumed sexpr '%s'\n", ch);
    free(ch);
#endif

    return sexpr;
}

void plot_parse_comment(const char *source, size_t *upto){
    while( 1 ){
        switch( source[ *upto ] ){
            case '\0':
            case '\r':
            case '\n':
                /* end of comment */
                return;
            default:
                break;
        }
        ++ *upto;
    }
}


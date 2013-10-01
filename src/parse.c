#include <stdio.h>
#include <stdlib.h> /* strtol */
#include <string.h>
#include <ctype.h> /* isdigit */

#include "value.h"
#include "types.h"
#include "parse.h"

#define DEBUG 1

/* we need a way for _sexpr and _expr to express 'here is the result, and here is where I got up to'
 * so we also need for them to take an int* which is one past the place they finished reading
 */

/* provides an interface to convert a string to a plot_program (AST)
 * return a plot_program* or 0 for error
 */
plot_program * plot_parse(char *source){
    int i=0;
    plot_program *prog = calloc(1, sizeof(*prog));
    prog->nchildren = 0;
    prog->max_children = 10;
    prog->exprs = calloc(prog->max_children, sizeof(*(prog->exprs))); /* FIXME fixed size */

    /* a plot_program is a colletion of s-expressions */
    while( source[i] != '\0' ){
        switch( source[i] ){
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
                printf("UNKNOWN CASE char '%c' at '%d'\n", source[i], i);
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
plot_expr * plot_parse_expr(plot_expr *expr, char *source, int *upto){
    int start = *upto;
    int cont = 1;
    /* set to true if we are within a value
     * if we are inside a value then only numbers or characters are allowed
     */
    int inside_value = 0;

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
                /* whitespace is the end of a token, leave it for parent and return */
                cont = 0;
                break;
            case '(':
            case '[':
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
                /* ) is the end of a token, leave it for parent to consume */
                cont = 0;
                break;
            default:
                /* either symbol or number
                 * keep going *
                 */
                inside_value = 1;
                ++ *upto;
                break;
        }
    }

    /* if inside_value then a value was consumed */
    if( inside_value ){
        char *invalid;
        expr->type = plot_expr_value;
        if( isdigit(source[start]) ){
            /* if digit then number */
            expr->u.value.u.number.val = strtol( &source[start], &invalid, 10 );
            expr->u.value.type = plot_type_number;
            if( invalid != &source[*upto] ){
                puts("ERROR: conversion of token to t via strtol encountered an error\n");
                return 0;
            }
        } else {
            /* otherwise it is a symbol */
            int len = (*upto) - start + 1;
            expr->u.value.u.symbol.val = calloc(len, sizeof(char));
            expr->u.value.u.symbol.len = len;
            expr->u.value.u.symbol.size = len;
            strncpy(expr->u.value.u.symbol.val, &source[start], (*upto) - start);
            expr->u.value.type = plot_type_symbol;
        }
    }

#if DEBUG
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
plot_sexpr * plot_parse_sexpr(plot_sexpr *sexpr, char *source, int *upto){
    int start = *upto;
    int matching = 1; /* brackets are matches when 0 */

    int nchildren = 0;
    int max_children = 10;
    plot_expr *children = calloc(max_children, sizeof(*children)); /* FIXME fixed size */

    /* opening bracket we encountered, if ( then closing bracket must be ), if [ the closing must be ] */
    char bracket;

    if( source[start] != '(' && source[start] != '[' ){
        printf("\t\tERROR: asked to consume token which is not an sexpr, char is '%c' and start is '%d'\n", source[start], start);
        return 0;
    }

    ++ *upto;
    while( matching ){
        switch( source[ *upto ] ){
            case '\0':
                /* end of the line */
                matching = 0;
                break;
                return 0;
            case '[':
            case '(':
                bracket = source[*upto];
                ++ matching;
                ++ *upto;
                break;
            case ']':
            case ')':
                /* TODO FIXME check this bracket matches bracket */
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
                if( ! plot_parse_expr(&(children[nchildren++]), source, upto) ){
                    puts("\t\tError in plot_parse_sexpr, called to plot_parse_expr failed\n");
                    return 0;
                }
                break;
        }
    }

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



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
                #if DEBUG
                    puts("plot_parse: calling plot_parse_expr");
                #endif
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
                #if DEBUG
                    puts("plot_parse: skipping whitespace");
                #endif
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

/* internal method used by plot_parse_expr
 * deals with literals beginning with #
 *
 * this includes:
 *      booleans: #t #f
 *      characters: #\a #\space #\newline (etc.) FIXME unimplemented
 *      vectors: FIXME unimplemented
 *
 * expects first character of value to be at source[*upto]
 */
static plot_expr * plot_parse_expr_hash(plot_expr *expr, const char *source, size_t *upto){
    /* char_start is where the character constant started */
    size_t char_start = 0; /* only used for character parsing */
    /* char_cont signified we are still parsing a character constant */
    int char_cont = 1;


    if( ! source[*upto] == '#' ){
        plot_fatal_error("plot_parse_expr_hash: first character was not '#'");
    }

#if DEBUG
    puts("plot_parse_expr_hash: start");
#endif

    ++ *upto;

    expr->type = plot_expr_value;
    expr->u.value = plot_new_constant();

    switch (source[*upto]){
        case 't':
            expr->u.value->type = plot_type_boolean;
            expr->u.value->u.boolean.val = true;
            ++ *upto;
            return expr;
            break;
        case 'f':
            expr->u.value->type = plot_type_boolean;
            expr->u.value->u.boolean.val = false;
            ++ *upto;
            return expr;
            break;
        case '\\':
            /* skip past the \ */
            ++*upto;
            char_start = *upto;
            while (char_cont){
                switch (source[*upto]){
                    case '\r':
                    case '\n':
                    case ' ':
                        /* r5rs allows for the following space literals:
                         *      `#\ ` to signify a literal space
                         *      `#\\r` or `#\\n` to signify a newline (where \n and \r are the literal char values)
                         *
                         * if this is the first character within the literal:
                         *      this is a space literal, consume and end
                         *      else end
                         */
                        if( char_start == *upto ){
                            ++*upto;
                        }
                        char_cont = 0;
                        break;
                    case '\0':
                        char_cont = 0;
                        break;
                    default:
                        /* keep on going */
                        ++ *upto;
                        /* FIXME this disallowed complex char literals
                         * HOWEVER without this we will try to consume
                         * `#\a)` as a single token, which is not right
                         * however `#\)` is a valid character literal
                         */
                        char_cont = 0;
                        break;
                }
            }
            break;
        default:
            plot_fatal_error("plot_parse_expr_hash: unexpected character (vectors not implemented yet");
            break;
    }

    /* here means we are dealing with a character literal */
    if( char_start ){
        expr->u.value->type = plot_type_character;
        if( (*upto - char_start) == 1 ){
            /* simple */
            expr->u.value->u.character.val = source[char_start];
        } else {
            printf("distance '%d'\n", (int)(*upto - char_start));
            /* complex char constant
             * supported:
             *      `newline`
             *      `space`
             */
            /* FIXME difficult */
            plot_fatal_error("plot_parse_expr_hash: complex character literals are not currently implemented");
        }
    }

#if DEBUG
    puts("plot_parse_expr_hash: end");
#endif

    return expr;
}

/* internal method used by plot_parse_expr
 * deals with parsing of strings
 *
 * expects first character of string to be at source[*upto]
 */
static plot_expr * plot_parse_expr_string(plot_expr *expr, const char *source, size_t *upto){
    size_t start;
    int cont = 1;
    int escaped = 0; /* is the next character escaped? set by / */

    int len;

#if DEBUG
    puts("plot_parse_expr_string: start");
#endif

    if( source[*upto] != '"' ){
        plot_fatal_error("plot_parse_expr_string: did not find starting quote");
    }

    /* do not include the leading " in string */
    ++*upto;
    start = *upto;
    while( cont ){
        switch (source[*upto]){
            case '"':
                if( ! escaped ){
                    /* end of string unless escaped */
                    cont = 0;
                }
                ++*upto;
                break;
            case '\\':
                escaped = 1;
                ++*upto;
                break;
            default:
                /* part of string */
                ++*upto;
                break;
        }
    }

    /* NB: at this point *upto is PAST the terminating "
     * we do NOT want to copy this, however we do want to
     * insert a null terminator instead
     */
    len = (*upto) - start; /* includes null term */
    expr->type  = plot_expr_value;
    expr->u.value = plot_new_constant();
    expr->u.value->type = plot_type_string;
    expr->u.value->u.string.len = len;
    expr->u.value->u.string.size = len;
    expr->u.value->u.string.val = plot_new_string(len);
    strncpy(expr->u.value->u.string.val, &source[start], len - 1);

#if DEBUG
    printf("plot_parse_expr_string: end '%s'\n", expr->u.value->u.string.val);
#endif

    return expr;
}

/* internal method used by plot_parse_expr
 * deals with parsing of symbols
 *
 * expects first character of symbol to be at source[*upto]
 */
static plot_expr * plot_parse_expr_symbol(plot_expr *expr, const char *source, size_t *upto){
    size_t start = *upto;
    int len;
    int cont = 1;
    char *tmp;

#if DEBUG
    puts("plot_parse_expr_symbol: start");
    printf("started looking at '%c'\n", source[start]);
#endif

    while( cont ){
        switch (source[*upto]){
            case '\0':
            case ' ':
            case '\r':
            case '\n':
            case '\t':
                /* end, leave for parent to consume*/
                cont = 0;
                break;
            case ')':
            case ']':
                /* end, leave for parent to consume*/
                cont = 0;
                break;
            default:
                /* probably part of symbol */
                ++ *upto;
                break;
        }
    }

    len = (*upto) - start + 1; /* +1 to include null term */
    expr->type = plot_expr_value;
    expr->u.value = plot_new_constant();
    expr->u.value->type = plot_type_symbol;
    expr->u.value->u.symbol.len = len;
    expr->u.value->u.symbol.size = len;
    /* NB: we also assign to tmp so that we can
     * then write to it without violating const
     * from plot symbol val
     */
    expr->u.value->u.symbol.val = tmp = plot_new_string(len);
    strncpy(tmp, &source[start], len - 1);

#if DEBUG
    printf("plot_parse_expr_symbol: end '%s'\n", expr->u.value->u.symbol.val);
#endif

    return expr;
}

/* internal method used by plot_parse_expr
 * deals with parsing of numbers
 *
 * expects first character of number to be at source[*upto]
 */
static plot_expr * plot_parse_expr_number(plot_expr *expr, const char *source, size_t *upto){
    int cont = 1;
    size_t start = *upto;
    char *end;

#if DEBUG
    puts("plot_parse_expr_number: start");
    printf("started looking at '%c'\n", source[start]);
#endif

    while (cont){
        switch (source[*upto]) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                /* part of number, keep going */
                ++ *upto;
                break;
            default:
                /*end, leave for parent to consume */
                cont = 0;
                break;
        }
    }
    if( start == *upto ){
        plot_fatal_error("plot_parse_expr_number: no digits consumed");
    }

    expr->type = plot_expr_value;
    expr->u.value = plot_new_constant();
    expr->u.value->type = plot_type_number;
    expr->u.value->u.number.val = strtol( &source[start], &end, 10);
    /* end should be first non-digit
     * it should also be where we stopped parsing
     */
    if( end != &source[*upto] ){
        plot_fatal_error("plot_parse_expr_number: did not correctly consume number");
    }

#if DEBUG
    printf("plot_parse_expr_number: end '%d'\n", expr->u.value->u.number.val);
#endif

    return expr;
}

/* FIXME ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"
/* internal method used by plot_parse_expr
 * deals with parsing of quoted values
 *
 * expects quote to be at source[*upto]
 */
static plot_expr * plot_parse_expr_quote(plot_expr *expr, const char *source, size_t *upto){
#if DEBUG
    puts("plot_parse_expr_quote: start");
#endif

    plot_fatal_error("plot_parse_expr_quote: Unimplemented");
    return 0; /* FIXME */
}

/* plot_parse_expr will consume a token upto a separator (but will not consume the separator)
 * *upto is an offset into the source
 * *upto represents where plot_parse_sexpr starts and it will update it to match where it got up to
 * return a plot_expr* or 0 for errors
 * */
plot_expr * plot_parse_expr(plot_expr *expr, const char *source, size_t *upto){
    int cont = 1;

    while( cont ){
        switch( source[ *upto ] ){
            case ';':
                plot_parse_comment(source, upto);
                break;
            case '#':
                #if DEBUG
                    puts("plot_parse_expr: calling plot_parse_expr_hash");
                #endif
                return plot_parse_expr_hash(expr, source, upto);
                break;
            case '\'':
                #if DEBUG
                    puts("plot_parse_expr: calling plot_parse_expr_quote");
                #endif
                return plot_parse_expr_quote(expr, source, upto);
                break;
            case '"':
                #if DEBUG
                    puts("plot_parse_expr: calling plot_parse_expr_string");
                #endif
                /* start of string */
                return plot_parse_expr_string(expr, source, upto);
                break;
            case '\0':
                #if DEBUG
                    puts("plot_parse_expr: null");
                #endif
                /* end of the line */
                cont = 0;
                break;
            case ' ':
            case '\r':
            case '\t':
            case '\n':
                #if DEBUG
                    puts("plot_parse_expr: consuming whitespace");
                #endif
                /* consume whitespace */
                ++ *upto;
                break;
            case '(':
            case '[':
                #if DEBUG
                    puts("plot_parse_expr: calling plot_parse_sexpr");
                #endif
                /* ( start of s_expr */
                expr->type = plot_expr_sexpr;
                if( ! plot_parse_sexpr(&(expr->u.sexpr), source, upto) ){
                    puts("\t\t Error in plot_parse_expr when calling plot_parse_sexpr, returning\n");
                    return 0;
                }
                return expr;
                break;
            case ']':
            case ')':
                #if DEBUG
                    puts("plot_parse_expr: end of token");
                #endif
                /* ) is the end of a token, leave it for parent to consume */
                cont = 0;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                #if DEBUG
                    puts("plot_parse_expr: calling plot_parse_expr_number");
                #endif
                return plot_parse_expr_number(expr, source, upto);
                break;
            default:
                #if DEBUG
                    puts("plot_parse_expr: calling plot_parse_expr_symbol");
                #endif
                /* symbol */
                return plot_parse_expr_symbol(expr, source, upto);
                break;
        }
    }

    #if DEBUG
        puts("plot_parse_expr: returning expr...");
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
                #if DEBUG
                puts("\n\nplot_parse_sexpr: calling plot_parse_expr");
                #endif
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


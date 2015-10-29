#include <stdio.h>
#include <stdlib.h> /* strtol */
#include <string.h>
#include <ctype.h> /* isdigit */

#include "value.h"
#include "parse.h"
#include "plot.h"

#define DEBUG 0

void plot_parse_comment(const char *source, size_t *upto);

/* we need a way for _sexpr and _expr to express 'here is the result, and here is where I got up to'
 * so we also need for them to take an int* which is one past the place they finished reading
 */

/* provides an interface to convert a string to a plot_program (AST)
 * return a plot_value * (list) or 0 for error
 */
plot_value * plot_parse(const char *source){
    size_t i=0;
    plot_value *head, **cur; /* list */
    head = 0;
    cur = &head;

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
                if( *cur ){
                    /* cur will be a constant null, but decr to keep consistency */
                    plot_value_decr(*cur);
                }
                *cur = plot_new_pair(0, plot_new_null());
                lcar(*cur) = plot_parse_expr(source, &i);
                if( ! car(*cur) ){
                    puts("\t\t error in plot_parse calling plot_parse_expr, returning");
                    return 0;
                    break;
                }
                cur = &lcdr(*cur);
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
    return head;
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
static plot_value * plot_parse_expr_hash(const char *source, size_t *upto){
    /* char_start is where the character constant started */
    size_t char_start = 0;
    plot_value *val;

    if( source[*upto] != '#' ){
        plot_fatal_error("plot_parse_expr_hash: first character was not '#'");
    }

#if DEBUG
    puts("plot_parse_expr_hash: start");
#endif

    ++ *upto;

    switch (source[*upto]){
        case 't':
            val = plot_new_boolean(true);
            plot_value_constantify(val);
            ++ *upto;
            return val;
            break;
        case 'f':
            val = plot_new_boolean(false);
            plot_value_constantify(val);
            ++ *upto;
            return val;
            break;
        case '\\':
            /* skip past the \ */
            ++*upto;
            char_start = *upto;
            break;
        default:
            plot_fatal_error("plot_parse_expr_hash: unexpected character (vectors not implemented yet");
            break;
    }

    /* here means we are dealing with a character literal */
    if( char_start ){
        /* try complex character literals first */
        if( ! strncmp(&source[char_start], "newline", 7) ){
            val = plot_new_character('\n');
            plot_value_constantify(val);
            *upto += 7;
            return val;
        }
        if( ! strncmp(&source[char_start], "space", 5) ){
            val = plot_new_character(' ');
            plot_value_constantify(val);
            *upto += 5;
            return val;
        }

        /* otherwise we are a simple (single letter) character literal */
        val = plot_new_character( source[char_start] );
        plot_value_constantify(val);
        ++ *upto;
        return val;
    }

#if DEBUG
    puts("plot_parse_expr_hash: end");
#endif

    plot_fatal_error("Impossible case in plot_parse_expr_hash");
    return 0;
}

/* internal method used by plot_parse_expr
 * deals with parsing of strings
 *
 * expects first character of string to be at source[*upto]
 */
static plot_value * plot_parse_expr_string(const char *source, size_t *upto){
    size_t start;
    int cont = 1;
    int escaped = 0; /* is the next character escaped? set by / */
    plot_value *val;

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
    val = plot_new_string( plot_alloc_string(len), len);
    plot_value_constantify(val);

    strncpy(val->u.string.val, &source[start], len - 1);

#if DEBUG
    printf("plot_parse_expr_string: end '%s'\n", val->u.string.val);
#endif

    return val;
}

/* internal method used by plot_parse_expr
 * deals with parsing of symbols
 *
 * expects first character of symbol to be at source[*upto]
 */
static plot_value * plot_parse_expr_symbol(const char *source, size_t *upto){
    char ch;
    size_t start = *upto;
    int len;
    int cont = 1;
    char *tmp;
    plot_value *val;

#if DEBUG
    puts("plot_parse_expr_symbol: start");
    printf("started looking at '%c'\n", source[start]);
#endif

    while( cont ){
        ch = source[*upto];
        switch (ch){
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
            case '(':
            case '[':
                /* end of symbol, leave for parent to consume */
                cont = 0;
                break;
            default:
                if( ch < 33 || ch > 126 ){
                    plot_fatal_error("symbol outside valid ascii range (33 <= symbol <= 126)");
                }
                /* probably part of symbol */
                ++ *upto;
                break;
        }
    }

    len = (*upto) - start + 1; /* +1 to include null term */
    /* NB: we also assign to tmp so that we can
     * then write to it without violating const
     * from plot symbol val
     */
    tmp = plot_alloc_string(len);
    strncpy(tmp, &source[start], len - 1);
    val = plot_new_symbol(tmp, len);
    plot_value_constantify(val);

    plot_hash_symbol(&(val->u.symbol));

#if DEBUG
    printf("plot_parse_expr_symbol: end '%s'\n", val->u.symbol.val);
#endif

    return val;
}

/* internal method used by plot_parse_expr
 * deals with parsing of numbers
 *
 * expects first character of number to be at source[*upto]
 */
static plot_value * plot_parse_expr_number(const char *source, size_t *upto){
    int cont = 1;
    size_t start = *upto;
    char *end;
    plot_value *val;

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

    val = plot_new_number(strtol( &source[start], &end, 10));
    plot_value_constantify(val);
    /* end should be first non-digit
     * it should also be where we stopped parsing
     */
    if( end != &source[*upto] ){
        plot_fatal_error("plot_parse_expr_number: did not correctly consume number");
    }

#if DEBUG
    printf("plot_parse_expr_number: end '%d'\n", val->u.number.val);
#endif

    return val;
}

/* FIXME ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"
/* internal method used by plot_parse_expr
 * deals with parsing of quoted values
 *
 * expects quote to be at source[*upto]
 */
static plot_value * plot_parse_expr_quote(const char *source, size_t *upto){
    plot_value *val;

#if DEBUG
    puts("plot_parse_expr_quote: start");
#endif

    /* FIXME check if quote
     * consume quote
     */
    ++*upto;

    /* return an s-expr of the form
     * (quote <data>)
     *
     * we represent this as
     * car = (quote <data>)
     * cdr = null
     */

    val = plot_new_pair(0, plot_new_pair(0, plot_new_null()));
    lcar(val) = plot_new_symbol("quote", 6);
    plot_value_constantify(car(val));
    lcar(cdr(val)) = plot_parse_expr(source, upto);

    #if DEBUG
    puts("parsed quote:");
    display_error_expr(val);
    #endif

    if( ! car(cdr(val)) ){
        puts("\t\t Error in plot_parse_expr_quote when calling plot_parse_expr, returning\n");
        return 0;
    }

#if DEBUG
    puts("plot_parse_expr_quote: fin");
#endif

    return val;
}

/* plot_parse_expr will consume a token upto a separator (but will not consume the separator)
 * *upto is an offset into the source
 * *upto represents where plot_parse_sexpr starts and it will update it to match where it got up to
 * return a plot_value * (list) or 0 for errors
 * */
plot_value * plot_parse_expr(const char *source, size_t *upto){
    int cont = 1;
    plot_value *val;

    while( cont ){
        switch( source[ *upto ] ){
            case ';':
                plot_parse_comment(source, upto);
                break;
            case '#':
                #if DEBUG
                    puts("plot_parse_expr: calling plot_parse_expr_hash");
                #endif
                return plot_parse_expr_hash(source, upto);
                break;
            case '\'':
                #if DEBUG
                    puts("plot_parse_expr: calling plot_parse_expr_quote");
                #endif
                return plot_parse_expr_quote(source, upto);
                break;
            case '"':
                #if DEBUG
                    puts("plot_parse_expr: calling plot_parse_expr_string");
                #endif
                /* start of string */
                return plot_parse_expr_string(source, upto);
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
                val = plot_parse_sexpr(source, upto);
                if( ! val ){
                    puts("\t\t Error in plot_parse_expr when calling plot_parse_sexpr, returning\n");
                    return 0;
                }
                return val;
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
                return plot_parse_expr_number(source, upto);
                break;
            default:
                #if DEBUG
                    puts("plot_parse_expr: calling plot_parse_expr_symbol");
                #endif
                /* symbol */
                return plot_parse_expr_symbol(source, upto);
                break;
        }
    }

    plot_fatal_error("Impossible situation in plot_parse_expr");
    return 0;
}

/* plot_parse_sexpr will consume a token upto and including the matching close paren
 * *upto is an offset into the source
 * *upto represents where plot_parse_sexpr starts and it will update it to match where it got up to
 * *sexpr is the allocated location to save parsed s_expr
 * return the plot_value* (list) or 0 for errors
 */
plot_value * plot_parse_sexpr(const char *source, size_t *upto){
    int start = *upto;
    int cont = 1; /* continue */

    /* opening bracket we encountered, if ( then closing bracket must be ), if [ the closing must be ] */
    char bracket;

    plot_value *head, **cur;
    head = 0;
    cur = &head;

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
                return 0;
                break;
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
                /* about to overwrite cur, decr, doesnt matter as null is constant, but better to be consistent */
                if( *cur ){
                    plot_value_decr(*cur);
                }
                *cur = plot_new_pair(0, plot_new_null());
                lcar(*cur) = plot_parse_expr(source, upto);
                if( ! car(*cur) ){
                    puts("\t\tError in plot_parse_sexpr, called to plot_parse_expr failed\n");
                    return 0;
                }
                cur = &lcdr(*cur);
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

    /* empty list */
    if( ! head )
        return plot_new_null();

    return head;
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


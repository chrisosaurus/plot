#include <stdio.h>
#include <string.h> /* strlen */

#include "value.h"
#include "output.h"
#include "plot.h"

/* plot_value deconstructor
 * correctly decr any values held within value
 */
void plot_value_decons(plot_value *value){
    switch( value->type ){
        case plot_type_promise:
            plot_env_decr(value->u.promise.env);
            plot_value_decr(value->u.promise.expr);
            value->u.promise.expr = 0;
            plot_value_decr(value->u.promise.value);
            value->u.promise.value = 0;
            break;
        case plot_type_lambda:
            plot_env_decr(value->u.lambda.env);
            plot_value_decr(value->u.lambda.body);
            value->u.lambda.body = 0;
            break;
        case plot_type_pair:
            plot_value_decr(value->u.pair.car);
            value->u.pair.car = 0;
            plot_value_decr(value->u.pair.cdr);
            value->u.pair.cdr = 0;
            break;
        case plot_type_binary_port:
            if( value->u.binport.status == plot_port_open ){
                value->u.binport.status = plot_port_closed;
                fclose(value->u.binport.file);
                value->u.binport.file = 0;
            }
            break;
        case plot_type_textual_port:
            if( value->u.textport.status == plot_port_open ){
                value->u.textport.status = plot_port_closed;
                fclose(value->u.textport.file);
                value->u.textport.file = 0;
            }
            break;
        default:
            break;
    }
}

/* `*file` is an already opened file */
plot_value * plot_new_textual_port(plot_port_dir direction, FILE *file){
    plot_value *res;
    res = plot_alloc_value();
    res->type = plot_type_textual_port;

    res->u.textport.file = file;
    res->u.textport.direction = direction;
    res->u.textport.status = plot_port_open;

    return res;
}

/* `*file` is an already opened file */
plot_value * plot_new_binary_port(plot_port_dir direction, FILE *file){
    plot_value *res;
    res = plot_alloc_value();
    res->type = plot_type_binary_port;

    res->u.binport.file = file;
    res->u.binport.direction = direction;
    res->u.binport.status = plot_port_open;

    return res;
}

plot_value * plot_new_eof(void){
    return plot_get_eof_constant();
}

plot_value * plot_new_unspecified(void){
    return plot_get_unspecified_constant();
}

plot_value * plot_new_number(int value){
    plot_value *res;
    res = plot_alloc_value();

    res->type = plot_type_number;
    res->u.number.val = value;

    return res;
}

plot_value * plot_new_boolean(int value){
    plot_value *res;
    res = plot_alloc_value();

    res->type = plot_type_boolean;
    res->u.boolean.val = value;

    return res;
}

plot_value * plot_new_character(char value){
    plot_value *res;
    res = plot_alloc_value();

    res->type = plot_type_character;
    res->u.character.val = value;

    return res;
}

plot_value * plot_new_symbol(char *val, int len){
    plot_value *res;
    res = plot_alloc_value();

    res->type = plot_type_symbol;
    res->u.symbol.val = val;
    res->u.symbol.hash = 0;

    if( len >= 0 ){
        res->u.symbol.len = len;
    } else {
        res->u.symbol.len = strlen(val) + 1;
    }
    res->u.symbol.size = res->u.symbol.len;

    return res;
}

plot_value * plot_new_string(char * val, int len){
    plot_value *res;
    res = plot_alloc_value();

    res->type = plot_type_string;
    res->u.string.val = val;

    if( len >= 0 ){
        res->u.string.len = len;
    } else {
        res->u.string.len = strlen(val) + 1;
    }
    res->u.string.size = res->u.string.len;

    return res;
}

/* allocate and construct a new pair with the supplied car and cdr
 * plot_new_pair will NOT incr supplied car or cdr
 */
plot_value * plot_new_pair(struct plot_value *car, struct plot_value *cdr){
    plot_value *res;
    res = plot_alloc_value();

    res->type = plot_type_pair;
    res->u.pair.car = car;
    res->u.pair.cdr = cdr;

    return res;
}

plot_value * plot_new_null(void){
    return plot_get_null_constant();
}

plot_value * plot_new_error(plot_error_type type, const char *msg, const char *location){
    plot_value *res;
    res = plot_alloc_value();

    res->type = plot_type_error;
    res->u.error.type = type;
    res->u.error.msg = msg;
    res->u.error.location = location;

    return res;
}

plot_value * plot_new_promise(struct plot_env *env, struct plot_value *expr){
    plot_value *res;
    res = plot_alloc_value();

    res->type = plot_type_promise;
    res->u.promise.env = env;

    /* expr is only evaluated once
     */
    res->u.promise.expr = expr;

    /* value is only set once forced
     */
    res->u.promise.value = 0;

    plot_env_incr(env);
    plot_value_incr(expr);

    return res;
}

plot_value * plot_new_lambda(struct plot_env *env, struct plot_value *body){
    plot_value *res;
    res = plot_alloc_value();

    res->type = plot_type_lambda;
    res->u.lambda.env = env;
    res->u.lambda.body = body;

    plot_env_incr(env);
    plot_value_incr(body);

    return res;
}

plot_value * plot_new_form( struct plot_value * (*func)(struct plot_env *env, struct plot_value *sexpr), int syntactic){
    plot_value *res;
    res = plot_alloc_value();

    res->type = plot_type_form;
    res->u.form.func = func;
    res->u.form.syntactic = syntactic;

    return res;
}

plot_value * plot_new_library(struct plot_env *internal, struct plot_env *exported){
    plot_value *res;
    res = plot_alloc_value();

    res->type = plot_type_library;
    res->u.library.internal = internal;
    res->u.library.exported = exported;

    return res;
}

/* raise l to power i
 */
static unsigned long long lpow(unsigned long long l, int i){
    unsigned long long res = 1;
    for( ; i > 0; --i ){
        res *= l;
        if( res < l ){
            plot_fatal_error("lpow: overflow detected");
        }
    }
    return res;
}

/* internal character hashing function
 * returns a hash value in the range of 0 <= hash <= 94
 */
static unsigned long long plot_hash_char_func(char ch){
    unsigned long long v;

    if( ch < 33 ){ /* lowest character is 33 '!' */
        printf("Invalid char '%c' (%d)\n", ch, ch);
        plot_fatal_error("plot_hash_char_func: character our of range (<33)");
    }

    if( ch > 126 ){ /* highest char is 126 '~', (- 126 33) => 93 */
        printf("Invalid char '%c' (%d)\n", ch, ch);
        plot_fatal_error("plot_hash_char_func: character our of range (>126)");
    }

    /* NB: 32 rather than 33 as we want to avoid '0' as valid hash
     * this also means our max hash value is actually 94
     */
    v = ch - 32;

    return v;
}

/* function to hash symbols
 *
 * a fatal error will be thrown if an invalid symbol is passed in
 *
 * function will not modify existing hash
 */
void plot_hash_symbol(plot_symbol *s){
    int i;
    unsigned long long hash = 0;

    if( !s ){
        /* will not return */
        plot_fatal_error("plot_hash_symbol: Invalid plot_symbol given as argument");
    }

    if( s->hash ){
        /* do not re hash symbol */
        return;
    }

    /* len - 1 as len includes null terminator */
    for( i=0; i< (s->len - 1); ++i ){
        /* 94 is highest possible hash value for a char
         * so we use 95 to prevent (or reduce) collision
         */
        hash += lpow(95, i) * plot_hash_char_func( s->val[i] );
    }

    //printf("hash for '%s' is '%llu'\n", s->val, hash);
    s->hash = hash;
}

/* turn an existing plot_value into a constant
 */
void plot_value_constantify(plot_value *val){
    val->gc.refcount = -1;
}

void display_type(plot_value *val){
    switch(val->type){
        case plot_type_number:
            puts("type is number");
            break;
        case plot_type_symbol:
            puts("type is symbol");
            break;
        case plot_type_lambda:
            puts("type is lambda");
            break;
        case plot_type_form:
            puts("type is form");
            break;
        case plot_type_error:
            puts("type is error");
            break;
        case plot_type_string:
            puts("type is string");
            break;
        case plot_type_boolean:
            puts("type is boolean");
            break;
        case plot_type_character:
            puts("type is character");
            break;
        case plot_type_pair:
            puts("type is pair");
            break;
        case plot_type_promise:
            puts("type is promise");
            break;
        case plot_type_null:
            puts("type is null");
            break;
        case plot_type_unspecified:
            puts("type is unspecified");
            break;
        case plot_type_reclaimed:
            puts("type is reclaimed");
            break;
        default:
            puts("UNKNOWN TYPE");
            break;
    }
}

void display_expr(plot_value * sexpr){
    plot_value *val;

    if( sexpr->type == plot_type_pair ){
        fputs("(", stdout);
        for( val = sexpr; val->type != plot_type_null; val = cdr(val) ){
            display_expr(car(val));
            if( cdr(val)->type != plot_type_null ){
                /* if there is more to come */
                fputs(" ", stdout);
            }
        }
        fputs(")", stdout);
    } else {
        val = cons(sexpr, null);
        plot_func_display(0, val);

        /* set car of val to 0 before gc
         * as cons does not incr and we don't want
         * to accidentally decr sexpr
         */
        car(val) = 0;
        plot_value_decr(val);
    }
}

void display_error_expr(plot_value *expr){
    fputs(">\t", stdout);
    display_expr(expr);
    fputs("\n", stdout);
}



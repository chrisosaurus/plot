#include <stdio.h>
#include <string.h> /* strlen */

#include "value.h"
#include "plot.h"

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

plot_value * plot_new_pair(struct plot_value *car, struct plot_value *cdr){
    plot_value *res;
    res = plot_alloc_value();

    res->type = plot_type_pair;
    res->u.pair.car = car;
    res->u.pair.cdr = cdr;

    return res;
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

plot_value * plot_new_lambda(struct plot_env *env, struct plot_sexpr *body){
    plot_value *res;
    res = plot_alloc_value();

    res->type = plot_type_lambda;
    res->u.lambda.env = env;
    res->u.lambda.body = body;

    plot_env_incr(env);

    return res;
}

plot_value * plot_new_builtin( struct plot_value * (*func)(struct plot_env *env, struct plot_value **args, int argc) ){
    plot_value *res;
    res = plot_alloc_value();

    res->type = plot_type_builtin;
    res->u.builtin.func = func;

    return res;
}

plot_value * plot_new_syntactic( struct plot_value * (*func)(struct plot_env *env, struct plot_sexpr *sexpr) ){
    plot_value *res;
    res = plot_alloc_value();

    res->type = plot_type_syntactic;
    res->u.syntactic.func = func;

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


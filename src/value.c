#include <string.h> /* strlen */

#include "value.h"
#include "plot.h"

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

    return res;
}

plot_value * plot_new_builtin( struct plot_value * (*func)(struct plot_env *env, struct plot_value **args, int argc) ){
    plot_value *res;
    res = plot_alloc_value();

    res->type = plot_type_builtin;
    res->u.builtin.func = func;

    return res;
}


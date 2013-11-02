#include <ctype.h> /* tolower */

#include "value.h"
#include "funcs.h"

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

#define DEBUG 0

/****** character procedures ******/

/* (char=? char1 char2)
 * character equality test
 */
struct plot_value * plot_func_char_equal(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 arguments", "plot_func_char_equal");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_equal");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_equal");
    }

    res = plot_new_value();
    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    if( args[0]->u.character.val == args[1]->u.character.val ){
        res->u.boolean.val = true;
    }

    return res;
}

/* (char-ci=? char1 char2)
 * character case-insensitive equality test
 */
struct plot_value * plot_func_char_ci_equal(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 arguments", "plot_func_char_equal");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_equal");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_equal");
    }

    res = plot_new_value();
    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    if( tolower(args[0]->u.character.val) == tolower(args[1]->u.character.val) ){
        res->u.boolean.val = true;
    }

    return res;
}

/* (char<? char1 char2)
 */
struct plot_value * plot_func_char_less(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_less");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_less");
    }

    if( args[1]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_less");
    }

    res = plot_new_value();
    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    if( args[0]->u.boolean.val < args[1]->u.boolean.val ){
        res->u.boolean.val = true;
    }

    return res;
}

/* (char>? char1 char2)
 */
struct plot_value * plot_func_char_greater(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_greater");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_greater");
    }

    if( args[1]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_greater");
    }

    res = plot_new_value();
    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    if( args[0]->u.boolean.val > args[1]->u.boolean.val ){
        res->u.boolean.val = true;
    }

    return res;
}

/* (char<=? char1 char2)
 */
struct plot_value * plot_func_char_less_equal(struct plot_env *env, struct plot_value **args, int argc){
        plot_value *res;

    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_less_equal");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_less_equal");
    }

    if( args[1]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_less_equal");
    }

    res = plot_new_value();
    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    if( args[0]->u.boolean.val <= args[1]->u.boolean.val ){
        res->u.boolean.val = true;
    }

    return res;
}

/* (char>=? char1 char2)
 */
struct plot_value * plot_func_char_greater_equal(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_greater_equal");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_greater_equal");
    }

    if( args[1]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_greater_equal");
    }

    res = plot_new_value();
    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    if( args[0]->u.boolean.val >= args[1]->u.boolean.val ){
        res->u.boolean.val = true;
    }

    return res;
}

/* (char-ci<? char1 char2)
 */
struct plot_value * plot_func_char_ci_less(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_ci_less");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_ci_ess");
    }

    if( args[1]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_ci_ess");
    }

    res = plot_new_value();
    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    if( tolower(args[0]->u.boolean.val) < tolower(args[1]->u.boolean.val) ){
        res->u.boolean.val = true;
    }

    return res;
}

/* (char-ci>? char1 char2)
 */
struct plot_value * plot_func_char_ci_greater(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_ci_reater");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_ci_reater");
    }

    if( args[1]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_ci_reater");
    }

    res = plot_new_value();
    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    if( tolower(args[0]->u.boolean.val) > tolower(args[1]->u.boolean.val) ){
        res->u.boolean.val = true;
    }

    return res;
}

/* (char-ci<=? char1 char2)
 */
struct plot_value * plot_func_char_ci_less_equal(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_ci_ess_equal");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_ci_ess_equal");
    }

    if( args[1]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_ci_ess_equal");
    }

    res = plot_new_value();
    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    if( tolower(args[0]->u.boolean.val) <= tolower(args[1]->u.boolean.val) ){
        res->u.boolean.val = true;
    }

    return res;
}

/* (char-ci>=? char1 char2)
 */
struct plot_value * plot_func_char_ci_greater_equal(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_greater_ci_qual");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_ci_reater_equal");
    }

    if( args[1]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_ci_reater_equal");
    }

    res = plot_new_value();
    res->type = plot_type_boolean;
    res->u.boolean.val = false;

    if( tolower(args[0]->u.boolean.val) >= tolower(args[1]->u.boolean.val) ){
        res->u.boolean.val = true;
    }

    return res;
}



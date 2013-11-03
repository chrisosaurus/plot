#include <ctype.h> /* tolower, toupper, isalpha, isdigit, isspace, isupper, islower */

#include "value.h"
#include "funcs.h"

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

#define DEBUG 0

/****** character procedures ******/

/* char?
 */
struct plot_value * plot_func_char_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;
    plot_value *val;

    #if DEBUG
    puts("inside plot_func_char_test");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    res = plot_new_value();
    if( ! res ){
        #if DEBUG
        puts("called to plot_new_value failed");
        #endif
        return 0; /* FIXME error */
    }

    if( argc != 1 ){
        #if DEBUG
        puts("incorrect number of args to plot_func_char_test");
        #endif
        return 0; /* FIXME error */
    }

    val = args[0];

    if( ! val ){
        #if DEBUG
        puts("call to plot_eval_expr returned NULL");
        #endif
        return 0; /* FIXME error */
    }

    res->type = plot_type_boolean;
    if( val->type == plot_type_character ){
        res->u.boolean.val = true;
        return res;
    } else {
        res->u.boolean.val = false;
        return res;
    }

}

/* (char=? char1 char2)
 * character equality test
 */
struct plot_value * plot_func_char_equal_test(struct plot_env *env, struct plot_value **args, int argc){
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
struct plot_value * plot_func_char_ci_equal_test(struct plot_env *env, struct plot_value **args, int argc){
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
struct plot_value * plot_func_char_less_test(struct plot_env *env, struct plot_value **args, int argc){
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
struct plot_value * plot_func_char_greater_test(struct plot_env *env, struct plot_value **args, int argc){
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
struct plot_value * plot_func_char_less_equal_test(struct plot_env *env, struct plot_value **args, int argc){
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
struct plot_value * plot_func_char_greater_equal_test(struct plot_env *env, struct plot_value **args, int argc){
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
struct plot_value * plot_func_char_ci_less_test(struct plot_env *env, struct plot_value **args, int argc){
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
struct plot_value * plot_func_char_ci_greater_test(struct plot_env *env, struct plot_value **args, int argc){
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
struct plot_value * plot_func_char_ci_less_equal_test(struct plot_env *env, struct plot_value **args, int argc){
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
struct plot_value * plot_func_char_ci_greater_equal_test(struct plot_env *env, struct plot_value **args, int argc){
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

/* (char-alphabetic? char)
 */
struct plot_value * plot_func_char_alphabetic_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_alphabetic_test");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_alphabetic_test");
    }

    res = plot_new_value();
    res->type = plot_type_boolean;
    res->u.boolean.val = isalpha(args[0]->u.character.val);

    return res;
}

/* (char-numeric? char)
 */
struct plot_value * plot_func_char_numeric_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_numeric_test");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_numeric_test");
    }

    res = plot_new_value();
    res->type = plot_type_boolean;
    res->u.boolean.val = isdigit(args[0]->u.character.val);

    return res;

}

/* (char-whitespace? char)
 */
struct plot_value * plot_func_char_whitespace_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_whitespace_test");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_whitespace_test");
    }

    res = plot_new_value();
    res->type = plot_type_boolean;
    res->u.boolean.val = isspace(args[0]->u.character.val);

    return res;
}

/* (char-upper-case? char)
 */
struct plot_value * plot_func_char_upper_case_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_upper_case_test");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_upper_case_test");
    }

    res = plot_new_value();
    res->type = plot_type_boolean;
    res->u.boolean.val = isupper(args[0]->u.character.val);

    return res;
}

/* (char-lower-case? char)
 */
struct plot_value * plot_func_char_lower_case_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_lower_case_test");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_lower_case_test");
    }

    res = plot_new_value();
    res->type = plot_type_boolean;
    res->u.boolean.val = islower(args[0]->u.character.val);

    return res;
}

/* (char->integer char)
 */
struct plot_value * plot_func_char_to_integer(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_to_integer");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_to_integer");
    }

    res = plot_new_value();
    res->type = plot_type_number;
    res->u.number.val = args[0]->u.character.val;

    return res;
}

/* (integer->char n)
 */
struct plot_value * plot_func_integer_to_char(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_integer_to_char");
    }

    if( args[0]->type != plot_type_number ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_integer_to_char");
    }

    res = plot_new_value();
    res->type = plot_type_character;
    res->u.character.val = args[0]->u.number.val;

    return res;
}

/* (char-upcase char)
 */
struct plot_value * plot_func_char_upcase(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_upcase");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_upcase");
    }

    res = plot_new_value();
    res->type = plot_type_character;
    res->u.character.val = toupper(args[0]->u.character.val);

    return res;
}

/* (char-downcase char)
 */
struct plot_value * plot_func_char_downcase(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_downcase");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_downcase");
    }

    res = plot_new_value();
    res->type = plot_type_character;
    res->u.character.val = tolower(args[0]->u.character.val);

    return res;
}



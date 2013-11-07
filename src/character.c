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

    return plot_new_boolean( val->type == plot_type_character );
}

/* (char=? char1 char2)
 * character equality test
 */
struct plot_value * plot_func_char_equal_test(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 arguments", "plot_func_char_equal");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_equal");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_equal");
    }

    return plot_new_boolean( args[0]->u.character.val == args[1]->u.character.val );
}

/* (char-ci=? char1 char2)
 * character case-insensitive equality test
 */
struct plot_value * plot_func_char_ci_equal_test(struct plot_env *env, struct plot_value **args, int argc){

    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 arguments", "plot_func_char_equal");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_equal");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_equal");
    }

    return plot_new_boolean( tolower(args[0]->u.character.val) == tolower(args[1]->u.character.val) );
}

/* (char<? char1 char2)
 */
struct plot_value * plot_func_char_less_test(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_less");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_less");
    }

    if( args[1]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_less");
    }

    return plot_new_boolean( args[0]->u.boolean.val < args[1]->u.boolean.val );
}

/* (char>? char1 char2)
 */
struct plot_value * plot_func_char_greater_test(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_greater");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_greater");
    }

    if( args[1]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_greater");
    }

    return plot_new_boolean( args[0]->u.boolean.val > args[1]->u.boolean.val );
}

/* (char<=? char1 char2)
 */
struct plot_value * plot_func_char_less_equal_test(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_less_equal");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_less_equal");
    }

    if( args[1]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_less_equal");
    }

    return plot_new_boolean( args[0]->u.boolean.val <= args[1]->u.boolean.val );
}

/* (char>=? char1 char2)
 */
struct plot_value * plot_func_char_greater_equal_test(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_greater_equal");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_greater_equal");
    }

    if( args[1]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_greater_equal");
    }

    return plot_new_boolean( args[0]->u.boolean.val >= args[1]->u.boolean.val );
}

/* (char-ci<? char1 char2)
 */
struct plot_value * plot_func_char_ci_less_test(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_ci_less");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_ci_ess");
    }

    if( args[1]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_ci_ess");
    }

    return plot_new_boolean( tolower(args[0]->u.boolean.val) < tolower(args[1]->u.boolean.val) );
}

/* (char-ci>? char1 char2)
 */
struct plot_value * plot_func_char_ci_greater_test(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_ci_reater");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_ci_reater");
    }

    if( args[1]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_ci_reater");
    }

    return plot_new_boolean( tolower(args[0]->u.boolean.val) > tolower(args[1]->u.boolean.val) );
}

/* (char-ci<=? char1 char2)
 */
struct plot_value * plot_func_char_ci_less_equal_test(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_ci_ess_equal");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_ci_ess_equal");
    }

    if( args[1]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_ci_ess_equal");
    }

    return plot_new_boolean( tolower(args[0]->u.boolean.val) <= tolower(args[1]->u.boolean.val) );
}

/* (char-ci>=? char1 char2)
 */
struct plot_value * plot_func_char_ci_greater_equal_test(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_greater_ci_qual");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_ci_reater_equal");
    }

    if( args[1]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_char_ci_reater_equal");
    }

    return plot_new_boolean( tolower(args[0]->u.boolean.val) >= tolower(args[1]->u.boolean.val) );
}

/* (char-alphabetic? char)
 */
struct plot_value * plot_func_char_alphabetic_test(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_alphabetic_test");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_alphabetic_test");
    }

    return plot_new_boolean( isalpha(args[0]->u.character.val) );
}

/* (char-numeric? char)
 */
struct plot_value * plot_func_char_numeric_test(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_numeric_test");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_numeric_test");
    }

    return plot_new_boolean( isdigit(args[0]->u.character.val) );
}

/* (char-whitespace? char)
 */
struct plot_value * plot_func_char_whitespace_test(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_whitespace_test");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_whitespace_test");
    }

    return plot_new_boolean( isspace(args[0]->u.character.val) );
}

/* (char-upper-case? char)
 */
struct plot_value * plot_func_char_upper_case_test(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_upper_case_test");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_upper_case_test");
    }

    return plot_new_boolean( isupper(args[0]->u.character.val) );
}

/* (char-lower-case? char)
 */
struct plot_value * plot_func_char_lower_case_test(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_lower_case_test");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_lower_case_test");
    }

    return plot_new_boolean( islower(args[0]->u.character.val) );
}

/* (char->integer char)
 */
struct plot_value * plot_func_char_to_integer(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_to_integer");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_to_integer");
    }

    return plot_new_number( args[0]->u.character.val );
}

/* (integer->char n)
 */
struct plot_value * plot_func_integer_to_char(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_integer_to_char");
    }

    if( args[0]->type != plot_type_number ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_integer_to_char");
    }

    return plot_new_character( args[0]->u.number.val );
}

/* (char-upcase char)
 */
struct plot_value * plot_func_char_upcase(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_upcase");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_upcase");
    }

    return plot_new_character( toupper(args[0]->u.character.val) );
}

/* (char-downcase char)
 */
struct plot_value * plot_func_char_downcase(struct plot_env *env, struct plot_value **args, int argc){
    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_downcase");
    }

    if( args[0]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_downcase");
    }

    return plot_new_character( tolower(args[0]->u.character.val) );
}



#include <ctype.h> /* tolower, toupper, isalpha, isdigit, isspace, isupper, islower */

#include "value.h"
#include "funcs.h"
#include "character.h"

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

#define DEBUG 0

/****** character procedures ******/

/* char?
 */
struct plot_value * plot_func_char_test(struct plot_env *env, struct plot_value *args){
    plot_value *val;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_test");
    }

    val = car(args);

    return plot_new_boolean( val->type == plot_type_character );
}

/* (char=? char1 char2)
 * character equality test
 */
struct plot_value * plot_func_char_equal_test(struct plot_env *env, struct plot_value *args){
    plot_value *o1, *o2;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_pair || cdr(cdr(args))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 arguments", "plot_func_char_equal");
    }

    o1 = car(args);
    o2 = car(cdr(args));

    if( o1->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    if( o2->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    return plot_new_boolean( o1->u.character.val == o2->u.character.val );
}

/* (char-ci=? char1 char2)
 * character case-insensitive equality test
 */
struct plot_value * plot_func_char_ci_equal_test(struct plot_env *env, struct plot_value *args){
    plot_value *o1, *o2;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_pair || cdr(cdr(args))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 arguments", "plot_func_char_equal");
    }

    o1 = car(args);
    o2 = car(cdr(args));

    if( o1->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    if( o2->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    return plot_new_boolean( tolower(o1->u.character.val) == tolower(o2->u.character.val) );
}

/* (char<? char1 char2)
 */
struct plot_value * plot_func_char_less_test(struct plot_env *env, struct plot_value *args){
    plot_value *o1, *o2;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_pair || cdr(cdr(args))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_less");
    }

    o1 = car(args);
    o2 = car(cdr(args));

    if( o1->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    if( o2->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    return plot_new_boolean( o1->u.character.val < o2->u.character.val );
}

/* (char>? char1 char2)
 */
struct plot_value * plot_func_char_greater_test(struct plot_env *env, struct plot_value *args){
    plot_value *o1, *o2;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_pair || cdr(cdr(args))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_greater");
    }

    o1 = car(args);
    o2 = car(cdr(args));

    if( o1->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    if( o2->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    return plot_new_boolean( o1->u.character.val > o2->u.character.val );
}

/* (char<=? char1 char2)
 */
struct plot_value * plot_func_char_less_equal_test(struct plot_env *env, struct plot_value *args){
    plot_value *o1, *o2;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_pair || cdr(cdr(args))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_less_equal");
    }

    o1 = car(args);
    o2 = car(cdr(args));

    if( o1->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    if( o2->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    return plot_new_boolean( o1->u.character.val <= o2->u.character.val );
}

/* (char>=? char1 char2)
 */
struct plot_value * plot_func_char_greater_equal_test(struct plot_env *env, struct plot_value *args){
    plot_value *o1, *o2;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_pair || cdr(cdr(args))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_greater_equal");
    }

    o1 = car(args);
    o2 = car(cdr(args));

    if( o1->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    if( o2->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    return plot_new_boolean( o1->u.character.val >= o2->u.character.val );
}

/* (char-ci<? char1 char2)
 */
struct plot_value * plot_func_char_ci_less_test(struct plot_env *env, struct plot_value *args){
    plot_value *o1, *o2;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_pair || cdr(cdr(args))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_ci_less");
    }

    o1 = car(args);
    o2 = car(cdr(args));

    if( o1->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    if( o2->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    return plot_new_boolean( tolower(o1->u.character.val) < tolower(o2->u.character.val) );
}

/* (char-ci>? char1 char2)
 */
struct plot_value * plot_func_char_ci_greater_test(struct plot_env *env, struct plot_value *args){
    plot_value *o1, *o2;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_pair || cdr(cdr(args))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_ci_reater");
    }

    o1 = car(args);
    o2 = car(cdr(args));

    if( o1->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    if( o2->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    return plot_new_boolean( tolower(o1->u.character.val) > tolower(o2->u.character.val) );
}

/* (char-ci<=? char1 char2)
 */
struct plot_value * plot_func_char_ci_less_equal_test(struct plot_env *env, struct plot_value *args){
    plot_value *o1, *o2;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_pair || cdr(cdr(args))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_ci_ess_equal");
    }

    o1 = car(args);
    o2 = car(cdr(args));

    if( o1->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    if( o2->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    return plot_new_boolean( tolower(o1->u.character.val) <= tolower(o2->u.character.val) );
}

/* (char-ci>=? char1 char2)
 */
struct plot_value * plot_func_char_ci_greater_equal_test(struct plot_env *env, struct plot_value *args){
    plot_value *o1, *o2;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_pair || cdr(cdr(args))->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_char_greater_ci_qual");
    }

    o1 = car(args);
    o2 = car(cdr(args));

    if( o1->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    if( o2->type != plot_type_character ){
        return plot_new_boolean(0);
    }

    return plot_new_boolean( tolower(o1->u.character.val) >= tolower(o2->u.character.val) );
}

/* (char-alphabetic? char)
 */
struct plot_value * plot_func_char_alphabetic_test(struct plot_env *env, struct plot_value *args){
    plot_value *o1;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_alphabetic_test");
    }

    o1 = car(args);

    if( o1->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_alphabetic_test");
    }

    return plot_new_boolean( isalpha(o1->u.character.val) );
}

/* (char-numeric? char)
 */
struct plot_value * plot_func_char_numeric_test(struct plot_env *env, struct plot_value *args){
    plot_value *o1;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_numeric_test");
    }

    o1 = car(args);

    if( o1->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_numeric_test");
    }

    return plot_new_boolean( isdigit(o1->u.character.val) );
}

/* (char-whitespace? char)
 */
struct plot_value * plot_func_char_whitespace_test(struct plot_env *env, struct plot_value *args){
    plot_value *o1;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_whitespace_test");
    }

    o1 = car(args);

    if( o1->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_whitespace_test");
    }

    return plot_new_boolean( isspace(o1->u.character.val) );
}

/* (char-upper-case? char)
 */
struct plot_value * plot_func_char_upper_case_test(struct plot_env *env, struct plot_value *args){
    plot_value *o1;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_upper_case_test");
    }

    o1 = car(args);

    if( o1->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_upper_case_test");
    }

    return plot_new_boolean( isupper(o1->u.character.val) );
}

/* (char-lower-case? char)
 */
struct plot_value * plot_func_char_lower_case_test(struct plot_env *env, struct plot_value *args){
    plot_value *o1;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_lower_case_test");
    }

    o1 = car(args);

    if( o1->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_lower_case_test");
    }

    return plot_new_boolean( islower(o1->u.character.val) );
}

/* (char->integer char)
 */
struct plot_value * plot_func_char_to_integer(struct plot_env *env, struct plot_value *args){
    plot_value *o1;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_to_integer");
    }

    o1 = car(args);

    if( o1->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_to_integer");
    }

    return plot_new_number( o1->u.character.val );
}

/* (integer->char n)
 */
struct plot_value * plot_func_integer_to_char(struct plot_env *env, struct plot_value *args){
    plot_value *o1;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_integer_to_char");
    }

    o1 = car(args);

    if( o1->type != plot_type_number ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_integer_to_char");
    }

    return plot_new_character( o1->u.number.val );
}

/* (char-upcase char)
 */
struct plot_value * plot_func_char_upcase(struct plot_env *env, struct plot_value *args){
    plot_value *o1;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_upcase");
    }

    o1 = car(args);

    if( o1->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_upcase");
    }

    return plot_new_character( toupper(o1->u.character.val) );
}

/* (char-downcase char)
 */
struct plot_value * plot_func_char_downcase(struct plot_env *env, struct plot_value *args){
    plot_value *o1;

    if( args->type != plot_type_pair || cdr(args)->type != plot_type_null ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 1 arg", "plot_func_char_downcase");
    }

    o1 = car(args);

    if( o1->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_character", "plot_func_char_downcase");
    }

    return plot_new_character( tolower(o1->u.character.val) );
}



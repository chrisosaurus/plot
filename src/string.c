#include <ctype.h> /* tolower */
#include <string.h>
#include <stdio.h>

#include "string.h"
#include "value.h"

/* ignore unused parameter warnings */
#pragma GCC diagnostic ignored "-Wunused-parameter"

#define DEBUG 0

/******* string procedures *******/

/* string?
 */
struct plot_value * plot_func_string_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;
    plot_value *val;

    #if DEBUG
    puts("inside plot_func_boolean_test");
    #endif

    if( ! env ){
        #if DEBUG
        puts("env is NULL");
        #endif
        return 0; /* FIXME error */
    }

    res = plot_alloc_value();
    if( ! res ){
        #if DEBUG
        puts("called to plot_alloc_value failed");
        #endif
        return 0; /* FIXME error */
    }

    if( argc != 1 ){
        #if DEBUG
        puts("incorrect number of args to plot_func_string_test");
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
    if( val->type == plot_type_string ){
        res->u.boolean.val = true;
        return res;
    } else {
        res->u.boolean.val = false;
        return res;
    }
}

/* return plot number representing length of string (excluding null terminator)
 * 'number of characters in string'
 */
struct plot_value * plot_func_string_length(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *ret;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected 1 arg", "plot_func_string_length");
    }

    if( args[0]->type != plot_type_string ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_string", "plot_func_string_lenght");
    }

    ret = plot_alloc_value();
    ret->type = plot_type_number;
    ret->u.number.val = args[0]->u.string.len - 1; /* len includes null terminator */

    return ret;
}

/* (substring string start end)
 * 0 <= start <= end <= (string-length string)
 * returns a string from start (inclusive) to end (exclusive)
 */
struct plot_value * plot_func_substring(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;
    int start, end;

    if( argc != 3 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 3 args", "plot_func_substring");
    }

    if( args[0]->type != plot_type_string ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_string", "plot_func_substring");
    }

    if( args[1]->type != plot_type_number ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_number", "plot_func_substring");
    }
    start = args[1]->u.number.val;

    if( args[2]->type != plot_type_number ){
        return plot_runtime_error(plot_error_bad_args, "third arg was not of type plot_type_number", "plot_func_substring");
    }
    end = args[2]->u.number.val;

    if( start > end ){
        return plot_runtime_error(plot_error_bad_args, "start was greater than end, impossible substring", "plot_func_substring");
    }

    if( start < 0 ){
        return plot_runtime_error(plot_error_bad_args, "provided start was < 0", "plot_func_substring");
    }

    /* len is 1-based counting and includes \0 terminator
     * end is 0-based counting
     */
    if( end >= (args[0]->u.string.len - 1) ){
        return plot_runtime_error(plot_error_bad_args, "provided end value is not within string", "plot_func_substring");
    }

    res = plot_alloc_value();
    res->type = plot_type_string;
    res->u.string.len = (end - start) + 1;
    res->u.string.size = res->u.string.len;
    res->u.string.val = plot_alloc_string( res->u.string.len );

    /* start is inclusive and end is exclusive */
    strncpy(res->u.string.val, &(args[0]->u.string.val[start]), end-start);
    res->u.string.val[ end-start ] = '\0';

    return res;
}

/* (string-append string ...)
 * returns a new string whose characters form the concatentation
 * of the given strings
 */
struct plot_value * plot_func_string_append(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;
    int i;
    int len = 0;

    if( argc < 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected at least one argument", "plot_func_substring");
    }

    for( i=0; i<argc; ++i ){
        if( args[i]->type != plot_type_string ){
            return plot_runtime_error(plot_error_bad_args, "argument was not of type plot_type_string", "plot_func_substring");
        }
        len += args[i]->u.string.len - 1; /* do not include null terminator in count */
    }

    /* our final null terminator */
    len += 1;

    res = plot_alloc_value();
    res->type = plot_type_string;
    res->u.string.len = len;
    res->u.string.size = len;
    res->u.string.val = plot_alloc_string(len);

    /* we now start using len to refer to the next place in our
     * string to write
     */
    len = 0;
    for( i=0; i<argc; ++i ){
        strncpy( &(res->u.string.val[len]), args[i]->u.string.val, args[i]->u.string.len );
        len += args[i]->u.string.len - 1;
    }

    return res;
}

/* (string-copy string)
 * returns a newly allocated copy of the given string
 */
struct plot_value * plot_func_string_copy(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *ret;

    if( argc != 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected 1 arg", "plot_func_string_copy");
    }

    if( args[0]->type != plot_type_string ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_string", "plot_func_string_copy");
    }

    ret = plot_alloc_value();
    ret->type = plot_type_string;
    ret->u.string.size = args[0]->u.string.size;
    ret->u.string.len = args[0]->u.string.len;

    ret->u.string.val = plot_alloc_string(ret->u.string.len);
    strncpy(ret->u.string.val, args[0]->u.string.val, ret->u.string.len - 1);

    return ret;
}

/* (string=? str1 str2)
 * string equality test (case sensitive)
 * returns #t iff both strings are the same length and contain the same characters
 */
struct plot_value * plot_func_string_equal_test(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *ret;

    /* FIXME r5rs page 30 section 6.3.5 allows
     * for an implementation to generalise equality functions
     * to many args
     */
    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_string_equal");
    }

    /* FIXME r5rs page 30 section 6.3.5 does not seem to mention
     * what happens if procedures are not called with strings
     *
     * runtime error vs false
     */
    if( args[0]->type != plot_type_string ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_string", "plot_func_string_equal");
    }

    if( args[1]->type != plot_type_string ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_string", "plot_func_string_equal");
    }

    ret = plot_alloc_value();
    ret->type = plot_type_boolean;
    ret->u.boolean.val = false;

    if( args[0]->u.string.len != args[1]->u.string.len ){
        return ret;
    }

    if( strcmp(args[0]->u.string.val, args[1]->u.string.val) ){
        return ret;
    }

    ret->u.boolean.val = true;
    return ret;
}

/* (string-ci=? str1 str2)
 * string case-insensitive equality test
 * returns #t iff both string are the same length and contains the same characters
 */
struct plot_value * plot_func_string_ci_equal(struct plot_env *env, struct plot_value **args, int argc){
    int i;
    plot_value *ret, *a, *b;

    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected 2 args", "plot_func_string_ci_equal");
    }

    a = args[0];
    if( a->type != plot_type_string ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_string", "plot_func_string_ci_equal");
    }

    b = args[1];
    if( b->type != plot_type_string ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_string", "plot_func_string_ci_equal");
    }

    ret = plot_alloc_value();
    ret->type = plot_type_boolean;
    ret->u.boolean.val = false;

    if( a->u.string.len != b->u.string.len ){
        return ret;
    }

    for( i=0; i< a->u.string.len; ++i ){
        if( tolower(a->u.string.val[i]) != tolower(b->u.string.val[i]) ){
            return ret;
        }
    }

    ret->u.boolean.val = true;
    return ret;
}

/* (make-string len)
 * (make-string len char)
 */
struct plot_value * plot_func_make_string(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;

    if( argc != 1 && argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected either 1 or 2 arguments", "plot_func_make_string");
    }

    if( args[0]->type != plot_type_number ){
        return plot_runtime_error(plot_error_bad_args, "first argument was not of type plot_type_number", "plot_func_make_string");
    }

    res = plot_alloc_value();
    res->type = plot_type_string;
    res->u.string.len = args[0]->u.number.val + 1;
    res->u.string.size = res->u.string.len;
    res->u.string.val = plot_alloc_string( res->u.string.len );

    if( argc == 2 ){
        if( args[1]->type != plot_type_character ){
            plot_value_decr(res);
            return plot_runtime_error(plot_error_bad_args, "second argument was not of type plot_type_character", "plot_func_make_string");
        }
        memset( res->u.string.val, args[1]->u.character.val, sizeof(char) * (res->u.string.len - 1) );
        res->u.string.val[ res->u.string.len - 1] = '\0';
    } else {
        res->u.string.val[0] = '\0';
    }

    return res;
}

/* (string char ...)
 * FIXME spec is unclear about the minimum number of arguments
 * I will assume 1 as a minimum
 */
struct plot_value * plot_func_string(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;
    int i;

    if( argc < 1 ){
        return plot_runtime_error(plot_error_bad_args, "expected at least 1 argument", "plot_func_string");
    }

    res = plot_alloc_value();
    res->type = plot_type_string;
    res->u.string.len = argc + 1; /* null terminator */
    res->u.string.size = res->u.string.len;
    res->u.string.val = plot_alloc_string( res->u.string.len );

    for(i=0; i<argc; ++i ){
        if( args[i]->type != plot_type_character ){
            plot_value_decr(res);
            return plot_runtime_error(plot_error_bad_args, "argument was not of type plot_type_character", "plot_func_string");
        }
        res->u.string.val[i] = args[i]->u.character.val;
    }

    res->u.string.val[ argc ] = '\0';
    return res;
}

/* (string-ref string k)
 * return char at k, zero-origin indexing
 */
struct plot_value * plot_func_string_ref(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;
    int i;

    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 arguments", "plot_func_string_ref");
    }

    if( args[0]->type != plot_type_string ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_string", "plot_func_string_ref");
    }

    if( args[1]->type != plot_type_number ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_number", "plot_func_string_ref");
    }

    i = args[1]->u.number.val;

    /* NB: len - 1 as len is a size (counts from 1) and includes null terminator
     * whereas index is an index (counts from 0)
     *
     * >= as if we have size 5 (excluding null term) then 5 is not a valid index (highest valid index would be 4)
     */
    if( i >= args[0]->u.string.len - 1 ){
        return plot_runtime_error(plot_error_bad_args, "supplied index was out of range", "plot_func_string_ref");
    }

    res = plot_alloc_value();
    res->type = plot_type_character;
    res->u.character.val = args[0]->u.string.val[i];

    return res;
}

/* (string-set! string k char)
 * k is index, set character at index k to character char
 */
struct plot_value * plot_func_string_set(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;
    int i;

    if( argc != 3 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 3 arguments", "plot_func_string_set");
    }

    if( args[0]->type != plot_type_string ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_string", "plot_func_string_set");
    }

    if( args[1]->type != plot_type_number ){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_number", "plot_func_string_set");
    }

    i = args[1]->u.number.val;

    /* NB: len - 1 as len is a size (counts from 1) and includes null terminator
     * whereas index is an index (counts from 0)
     *
     * >= as if we have size 5 (excluding null term) then 5 is not a valid index (highest valid index would be 4)
     */
    if( i >= args[0]->u.string.len - 1 ){
        return plot_runtime_error(plot_error_bad_args, "specified index was out of range", "plot_func_string_set");
    }

    if( args[2]->type != plot_type_character ){
        return plot_runtime_error(plot_error_bad_args, "third arg was not of type plot_type_character", "plot_func_string_set");
    }

    args[0]->u.string.val[i] = args[2]->u.character.val;

    res = plot_alloc_value();
    res->type = plot_type_unspecified;
    return res;
}

/* (string<? string1 string2)
 */
struct plot_value * plot_func_string_less_test(struct plot_env *env, struct plot_value **args, int argc){
    return plot_runtime_error(plot_error_unimplemented, "not yet implemented", "plot_func_string_less_test");
}

/* (string>? string1 string2)
 */
struct plot_value * plot_func_string_greater_test(struct plot_env *env, struct plot_value **args, int argc){
    return plot_runtime_error(plot_error_unimplemented, "not yet implemented", "plot_func_string_greater_test");
}

/* (string<=? string1 string2)
 */
struct plot_value * plot_func_string_less_equal_test(struct plot_env *env, struct plot_value **args, int argc){
    return plot_runtime_error(plot_error_unimplemented, "not yet implemented", "plot_func_string_less_equal_test");
}

/* (string>=? string1 string 2)
 */
struct plot_value * plot_func_string_greater_equal_test(struct plot_env *env, struct plot_value **args, int argc){
    return plot_runtime_error(plot_error_unimplemented, "not yet implemented", "plot_func_string_greater_equal_test");
}

/* (string-ci<? string1 string 2)
 */
struct plot_value * plot_func_string_ci_less_test(struct plot_env *env, struct plot_value **args, int argc){
    return plot_runtime_error(plot_error_unimplemented, "not yet implemented", "plot_func_string_ci_less_test");
}

/* (string-ci>? string1 string 2)
 */
struct plot_value * plot_func_string_ci_greater_test(struct plot_env *env, struct plot_value **args, int argc){
    return plot_runtime_error(plot_error_unimplemented, "not yet implemented", "plot_func_string_ci_greater_test");
}

/* (string-ci<=? string1 string 2)
 */
struct plot_value * plot_func_string_ci_less_equal_test(struct plot_env *env, struct plot_value **args, int argc){
    return plot_runtime_error(plot_error_unimplemented, "not yet implemented", "plot_func_ci_less_equal_test");
}

/* (string-ci>=? string1 string2)
 */
struct plot_value * plot_func_string_ci_greater_equal_test(struct plot_env *env, struct plot_value **args, int argc){
    return plot_runtime_error(plot_error_unimplemented, "not yet implemented", "plot_func_string_ci_greater_equal_test");
}

/* (string->list string)
 */
struct plot_value * plot_func_string_to_list(struct plot_env *env, struct plot_value **args, int argc){
    return plot_runtime_error(plot_error_unimplemented, "not yet implemented", "plot_func_string_to_list");
}

/* (list->string list)
 */
struct plot_value * plot_func_list_to_string(struct plot_env *env, struct plot_value **args, int argc){
    return plot_runtime_error(plot_error_unimplemented, "not yet implemented", "plot_func_list_to_string");
}

/* (string-fill! string char)
 */
struct plot_value * plot_func_string_fill(struct plot_env *env, struct plot_value **args, int argc){
    plot_value *res;
    int i;
    char ch;

    if( argc != 2 ){
        return plot_runtime_error(plot_error_bad_args, "expected exactly 2 arguments", "plot_func_string_fill");
    }

    if( args[0]->type != plot_type_string ){
        return plot_runtime_error(plot_error_bad_args, "first arg was not of type plot_type_string", "plot_func_string_fill");
    }

    if( args[1]->type != plot_type_character){
        return plot_runtime_error(plot_error_bad_args, "second arg was not of type plot_type_character", "plot_func_string_fill");
    }

    ch = args[1]->u.character.val;
    res = plot_alloc_value();

    /* NB: len - 1 as len is a size (counts from 1) and includes null terminator
     * whereas i is an index (counts from 0)
     *
     * >= as if we have size 5 (excluding null term) then 5 is not a valid index (highest valid index would be 4)
     */
    for(i=0; i < args[0]->u.string.len - 1; ++i){
        args[0]->u.string.val[i] = ch;
    }

    return res;
}



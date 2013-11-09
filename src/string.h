#ifndef PLOT_STRING_H
#define PLOT_STRING_H

struct plot_env;
struct plot_value;

/******* string procedures *******/

/* (string? obj)
 */
struct plot_value * plot_func_string_test(struct plot_env *env, struct plot_value **args, int argc);

/* (string-length string)
 * return plot number representing length of string (excluding null terminator)
 * 'number of characters in string'
 */
struct plot_value * plot_func_string_length(struct plot_env *env, struct plot_value **args, int argc);

/* (substring string start end)
 * 0 <= start <= end <= (string-length string)
 * returns a string from start (inclusive) to end (exclusive)
 */
struct plot_value * plot_func_substring(struct plot_env *env, struct plot_value **args, int argc);

/* (string-append string ...)
 * returns a new string whose characters form the concatentation
 * of the given strings
 */
struct plot_value * plot_func_string_append(struct plot_env *env, struct plot_value **args, int argc);

/* (string-copy string)
 * returns a newly allocated copy of the given string
 */
struct plot_value * plot_func_string_copy(struct plot_env *env, struct plot_value **args, int argc);

/* (string=? str1 str2)
 * string equality test (case sensitive)
 * returns #t iff both strings are the same length and contain the same characters
 */
struct plot_value * plot_func_string_equal_test(struct plot_env *env, struct plot_value **args, int argc);

/* (string-ci=? str1 str2)
 * string case-insensitive equality test
 * returns #t iff both string are the same length and contains the same characters
 */
struct plot_value * plot_func_string_ci_equal(struct plot_env *env, struct plot_value **args, int argc);

/* (make-string len)
 * (make-string len char)
 */
struct plot_value * plot_func_make_string(struct plot_env *env, struct plot_value **args, int argc);

/* (string char ...)
 */
struct plot_value * plot_func_string(struct plot_env *env, struct plot_value **args, int argc);

/* (string-ref string k)
 * return char at k, zero-origin indexing
 */
struct plot_value * plot_func_string_ref(struct plot_env *env, struct plot_value **args, int argc);

/* (string-set! string k char)
 */
struct plot_value * plot_func_string_set(struct plot_env *env, struct plot_value **args, int argc);

/* (string<? string1 string2)
 */
struct plot_value * plot_func_string_less_test(struct plot_env *env, struct plot_value **args, int argc);

/* (string>? string1 string2)
 */
struct plot_value * plot_func_string_greater_test(struct plot_env *env, struct plot_value **args, int argc);

/* (string<=? string1 string2)
 */
struct plot_value * plot_func_string_less_equal_test(struct plot_env *env, struct plot_value **args, int argc);

/* (string>=? string1 string 2)
 */
struct plot_value * plot_func_string_greater_equal_test(struct plot_env *env, struct plot_value **args, int argc);

/* (string-ci<? string1 string 2)
 */
struct plot_value * plot_func_string_ci_less_test(struct plot_env *env, struct plot_value **args, int argc);

/* (string-ci>? string1 string 2)
 */
struct plot_value * plot_func_string_ci_greater_test(struct plot_env *env, struct plot_value **args, int argc);

/* (string-ci<=? string1 string 2)
 */
struct plot_value * plot_func_string_ci_less_equal_test(struct plot_env *env, struct plot_value **args, int argc);

/* (string-ci>=? string1 string2)
 */
struct plot_value * plot_func_string_ci_greater_equal_test(struct plot_env *env, struct plot_value **args, int argc);

/* (string->list string)
 */
struct plot_value * plot_func_string_to_list(struct plot_env *env, struct plot_value **args, int argc);

/* (list->string list)
 */
struct plot_value * plot_func_list_to_string(struct plot_env *env, struct plot_value **args, int argc);

/* (string-fill! string char)
 */
struct plot_value * plot_func_string_fill(struct plot_env *env, struct plot_value **args, int argc);


#endif

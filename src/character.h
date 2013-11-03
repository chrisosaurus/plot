#ifndef PLOT_CHARACTER_H
#define PLOT_CHARACTER_H

struct plot_env;
struct plot_value;

/****** character procedures ******/

/* char?
 */
struct plot_value * plot_func_char_test(struct plot_env *env, struct plot_value **args, int argc);

/* (char=? char1 char2)
 * character equality test
 */
struct plot_value * plot_func_char_equal_test(struct plot_env *env, struct plot_value **args, int argc);

/* (char-ci=? char1 char2)
 * character case-insensitive equality test
 */
struct plot_value * plot_func_char_ci_equal_test(struct plot_env *env, struct plot_value **args, int argc);

/* (char<? char1 char2)
 */
struct plot_value * plot_func_char_less_test(struct plot_env *env, struct plot_value **args, int argc);

/* (char>? char1 char2)
 */
struct plot_value * plot_func_char_greater_test(struct plot_env *env, struct plot_value **args, int argc);

/* (char<=? char1 char2)
 */
struct plot_value * plot_func_char_less_equal_test(struct plot_env *env, struct plot_value **args, int argc);

/* (char>=? char1 char2)
 */
struct plot_value * plot_func_char_greater_equal_test(struct plot_env *env, struct plot_value **args, int argc);

/* (char-ci<? char1 char2)
 */
struct plot_value * plot_func_char_ci_less_test(struct plot_env *env, struct plot_value **args, int argc);

/* (char-ci>? char1 char2)
 */
struct plot_value * plot_func_char_ci_greater_test(struct plot_env *env, struct plot_value **args, int argc);

/* (char-ci<=? char1 char2)
 */
struct plot_value * plot_func_char_ci_less_equal_test(struct plot_env *env, struct plot_value **args, int argc);

/* (char-ci>=? char1 char2)
 */
struct plot_value * plot_func_char_ci_greater_equal_test(struct plot_env *env, struct plot_value **args, int argc);

/* (char-alphabetic? char)
 */
struct plot_value * plot_func_char_alphabetic_test(struct plot_env *env, struct plot_value **args, int argc);

/* (char-numeric? char)
 */
struct plot_value * plot_func_char_numeric_test(struct plot_env *env, struct plot_value **args, int argc);

/* (char-whitespace? char)
 */
struct plot_value * plot_func_char_whitespace_test(struct plot_env *env, struct plot_value **args, int argc);

/* (char-upper-case? char)
 */
struct plot_value * plot_func_char_upper_case_test(struct plot_env *env, struct plot_value **args, int argc);

/* (char-lower-case? char)
 */
struct plot_value * plot_func_char_lower_case_test(struct plot_env *env, struct plot_value **args, int argc);

/* (char->integer char)
 */
struct plot_value * plot_func_char_to_integer(struct plot_env *env, struct plot_value **args, int argc);

/* (integer->char n)
 */
struct plot_value * plot_func_integer_to_char(struct plot_env *env, struct plot_value **args, int argc);

/* (char-upcase char)
 */
struct plot_value * plot_func_char_upcase(struct plot_env *env, struct plot_value **args, int argc);

/* (char-downcase char)
 */
struct plot_value * plot_func_char_downcase(struct plot_env *env, struct plot_value **args, int argc);

#endif

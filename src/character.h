#ifndef PLOT_CHARACTER_H
#define PLOT_CHARACTER_H

struct plot_env;
struct plot_value;

/****** character procedures ******/

/* (char=? char1 char2)
 * character equality test
 */
struct plot_value * plot_func_char_equal(struct plot_env *env, struct plot_value **args, int argc);

/* (char-ci=? char1 char2)
 * character case-insensitive equality test
 */
struct plot_value * plot_func_char_ci_equal(struct plot_env *env, struct plot_value **args, int argc);

/* (char<? char1 char2)
 */
struct plot_value * plot_func_char_less(struct plot_env *env, struct plot_value **args, int argc);

/* (char>? char1 char2)
 */
struct plot_value * plot_func_char_greater(struct plot_env *env, struct plot_value **args, int argc);

/* (char<=? char1 char2)
 */
struct plot_value * plot_func_char_less_equal(struct plot_env *env, struct plot_value **args, int argc);

/* (char>=? char1 char2)
 */
struct plot_value * plot_func_char_greater_equal(struct plot_env *env, struct plot_value **args, int argc);

/* (char-ci<? char1 char2)
 */
struct plot_value * plot_func_char_ci_less(struct plot_env *env, struct plot_value **args, int argc);

/* (char-ci>? char1 char2)
 */
struct plot_value * plot_func_char_ci_greater(struct plot_env *env, struct plot_value **args, int argc);

/* (char-ci<=? char1 char2)
 */
struct plot_value * plot_func_char_ci_less_equal(struct plot_env *env, struct plot_value **args, int argc);

/* (char-ci>=? char1 char2)
 */
struct plot_value * plot_func_char_ci_greater_equal(struct plot_env *env, struct plot_value **args, int argc);

#endif

#ifndef PLOT_H
#define PLOT_H

/* plot maintains internal state
 * plot_init sets up internal state and must be called
 * before any other methods
 *
 void* returns 1 on success or 0 on failure
 */
int plot_init(void);

struct plot_env;
/* return root env
 * root env represents 'global' scope
 */
struct plot_env * plot_get_env(void);

/* plot_cleanup cleans up internal state
 * must be last method called
 */
void plot_cleanup(void);

#endif

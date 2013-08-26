#ifndef PLOT_READ_H
#define PLOT_READ_H

/* given a filename to read returns an allocated string
 * containing content
 * or 0 on error
 *
 * return string is owned by caller
 */
char * plot_read(const char * const filename);

#endif

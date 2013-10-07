VERSION = 0.1

PREFIX = /usr
MANPREFIX = ${PREFIX}/share/man

INCS =
LIBS =

CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wshadow -Wwrite-strings -Wdeclaration-after-statement ${INCS}
#CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wshadow -Wwrite-strings -Wdeclaration-after-statement -fstack-usage ${INCS}

LDFLAGS = ${LIBS}

CC = cc

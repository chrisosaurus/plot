VERSION = 0.1

PREFIX = /usr
MANPREFIX = ${PREFIX}/share/man

INCS =
LIBS =

# removing -Wwrite-strings as it is causing issues with symbols
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wshadow -Wdeclaration-after-statement -Wunused-function ${INCS}
#CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wshadow -Wwrite-strings -Wdeclaration-after-statement -fstack-usage ${INCS}

TEST_CFLAGS = -std=c99 -pedantic -Wall -Wextra -Wshadow -Wwrite-strings -Wdeclaration-after-statement -Wunused-function ${INCS}
LDFLAGS = ${LIBS}

CC = cc

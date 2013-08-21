VERSION = 0.1

PREFIX = /usr
MANPREFIX = ${PREFIX}/share/man

INCS =
LIBS =

CFLAGS = -std=c99 -pedantic -Wall -Wextra ${INCS}
LDFLAGS = ${LIBS}

CC = cc

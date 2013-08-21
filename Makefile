.POSIX:

include config.mk

HEADERS = src/pase.h
SRC = src/main.c src/parse.c
OBJ = ${SRC:.c=.o}

all: plot

.c.o:
	@echo COMPILING CC $<
	@${CC} -g -c ${CFLAGS} $< -o $@

plot: ${OBJ}
	@echo more compiling CC -o $@
	@${CC} -o $@ ${LDFLAGS} ${OBJ}

test_parse: src/parse.o
	@echo test_parse CC -o tests/test_llist.c
	@${CC} -o t/test_parse t/test_parse.c src/parse.o -lcheck

test: test_parse
	@echo running test_llist
	./t/test_parse

clean:
	@echo cleaning
	@rm -f ${OBJ} t/*.o
	@rm -f plot t/test_parse

.PHONY: all clean test test_parse obj

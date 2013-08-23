.POSIX:

include config.mk

HEADERS = src/read.h src/parse.h src/execute.h
SRC = src/read.c src/parse.c src/execute.c
OBJ = ${SRC:.c=.o}

all: plot

.c.o:
	@echo COMPILING CC $<
	@${CC} -g -c ${CFLAGS} $< -o $@

plot: ${OBJ}
	@echo more compiling CC -o $@
	@${CC} src/main.c -o $@ ${LDFLAGS} ${OBJ}

# compile tests
tests: clean ${OBJ}
	@echo test_parse CC -o tests/test_llist.c
	@${CC} -o run_tests t/test_main.c ${OBJ} -lcheck

# run tests
test: tests
	@echo running test_llist
	./run_tests

clean:
	@echo cleaning
	@rm -f src/*.o t/*.o
	@rm -f plot run_tests

.PHONY: all clean test tests obj

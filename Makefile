.POSIX:

include config.mk

SRC = src/read.c src/parse.c src/eval.c src/hash.c src/env.c
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
	# pthread, rt and m are all needed by certain versions of libcheck 
	@${CC} -o run_tests t/test_main.c ${OBJ} -lpthread -lrt -lm -lcheck

# run tests
test: tests
	@echo running test_llist
	./run_tests

clean:
	@echo cleaning
	@rm -f src/*.o t/*.o
	@rm -f plot run_tests

.PHONY: all clean test tests obj

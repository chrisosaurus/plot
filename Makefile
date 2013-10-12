.POSIX:

include config.mk

SRC = src/read.c src/parse.c src/eval.c src/hash.c src/env.c src/funcs.c src/plot.c
OBJ = ${SRC:.c=.o}

all: plot

.c.o:
	@echo COMPILING CC $<
	@${CC} -g -c ${CFLAGS} $< -o $@

plot: ${OBJ}
	@echo more compiling CC -o $@
	@${CC} src/main.c -o $@ ${LDFLAGS} ${OBJ}

# make with clang to produce different errors, includes testing code
clang:
	clang ${SRC} t/test_main.c -lcheck -Wall -Wextra

# soft version for testing half baked tests
tests_soft: clean ${OBJ}
	@echo test_parse CC -o tests/test_llist.c
	@# pthread, rt and m are all needed by certain versions of libcheck 
	@${CC} -g -o run_tests t/test_main.c ${OBJ} -lpthread -lrt -lm -lcheck

test_soft: tests_soft plot
	@echo running test_llist
	./run_tests
	@echo "\nrunning simple.scm"
	./plot t/simple.scm

# harsh version for checking sanity of test code
tests: clean ${OBJ}
	@echo test_parse CC -o tests/test_llist.c
	@# pthread, rt and m are all needed by certain versions of libcheck 
	@${CC} -g -o run_tests t/test_main.c ${OBJ} ${TEST_CFLAGS} -lpthread -lrt -lm -lcheck

# run tests
test: tests plot
	@echo running test_llist
	./run_tests
	@echo "\nrunning simple.scm"
	./plot t/simple.scm

example: plot
	cat t/simple.scm
	@echo "\n"
	./plot t/simple.scm

clean:
	@echo cleaning
	@rm -f src/*.o t/*.o src/*.su
	@rm -f plot run_tests

.PHONY: all clean test tests obj

.POSIX:

include config.mk

SRC = src/read.c src/parse.c src/eval.c src/hash.c src/env.c src/funcs.c src/plot.c
OBJ = ${SRC:.c=.o}
TOBJ = ${SRC:.c=.to}

all: plot

%.o: %.c
	@echo COMPILING CC $<
	@${CC} -g -c ${CFLAGS} $< -o $@

# some features of plot should be off during testing
# e.g. output of `plot GC stats` during plot_cleanup
%.to: %.c
	@echo COMPILING CC $<
	@${CC} -g -c ${CFLAGS} $< -DPLOT_TESTS -o $@

plot: ${OBJ}
	@echo more compiling CC -o $@
	@${CC} src/main.c -o $@ ${LDFLAGS} ${OBJ}
	@make -s cleanobj

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
	@echo "\nrunning hard.scm"
	./plot t/hard.scm

# harsh version for checking sanity of test code
compile_tests: clean ${TOBJ}
	@echo test_parse CC -o tests/test_llist.c
	@# pthread, rt and m are all needed by certain versions of libcheck 
	@${CC} -g -o run_tests t/test_main.c ${TOBJ} ${TEST_CFLAGS} -lpthread -lrt -lm -lcheck

# run tests
run_tests: compile_tests plot
	@echo running test_llist
	./run_tests
	@echo "\nrunning simple.scm"
	./plot t/simple.scm
	@echo "\nrunning hard.scm"
	./plot t/hard.scm
	@make -s cleanobj

test: run_tests cleanobj

simple: plot
	cat t/simple.scm
	@echo "\n"
	./plot t/simple.scm

hard: plot
	cat t/hard.scm
	@echo "\n"
	./plot t/hard.scm

cleanobj:
	@echo cleaning objects
	@rm -f src/*.o src/*.to t/*.o src/*.su

foo: cleanobj

clean: cleanobj
	@echo cleaning executable
	@rm -f plot run_tests

.PHONY: all clean cleanobj test tests simple hard clang test_soft tests_soft

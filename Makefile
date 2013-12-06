.POSIX:

include config.mk

SRC = src/read.c src/parse.c src/eval.c src/hash.c src/env.c src/funcs.c src/plot.c src/character.c src/string.c src/number.c src/pair.c src/value.c src/forms.c src/control.c src/input.c src/ports.c
OBJ = ${SRC:.c=.o}

EXTRAFLAGS =

# default to including debug output
all: debug

%.o: %.c
	@echo COMPILING CC $<
	@${CC} -g -c ${CFLAGS} $< ${EXTRAFLAGS} -o $@

plot: src/bindings.h ${OBJ}
	@echo more compiling CC -o $@
	@${CC} src/main.c -o $@ ${LDFLAGS} -DPLOT_DEBUG ${OBJ}
	@make -s cleanobj

# build plot with debug output
debug:
	@make -s EXTRAFLAGS="-DPLOT_DEBUG" plot

# make with clang to produce different errors, includes testing code
clang: src/bindings.h
	clang ${SRC} t/test_main.c -lcheck -Wall -Wextra

compile_tests: clean src/bindings.h ${OBJ}
	@echo test_parse CC -o tests/test_llist.c
	@# pthread, rt and m are all needed by certain versions of libcheck 
	@${CC} -g -o run_tests t/test_main.c ${OBJ} ${TEST_CFLAGS} -lpthread -lrt -lm -lcheck

# run tests
run_tests: compile_tests plot
	# FIXME temporarily disabled
	@echo running c unit tests
	./run_tests
	@echo "\nrunning example.scm"
	./plot t/example.scm
	@echo "\nrunning basic.scm"
	./plot t/integration/basic.scm
	@echo "\nrunning character.scm"
	./plot t/integration/character.scm
	@echo "\nrunning string.scm"
	./plot t/integration/string.scm
	@echo "\nrunning pair.scm"
	./plot t/integration/pair.scm
	@echo "\nrunning quote.scm"
	./plot t/integration/quote.scm
	@echo "\nrunning control.scm"
	./plot t/integration/control.scm
	@echo "\nrunning define.scm"
	./plot t/integration/define.scm
	@echo "\nrunning input.scm"
	./plot t/integration/input.scm
	@make -s cleanobj

test: run_tests cleanobj

example: plot
	cat t/example.scm
	@echo "\n"
	./plot t/example.scm

integration: plot
	cat t/integration.scm
	@echo "\n"
	./plot t/integration.scm

cleanobj:
	@echo cleaning objects
	@rm -f src/*.o src/*.to t/*.o src/*.su

foo: cleanobj

clean: cleanobj
	@echo cleaning executable
	@rm -f plot run_tests
	@echo cleaning auto-generated files
	@rm -f src/bindings.h

# generate plot bindings
src/bindings.h:
	./build/generate_bindings.pl

.PHONY: all clean cleanobj test tests example integration clang test_soft tests_soft debug

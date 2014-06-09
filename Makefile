.POSIX:

include config.mk

SRC = src/read.c src/parse.c src/eval.c src/hash.c src/env.c src/funcs.c src/plot.c src/character.c src/string.c src/number.c src/pair.c src/value.c src/forms.c src/control.c src/input.c src/ports.c src/output.c
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
	@${CC} -g -o run_tests t/component/test_main.c ${OBJ} ${TEST_CFLAGS} -lpthread -lrt -lm -lcheck

# run tests
run_tests: compile_tests plot
	@echo running c unit tests
	./run_tests
	@echo "\nrunning example.scm"
	./plot t/examples/example.scm
	@echo "\nrunning basic.scm"
	./plot t/integration/basic.scm
	@echo "\nrunning boolean.scm"
	./plot t/integration/boolean.scm
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

	@rm -f test
	@echo "\nrunning output.scm"
	./plot t/integration/output.scm
	cat test

	@echo ""
	@make -s cleanobj

run_library_tests: compile_tests plot
	@echo "\nrunning library.scm (failure expected as implementation is incomplete)"
	./plot t/integration/library.scm

	@rm -f test
	@echo "\nrunning output.scm"
	./plot t/integration/output.scm
	cat test

	@echo ""
	@make -s cleanobj

test: run_tests cleanobj

test_libs: run_library_tests cleanobj

example: plot
	cat t/examples/example.scm
	@echo "\n"
	./plot t/examples/example.scm

integration: plot
	cat t/integration.scm
	@echo "\n"
	./plot t/integration.scm

cleanobj:
	@echo cleaning objects and temporary files
	@rm -f src/*.o src/*.to t/*.o t/component/*.o src/*.su test

foo: cleanobj

clean: cleanobj
	@echo cleaning executables
	@rm -f plot run_tests hasher
	@echo cleaning auto-generated files
	@rm -f src/bindings.h

# generate plot bindings
src/bindings.h:
	./build/generate_bindings.pl

# generate compliance report
compliance: src/bindings.h
	./build/compliance.pl

# helper program to generate hashes
hasher: src/bindings.h ${OBJ}
	@echo compiling hasher helper script
	@${CC} scripts/hasher.c -o hasher ${LDFLAGS} -DPLOT_DEBUG ${OBJ}

.PHONY: all clean cleanobj test tests example integration clang test_soft tests_soft debug compliance hasher run_library_tests test_libs


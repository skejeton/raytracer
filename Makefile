SOURCES := $(shell find src -name '*.c') $(shell find deps -name '*.c')
all:
	if [ ! -d "./bin" ]; then mkdir ./bin; fi
	ccache clang -Ofast -march=native -Wconversion -Wsign-conversion -Wextra -Wall -Werror -pedantic -lm -lc -lpthread $(SOURCES) -o ./bin/output
# 	rm *.o

tests:
	if [ ! -d "./bin" ]; then mkdir ./bin; fi
	gcc -g -fsanitize=address -fsanitize=undefined -lc -lm ./deps/enty/**.c ./deps/vec/**.c ./deps/slab/**.c ./test/lib/test.c ./test/runner.c -o ./bin/test
	./bin/test

run: all
	./bin/output

clean:
	# if [ -d "./bin" ]; then rm ./bin/*; fi
.PHONY: clean, test, wasm, run, all

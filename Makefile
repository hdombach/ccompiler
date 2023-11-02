#template from https://gist.github.com/tomdaley92/190c68e8a84038cc91a5459409e007df

CFLAGS = -Werror
ifdef DEBUG
	CFLAGS += -g
	CFLAGS += -D DEBUG=1
endif

DEPFLAGS = -MMD -MP
SRC = src
OBJECTS := $(patsubst $(SRC)/%.c, build/%.o, $(wildcard $(SRC)/*.c))
DEPENDS := $(OBJECTS:.o=.d)
COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) -c -o $@
COMPILE_EXE = $(CC) $(CFLAGS) $(filter %.o,$^) -lm



DEPS_GEN = build/argParser.o build/token.o build/tokenizer.o build/preprocessor.o\
					 $(patsubst $(SRC)/ast/%.c, build/ast/%.o, $(wildcard $(SRC)/ast/*.c))\
					 $(patsubst $(SRC)/util/%.c, build/util/%.o, $(wildcard $(SRC)/util/*.c))\
					 $(patsubst $(SRC)/sem/%.c, build/sem/%.o, $(wildcard $(SRC)/sem/*.c))\


DEPS_CCOMPILER = build/main.o $(DEPS_GEN)

DEPS_ALL_TESTS = $(DEPS_GEN)\
								 $(patsubst $(SRC)/tests/%.c, build/tests/%.o, $(wildcard $(SRC)/tests/*.c))

DEPS_MACRO_DICT_TEST = $(DEPS_GEN) build/tests/macroDictTest.o build/tests/test.o


all: build/ccompiler build/allTests

build/ccompiler: build $(DEPS_CCOMPILER)
	$(COMPILE_EXE) -o build/ccompiler

build/allTests: build $(DEPS_ALL_TESTS)
	$(COMPILE_EXE) -o build/allTests

build/%.o: $(SRC)/%.c
	$(COMPILE.c) $<

build:
	mkdir -p build 
	mkdir -p build/util
	mkdir -p build/tests
	mkdir -p build/ast
	mkdir -p build/sem

clean:
	rm -r build

-include $(DEPENDS)

#assets
assets: assets/exampleLines.txt testAssets
	-mkdir build/assets
	-cp assets/exampleLines.txt build/assets/exampleLines.txt

testAssets: assets/tests/backslash1.txt assets/tests/backslashResult1.txt\
	assets/tests/tokens.txt
	-mkdir build/assets
	-mkdir build/assets/tests
	-cp assets/tests/backslash1.txt build/assets/tests/backslash1.txt
	-cp assets/tests/backslash1.txt build/assets/tests/backslash1.txt
	-cp assets/tests/tokens.txt build/assets/tests/tokens.txt

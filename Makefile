#template from https://gist.github.com/tomdaley92/190c68e8a84038cc91a5459409e007df

CFLAGS = -g
DEPFLAGS = -MMD -MP
SRC = src
OBJECTS := $(patsubst $(SRC)/%.c, build/%.o, $(wildcard $(SRC)/*.c))
DEPENDS := $(OBJECTS:.o=.d)
COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) -c -o $@
COMPILE_EXE = $(CC) $(CFLAGS) $(filter %.o,$^)


DEPS_TOKENIZER = build/util/dlist.o build/token.o build/tokenizer.o

DEPS_CCOMPILER = build/main.o build/argParser.o $(DEPS_TOKENIZER)\
								 build/util/dlist.o build/util/dlist.o build/ast/macroDef.o\
								 build/ast/tokenParser.o build/util/util.o

DEPS_MACRO_DICT = build/util/dlist.o build/util/hash.o build/util/macroDict.o

DEPS_ALL_TESTS = build/util/dlist.o $(DEPS_MACRO_DICT)\
								 $(patsubst $(SRC)/tests/%.c, build/tests/%.o, $(wildcard $(SRC)/tests/*.c))

DEPS_MACRO_DICT_TEST = $(DEPS_MACRO_DICT) build/tests/macroDictTest.o build/tests/test.o


all: build/ccompiler assets testAssets build/allTests build/macroDictTest

build/ccompiler: build $(DEPS_CCOMPILER)
	$(COMPILE_EXE) -o build/ccompiler

build/allTests: build $(DEPS_ALL_TESTS)
	$(COMPILE_EXE) -o build/allTests

build/macroDictTest: build $(DEPS_MACRO_DICT_TEST)
	$(COMPILE_EXE) -o build/macroDictTest  -Wl,-e,_macroDictPlayground

build/%.o: $(SRC)/%.c
	$(COMPILE.c) $<

build:
	mkdir -p build 
	mkdir -p build/util
	mkdir -p build/tests
	mkdir -p build/ast

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

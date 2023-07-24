CFLAGS=-g

all: assets build/allTests\
	build/ccompiler build/macroDictTest

build/ccompiler: src/main.c build/argParser.o build/dlist.o build/tokenizer.o\
	build/token.o
	cc src/main.c build/argParser.o build/dlist.o build/tokenizer.o \
		build/token.o -o build/ccompiler $(CFLAGS)

#modules
build/dlist.o: src/util/dlist.c
	cc src/util/dlist.c -c -o build/dlist.o $(CFLAGS)

build/argParser.o: src/argParser.c build/dlist.o
	cc src/argParser.c -c -o build/argParser.o $(CFLAGS)

build/tokenizer.o: src/tokenizer.c build/dlist.o src/util/dstr.h
	cc src/tokenizer.c build/dlist.o -c -o build/tokenizer.o $(CFLAGS)

build/token.o: src/token.c src/util/dstr.h
	cc src/token.c -c -o build/token.o $(CFLAGS)

build/macroDict.o: src/util/macroDict.c build/dlist.o src/util/dstr.h\
	build/hash.o
	cc src/util/macroDict.c build/hash.o -c\
		-o build/macroDict.o $(CFLAGS)

build/hash.o: src/util/hash.c src/util/hash.h
	cc src/util/hash.c -c -o build/hash.o $(CFLAGS)

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


#testing
build/test.o: src/tests/test.c
	cc src/tests/test.c -c -o build/test.o $(CFLAGS)

#		-Wl,-e,_stagesTestMain -o build/stagesTest $(CFLAGS)

build/dlistTest.o: src/tests/dlistTest.c build/dlist.o
	cc src/tests/dlistTest.c -c -o build/dlistTest.o $(CFLAGS)

build/dstrTest.o: src/tests/dstrTest.c build/test.o build/dlist.o
	cc src/tests/dstrTest.c build/dlist.o -c -o build/dstrTest.o $(CFLAGS)

build/macroDictTest.o: src/tests/macroDictTest.c build/macroDict.o
	cc src/tests/macroDictTest.c build/macroDict.o -c -o build/macroDictTest.o $(CFLAGS)

build/macroDictTest: build/macroDictTest.o build/hash.o build/test.o
	cc build/macroDictTest.o build/macroDict.o build/hash.o $(CFLAGS)\
		-Wl,-e,_macroDictPlayground build/test.o -o build/macroDictTest
build/allTests: src/tests/allTests.c build/test.o\
	 build/dlistTest.o build/dlist.o\
	build/dstrTest.o build/macroDictTest.o build/test.o
	cc src/tests/allTests.c build/test.o\
		build/dlistTest.o build/dlist.o\
		build/dstrTest.o build/macroDictTest.o build/macroDict.o build/hash.o\
		\
		-o build/allTests $(CFLAGS)

CFLAGS=-g

all: build/linesTest build/stagesTest assets build/allTests build/preprocessor\
	build/ccompiler

build/ccompiler: src/main.c build/argParser.o build/dlist.o build/tokenizer.o\
	build/token.o
	cc src/main.c build/argParser.o build/dlist.o build/tokenizer.o \
		build/token.o -o build/ccompiler $(CFLAGS)

#modules
build/dlist.o: src/util/dlist.c
	cc src/util/dlist.c -c -o build/dlist.o $(CFLAGS)

build/argParser.o: src/argParser.c build/dlist.o
	cc src/argParser.c build/dlist.o -c -o build/argParser.o $(CFLAGS)

build/tokenizer.o: src/tokenizer.c build/dlist.o src/util/dstr.h
	cc src/tokenizer.c build/dlist.o -c -o build/tokenizer.o $(CFLAGS)

build/token.o: src/token.c src/util/dstr.h
	cc src/token.c -c -o build/token.o $(CFLAGS)

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

build/linesTest: build/linesTest.o build/lines.o build/test.o
	cc build/linesTest.o build/lines.o build/test.o\
		-Wl,-e,_linesTestMain -o build/linesTest $(CFLAGS)

build/stagesTest: build/stagesTest.o build/lines.o build/test.o build/stages.o
	cc build/stagesTest.o build/lines.o build/test.o build/stages.o\
		-Wl,-e,_stagesTestMain -o build/stagesTest $(CFLAGS)

build/dlistTest.o: src/tests/dlistTest.c
	cc src/tests/dlistTest.c -c -o build/dlistTest.o $(CFLAGS)

build/dstrTest.o: src/tests/dstrTest.c build/test.o build/dlist.o
	cc src/tests/dstrTest.c build/test.o build/dlist.o -c -o build/dstrTest.o $(CFLAGS)


build/allTests: src/tests/allTests.c build/linesTest.o build/test.o\
	build/lines.o build/stagesTest.o build/stages.o build/dlistTest.o build/dlist.o\
	build/dstrTest.o
	cc src/tests/allTests.c build/linesTest.o build/test.o build/lines.o\
		build/stagesTest.o build/stages.o build/dlistTest.o build/dlist.o\
		build/dstrTest.o\
		-o build/allTests $(CFLAGS)

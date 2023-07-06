CFLAGS=-g

all: build/linesTest build/stagesTest assets build/allTests build/preprocessor

#preprocessor
build/preprocessor: src/preprocessor/ppMain.c build/ppArgParser.o build/lines.o
	cc src/preprocessor/ppMain.c build/ppArgParser.o build/lines.o -o build/preprocessor $(CFLAGS)

#modules
build/lines.o: src/preprocessor/lines.c
	cc src/preprocessor/lines.c -c -o build/lines.o  $(CFLAGS)

build/ppArgParser.o: src/preprocessor/ppArgParser.c
	cc src/preprocessor/ppArgParser.c -c -o build/ppArgParser.o $(CFLAGS)

build/stages.o: src/preprocessor/stages.c
	cc src/preprocessor/stages.c -c -o build/stages.o $(CFLAGS)

#assets
assets: assets/exampleLines.txt testAssets
	-mkdir build/assets
	-cp assets/exampleLines.txt build/assets/exampleLines.txt

testAssets: assets/tests/backslash1.txt assets/tests/backslashResult1.txt
	-mkdir build/assets
	-mkdir build/assets/tests
	-cp assets/tests/backslash1.txt build/assets/tests/backslash1.txt
	-cp assets/tests/backslash1.txt build/assets/tests/backslash1.txt

#testing
build/tests.o: src/util/tests.c
	cc src/util/tests.c -c -o build/tests.o $(CFLAGS)

build/linesTest.o: src/preprocessor/linesTest.c
	cc src/preprocessor/linesTest.c -c -o build/linesTest.o $(CFLAGS)

build/linesTest: build/linesTest.o build/lines.o build/tests.o
	cc build/linesTest.o build/lines.o build/tests.o\
		-Wl,-e,_linesTestMain -o build/linesTest $(CFLAGS)

build/stagesTest.o: src/preprocessor/stagesTest.c
	cc src/preprocessor/stagesTest.c -c -o build/stagesTest.o $(CFLAGS)

build/stagesTest: build/stagesTest.o build/lines.o build/tests.o build/stages.o
	cc build/stagesTest.o build/lines.o build/tests.o build/stages.o\
		-Wl,-e,_stagesTestMain -o build/stagesTest $(CFLAGS)

build/allTests: src/util/allTests.c build/linesTest.o build/tests.o build/lines.o build/stagesTest.o build/stages.o
	cc src/util/allTests.c build/linesTest.o build/tests.o build/lines.o\
		build/stagesTest.o build/stages.o -o build/allTests $(CFLAGS)

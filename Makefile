CFLAGS=-g

all: linesTest stagesTest assets allTests preprocessor

#preprocessor
preprocessor: src/preprocessor/ppMain.c ppArgParser.o lines.o
	cc src/preprocessor/ppMain.c build/ppArgParser.o build/lines.o -o build/preprocessor $(CFLAGS)

#modules
lines.o: src/preprocessor/lines.c
	cc src/preprocessor/lines.c -c -o build/lines.o  $(CFLAGS)

ppArgParser.o: src/preprocessor/ppArgParser.c
	cc src/preprocessor/ppArgParser.c -c -o build/ppArgParser.o $(CFLAGS)

stages.o: src/preprocessor/stages.c
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
tests.o: src/shared/tests.c
	cc src/shared/tests.c -c -o build/tests.o $(CFLAGS)

linesTest.o: src/preprocessor/linesTest.c
	cc src/preprocessor/linesTest.c -c -o build/linesTest.o $(CFLAGS)

linesTest: linesTest.o lines.o tests.o
	cc build/linesTest.o build/lines.o build/tests.o\
		-Wl,-e,_linesTestMain -o build/linesTest $(CFLAGS)

stagesTest.o: src/preprocessor/stagesTest.c
	cc src/preprocessor/stagesTest.c -c -o build/stagesTest.o $(CFLAGS)

stagesTest: stagesTest.o lines.o tests.o stages.o
	cc build/stagesTest.o build/lines.o build/tests.o build/stages.o\
		-Wl,-e,_stagesTestMain -o build/stagesTest $(CFLAGS)

allTests: src/shared/allTests.c linesTest.o tests.o lines.o stagesTest.o stages.o
	cc src/shared/allTests.c build/linesTest.o build/tests.o build/lines.o\
		build/stagesTest.o build/stages.o -o build/allTests $(CFLAGS)

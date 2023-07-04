CFLAGS=-g

all: linesTest assets allTests preprocessor

#preprocessor
preprocessor: src/preprocessor/ppMain.c ppArgParser.o lines.o
	cc src/preprocessor/ppMain.c build/ppArgParser.o build/lines.o -o build/preprocessor $(CFLAGS)

#modules
lines.o: src/preprocessor/lines.c
	cc src/preprocessor/lines.c -c -o build/lines.o  $(CFLAGS)

ppArgParser.o: src/preprocessor/ppArgParser.c
	cc src/preprocessor/ppArgParser.c -c -o build/ppArgParser.o $(CFLAGS)

stages.o: src/preprocessor/stages.c
	cc src/preprocessor/stages.c -c -o build/stages.c $(CFLAGS)

#assets
assets: assets/exampleLines.txt
	mkdir build/assets
	cp assets/exampleLines.txt build/assets/exampleLines.txt

#testing
tests.o: src/shared/tests.c
	cc src/shared/tests.c -c -o build/tests.o $(CFLAGS)

linesTest.o: src/preprocessor/linesTest.c
	cc src/preprocessor/linesTest.c -c -o build/linesTest.o $(CFLAGS)

linesTest: linesTest.o lines.o tests.o
	cc build/linesTest.o build/lines.o build/tests.o -Wl,-e,_linesTestMain -o build/linesTest $(CFLAGS)

allTests: src/shared/allTests.c linesTest.o tests.o lines.o
	cc src/shared/allTests.c build/linesTest.o build/tests.o build/lines.o -o build/allTests $(CFLAGS)

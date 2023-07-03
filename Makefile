CFLAGS=-g

all: linesTest assets

tests.o: preprocessor/tests.c
	cc preprocessor/tests.c -c -o build/tests.o $(CFLAGS)

linesTest: preprocessor/linesTest.c tests.o lines.o
	cc preprocessor/linesTest.c build/tests.o build/lines.o -o build/linesTest $(CFLAGS)

lines.o: preprocessor/lines.c
	cc preprocessor/lines.c -c -o build/lines.o  $(CFLAGS)

assets: assets/exampleLines.txt
	mkdir build/assets
	cp assets/exampleLines.txt build/assets/exampleLines.txt

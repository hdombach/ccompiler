#include <stdio.h>

#include "stagesTest.h"
#include "lines.h"
#include "stages.h"
#include "../tests/test.h"

void stagesTestBackslash() {
	FILE *exampleFile;
	FILE *exampleResultFile;
	Lines lines;
	Lines resultLines;
	const char *EXAMPLE_FILE_NAME = "assets/tests/backslash1.txt";
	const char *EXAMPLE_RESULT_FILE_NAME = "assets/tests/backslashResult1.txt";

	tStartSection("Preprocessor backslash");

	exampleFile = fopen(EXAMPLE_FILE_NAME, "r");
	if (!exampleFile) {
		tAssert("Opening backslash1.txt", 0);
		perror("Cannot open backslash1.txt");
		return;
	}

	exampleResultFile = fopen(EXAMPLE_RESULT_FILE_NAME, "r");
	if (!exampleResultFile) {
		tAssert("Opening exampleResultFile.txt", 0);
		perror("Cannot open exampleResultFile.txt");
		return;
	}

	initLinesFromFile(&lines, exampleFile);
	initLinesFromFile(&resultLines, exampleResultFile);

	stagesBackslashes(&lines);

	tAssert("compare result", linesCmp(&lines, &resultLines));

	freeLines(&lines);
	freeLines(&resultLines);
}

void stagesTest() {
	stagesTestBackslash();
}

int stagesTestMain() {
	tInit();
	stagesTestMain();
	tPrintResult();

	return 0;
}

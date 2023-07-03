#include <stdio.h>
#include <string.h>

#include "../shared/tests.h"
#include "lines.h"
#include "linesTest.h"

void linesTestEmpty() {
	Lines lines;

	tStartSection("Empty List");
	lines = initLines();
	tAssert("lines size", lines.size == 0);

	freeLines(&lines);
}

void linesTestAdding() {
	Lines lines;
	const char *LINE_1 = "This is line 1";
	const char *LINE_2 = "This is line 2";
	const char *LINE_3 = "This is line 3";

	tStartSection("Adding Lines");
	lines = initLines();
	linesApp(&lines, LINE_1);
	linesApp(&lines, LINE_2);
	linesApp(&lines, LINE_3);

	tAssert("compare line 1", strcmp(LINE_1, lines.data[0]) == 0);
	tAssert("compare line 2", strcmp(LINE_2, lines.data[1]) == 0);
	tAssert("compare line 3", strcmp(LINE_3, lines.data[2]) == 0);

	freeLines(&lines);
}

void linesTestFromFile() {
	Lines lines;
	FILE *file;
	char *currentLine;
	size_t lineCap;
	const char *EXAMPLE_FILE_NAME = "assets/exampleLines.txt";
	const char *EXAMPLE_LINE_1 = "This is line 1\n";
	const char *EXAMPLE_LINE_2 = "This is line 2\n";
	const char *EXAMPLE_LINE_3 = "This is going to be line 3\n";

	tStartSection("Loading lines from file");
	lines = initLines();
	currentLine = NULL;
	file = fopen(EXAMPLE_FILE_NAME, "r");
	if (!file) {
		tAssert("Opening exampleLines.txt", 0);
		perror("Cannot open exampleLines.txt");
		return;
	}

	while (getline(&currentLine, &lineCap, file) > 0) {
		linesApp(&lines, currentLine);
	}

	tAssert("compare line 1", strcmp(EXAMPLE_LINE_1, lines.data[0]) == 0);
	tAssert("compare line 2", strcmp(EXAMPLE_LINE_2, lines.data[1]) == 0);
	tAssert("compare line 3", strcmp(EXAMPLE_LINE_3, lines.data[2]) == 0);


	freeLines(&lines);
}

void linesTestRemove() {
	Lines lines;
	const char *LINES_1 = "This is line 1";
	const char *LINES_2 = "This is line 2";
	const char *LINES_3 = "This is line 3";
	const char *LINES_4 = "This is line 4";
	
	tStartSection("Remove lines");
	lines = initLines();
	linesApp(&lines, LINES_1);
	linesApp(&lines, LINES_2);
	linesApp(&lines, LINES_3);
	linesApp(&lines, LINES_4);
	tAssert("check size 1", lines.size == 4);

	
	tAssert("invalid index", linesRem(&lines, -1) == LINES_INVALID_INDEX);
	tAssert("invalid index 2", linesRem(&lines, 4) == LINES_INVALID_INDEX);

	tAssert("remove 1", linesRem(&lines, 1) == LINES_SUCCESS);
	tAssert("check 1", strcmp(lines.data[0], LINES_1) == 0);
	tAssert("check 2", strcmp(lines.data[1], LINES_3) == 0);
	tAssert("check 3", strcmp(lines.data[2], LINES_4) == 0);
	tAssert("check size 2", lines.size == 3);

	tAssert("remove 2", linesRem(&lines, 2) == LINES_SUCCESS);
	tAssert("check 4", strcmp(lines.data[0], LINES_1) == 0);
	tAssert("check 5", strcmp(lines.data[1], LINES_3) == 0);
	tAssert("check size 3", lines.size == 2);

	tAssert("remove 3", linesRem(&lines, 0) == LINES_SUCCESS);
	tAssert("check 5", strcmp(lines.data[0], LINES_3) == 0);
	tAssert("check size 4", lines.size == 1);

	tAssert("remove 4", linesRem(&lines, 0) == LINES_SUCCESS);
	tAssert("check size 5", lines.size == 0);

	tAssert("invalid index 3", linesRem(&lines, 0) == LINES_INVALID_INDEX);
}

void linesTestInsert() {
	Lines lines;
	const char *LINES_1 = "This is line 1";
	const char *LINES_2 = "This is line 2";
	const char *LINES_3 = "This is line 3";
	const char *LINES_4 = "This is line 4";


	tStartSection("Inserting Lines");
	lines = initLines();
	tAssert("invalid index 1", linesIns(&lines, LINES_1, -1) == LINES_INVALID_INDEX);
	tAssert("invalid index 1", linesIns(&lines, LINES_1, 1) == LINES_INVALID_INDEX);
	tAssert("insert 1", linesIns(&lines, LINES_3, 0) == LINES_SUCCESS); //3
	tAssert("insert 2", linesIns(&lines, LINES_1, 0) == LINES_SUCCESS); //1, 3
	tAssert("insert 3", linesIns(&lines, LINES_4, 2) == LINES_SUCCESS); //1, 3, 4
	tAssert("insert 4", linesIns(&lines, LINES_2, 1) == LINES_SUCCESS); //1, 2, 3, 4

	tAssert("check 1", strcmp(LINES_1, lines.data[0]) == 0);
	tAssert("check 2", strcmp(LINES_2, lines.data[1]) == 0);
	tAssert("check 3", strcmp(LINES_3, lines.data[2]) == 0);
	tAssert("check 4", strcmp(LINES_4, lines.data[3]) == 0);

}

void linesTest() {
	linesTestEmpty();
	linesTestAdding();
	linesTestFromFile();
	linesTestRemove();
	linesTestInsert();
}

int linesTestMain() {
	tInit();
	linesTest();
	tPrintResult();

	return 0;
}

#include "test.h"
#include <stdio.h>
#include "../util/log.h"

int testsTotal;
int testsPassed;
char *curSectionName;

void tInit() {
	testsTotal = 0;
	testsPassed = 0;
	curSectionName = "";
}

void tAssert(
		const char *file,
		int line,
		char *name,
		int expression,
		const char *expressionStr)
{
	testsTotal++;
	if (expression) {
		testsPassed++;
	} else {
		logTestFailed(file, line, curSectionName, name, expressionStr);
	}
}

void tPrintResult() {
	printf("Tests passed: %d/%d\n", testsPassed, testsTotal);
}

void tStartSection(char *name) {
	logInfo("Testing Section: \"%s\"", name);
	curSectionName = name;
}

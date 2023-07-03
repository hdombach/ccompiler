#include "tests.h"
#include <stdio.h>

int testsTotal;
int testsPassed;
char *curSectionName;

void tInit() {
	testsTotal = 0;
	testsPassed = 0;
	curSectionName = "";
}

void tAssert(char *name, int expression) {
	testsTotal++;
	if (expression) {
		testsPassed++;
	} else {
		fprintf(stderr, "TEST FAILED! %s: %s\n", curSectionName, name);
	}
}

void tPrintResult() {
	printf("Tests passed: %d/%d\n", testsPassed, testsTotal);
}

void tStartSection(char *name) {
	curSectionName = name;
}

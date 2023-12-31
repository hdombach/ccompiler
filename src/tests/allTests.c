#include "dstrTest.h"
#include <stdio.h>
#define TESTS_MAIN

#include "test.h"
#include "dlistTest.h"
#include "dstrTest.h"
#include "macroDictTest.h"
#include "wordDictTest.h"
#include "wordListTest.h"
#include "tokenizerTest.h"

int main() {
	tInit();

	dlistTest();
	dstrTest();
	macroDictTest();
	wordDictTest();
	wordListTest();
	tokenizerTest();

	tPrintResult();

	if (testsPassed == testsTotal) {
		return 0;
	} else {
		return 1;
	}
}

#include "dstrTest.h"
#include "typeGenTest.h"
#include <stdio.h>
#define TESTS_MAIN

#include "test.h"
#include "dlistTest.h"
#include "dstrTest.h"
#include "macroDictTest.h"
#include "wordDictTest.h"
#include "wordListTest.h"
#include "tokenizerTest.h"
#include "astTest.h"
#include "testArgs.h"

int main(int argc, char **argv) {
	int result;

	parseTestArgs(argc, argv, &gTestArgs);
	if (gTestArgs.help) {
		printf("%s", TEST_ARGS_HELP_MSG);
		result = 0;
		goto done;
	}

	if (gTestArgs.verbose) {
		gLogLevel = LL_DEBUG | LL_INFO | LL_WARNING | LL_INT_ERROR | LL_CERROR;
	} else {
		gLogLevel = LL_INT_ERROR;
	}

	tInit();

	dlistTest();
	dstrTest();
	macroDictTest();
	wordDictTest();
	wordListTest();
	tokenizerTest();
	astTests();
	typeGenTests();

	tPrintResult();

	if (testsPassed == testsTotal) {
		result = 0;
	} else {
		result = 1;
	}

done:
	freeTestArgs(&gTestArgs);
	return result;
}

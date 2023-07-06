#include <stdio.h>
#define TESTS_MAIN

#include "tests.h"
#include "../preprocessor/linesTest.h"
#include "../preprocessor/stagesTest.h"

int main() {
	tInit();

	linesTest();
	stagesTest();

	tPrintResult();
}

#include "dstrTest.h"
#include <stdio.h>
#define TESTS_MAIN

#include "test.h"
#include "../preprocessor/linesTest.h"
#include "../preprocessor/stagesTest.h"
#include "dlistTest.h"
#include "dstrTest.h"

int main() {
	tInit();

	linesTest();
	stagesTest();
	dlistTest();
	dstrTest();

	tPrintResult();
}

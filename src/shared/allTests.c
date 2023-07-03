#define TESTS_MAIN

#include "tests.h"
#include "../preprocessor/linesTest.h"

int main() {
	tInit();

	linesTest();

	tPrintResult();
}

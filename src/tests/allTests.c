#include "dstrTest.h"
#include <stdio.h>
#define TESTS_MAIN

#include "test.h"
#include "dlistTest.h"
#include "dstrTest.h"
#include "macroDictTest.h"

int main() {
	tInit();

	dlistTest();
	dstrTest();
	macroDictTest();

	tPrintResult();
}

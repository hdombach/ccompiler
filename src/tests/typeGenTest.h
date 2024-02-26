#pragma once

#include <stdlib.h>

#include "tools.h"

/**
 * @file
 *
 * @brief Tests type gen
 */

/**
 * @brief Runs all the type gen tests
 */
#include "test.h"
static void typeGenTests();

static void typeGenLabels() {
	tStartSection("Test Type Gen Labels");

	tTypeGenLabels(
			"int main() {\n"
			"test_label: say_hi();"
			"goto test_label;"
			"}\n",
			(const char *[]) {"test_label", NULL});

	tTypeGenLabels(
			"int main() {\n"
			"test_label: test_label2: say_hi();"
			"goto test_label;"
			"}\n",
			(const char *[]) {"test_label", "test_label2", NULL});

	tTypeGenFailed(
			"int main() {\n"
			"test_label: test_label: say_hi();"
			"goto test_label;"
			"}\n",
			(CError[]) {CERR_LBL, CERR_UNKNOWN});

	tTypeGenFailed(
			"int main() {\n"
			"test_label: say_hi();"
			"goto invalid_label;"
			"}\n",
			(CError[]) {CERR_LBL, CERR_UNKNOWN});

	tTypeGenFailed(
			"int main() {\n"
			"test_label: say_hi();"
			"goto main;"
			"}\n",
			(CError[]) {CERR_LBL, CERR_UNKNOWN});
}

static void typeGenTypes() {
	tStartSection("Test type gen types");

	tTypeGenSuccess(
			"int num;"
			"int *ptr1;"
			"int *ptr2;",
			(STypeT[]) {STT_PRIM, STT_POINTER, STT_PRIM, STT_POINTER, STT_PRIM, STT_UNKNOWN});

	tTypeGenFailed(
			"int test;\n"
			"int test;",
			(CError[]) {CERR_TYPE, CERR_TYPE, CERR_UNKNOWN});

	tTypeGenFailed(
			"int *test;\n"
			"int *test;",
			(CError[]) {CERR_TYPE, CERR_TYPE, CERR_UNKNOWN});

	tTypeGenFailed(
			"int *test;\n"
			"int test;",
			(CError[]) {CERR_TYPE, CERR_TYPE, CERR_UNKNOWN});

	tTypeGenSuccess(
			"int test;"
			"extern int test;"
			"int *testPtr;",
			(STypeT[]) {STT_PRIM, STT_POINTER, STT_PRIM, STT_UNKNOWN});
}

static void typeGenTests() {
	typeGenLabels();
	typeGenTypes();
}

#include <stdlib.h>
#include <string.h>

#include "dstrTest.h"
#include "../util/dstr.h"
#include "test.h"

void dstrTestEmpty() {
	DStr str;

	tStartSection("DStr test empty");
	initDStr(&str);
	T_ASSERT("Test size", str.size == 1);
	T_ASSERT("Test item 1", *dstrGet(&str, 0) == '\0');

	freeDStr(&str);
}

void dstrTestApp() {
	DStr str;

	tStartSection("DStr test append");
	initDStr(&str);
	T_ASSERT("check 1", strcmp(str.data, "") == 0);
	dstrApp(&str, 'H');
	T_ASSERT("check 2", strcmp(str.data, "H") == 0);
	dstrApp(&str, 'i');
	T_ASSERT("check 3", strcmp(str.data, "Hi") == 0);

	freeDStr(&str);
}

void dstrTestIns() {
	DStr str;

	tStartSection("DStr test insert");
	initDStr(&str);
	T_ASSERT("check 1", strcmp(str.data, "") == 0);
	dstrIns(&str, 'y', 0);
	T_ASSERT("check 2", strcmp(str.data, "y") == 0);
	dstrIns(&str, 'e', 1);
	T_ASSERT("check 2", strcmp(str.data, "ye") == 0);
	dstrIns(&str, 'B', 0);
	T_ASSERT("check 3", strcmp(str.data, "Bye") == 0);

	freeDStr(&str);
}

void dstrTestRem() {
	DStr str;

	tStartSection("DStr test rem");
	initDStr(&str);
	dstrApp(&str, 'H');
	dstrApp(&str, 'e');
	dstrApp(&str, 'l');
	dstrApp(&str, 'l');
	dstrApp(&str, 'o');

	T_ASSERT("check 1", strcmp(str.data, "Hello") == 0);
	dstrRem(&str, 1);
	T_ASSERT("check 2", strcmp(str.data, "Hllo") == 0);
	dstrRem(&str, 3);
	T_ASSERT("check 3", strcmp(str.data, "Hll") == 0);

	freeDStr(&str);
}

void dstrTestRemAll() {
	DStr str;

	tStartSection("DStr test rem all");
	initDStr(&str);
	dstrApp(&str, 'H');
	dstrApp(&str, 'e');
	dstrApp(&str, 'l');
	dstrApp(&str, 'l');
	dstrApp(&str, 'o');

	T_ASSERT("check 1", strcmp(str.data, "Hello") == 0);
	dstrRemAll(&str);
	T_ASSERT("check 2", strcmp(str.data, "") == 0);

	freeDStr(&str);
}

void dstrTest() {
	dstrTestEmpty();
	dstrTestApp();
	dstrTestIns();
	dstrTestRem();
	dstrTestRemAll();
}

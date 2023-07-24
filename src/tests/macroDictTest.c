#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macroDictTest.h"
#include "../util/macroDict.h"
#include "test.h"

void macroDictTestEmpty() {
	MacroDict macros;

	tStartSection("Macro Dict test empty");
	initMacroDict(&macros);

	tAssert("Test size", macros.elementCount == 0);

	freeMacroDict(&macros);
}

void macroDictTestInsert() {
	MacroDict macros;

	tStartSection("Macro Dict test insert");
	initMacroDict(&macros);

	tAssert("insert 1", 1 == macroDictInsert(&macros, strdup("key 1"), strdup("value 1")));
	tAssert("insert 2", 1 == macroDictInsert(&macros, strdup("key 2"), strdup("value 2")));
	tAssert("insert 3", 1 == macroDictInsert(&macros, strdup("key 3"), strdup("value 3")));
	tAssert("insert 4", 1 == macroDictInsert(&macros, strdup("key 4"), strdup("value 4")));
	tAssert("insert 5", 1 == macroDictInsert(&macros, strdup("key 5"), strdup("value 5")));
	tAssert("insert 6", 1 == macroDictInsert(&macros, strdup("key 6"), strdup("value 6")));
	tAssert("insert 7", 1 == macroDictInsert(&macros, strdup("key 7"), strdup("value 7")));
	tAssert("insert 8", 1 == macroDictInsert(&macros, strdup("key 8"), strdup("value 8")));
	tAssert("insert 9", 1 == macroDictInsert(&macros, strdup("key 9"), strdup("value 9")));
	tAssert("insert 10", 1 == macroDictInsert(&macros, strdup("key 10"), strdup("value 10")));
	tAssert("insert 11", 1 == macroDictInsert(&macros, strdup("key 11"), strdup("value 11")));
	tAssert("insert 12", 1 == macroDictInsert(&macros, strdup("key 12"), strdup("value 12")));
	tAssert("insert 13", 1 == macroDictInsert(&macros, strdup("key 13"), strdup("value 13")));
	tAssert("insert 14", 1 == macroDictInsert(&macros, strdup("key 14"), strdup("value 14")));
	tAssert("insert 15", 1 == macroDictInsert(&macros, strdup("key 15"), strdup("value 15")));
	tAssert("insert 16", 1 == macroDictInsert(&macros, strdup("key 16"), strdup("value 16")));

	tAssert("is present 1", !macroDictPresent(&macros, "key 17"));
	tAssert("insert 17", 1 == macroDictInsert(&macros, strdup("key 17"), strdup("value 17")));
	tAssert("is present 2", macroDictPresent(&macros, "key 17"));
	tAssert("insert 17 2", 0 == macroDictInsert(&macros, strdup("key 17"), strdup("value 17")));

	freeMacroDict(&macros);
}

void macroDictTestDelete() {
	MacroDict macros;

	tStartSection("Macro Dict test delete");
	initMacroDict(&macros);

	tAssert("insert 1", 1 == macroDictInsert(&macros, strdup("key 1"), strdup("value 1")));
	tAssert("insert 2", 1 == macroDictInsert(&macros, strdup("key 2"), strdup("value 2")));
	tAssert("insert 3", 1 == macroDictInsert(&macros, strdup("key 3"), strdup("value 3")));

	tAssert("present 1", 1 == macroDictPresent(&macros, "key 2"));
	macroDictDelete(&macros, "key 2");
	tAssert("present 2", 0 == macroDictPresent(&macros, "key 2"));
	tAssert("insert 2", 1 == macroDictInsert(&macros, strdup("key 2"), strdup("value 2")));
	tAssert("present 3", 1 == macroDictPresent(&macros, "key 2"));

	freeMacroDict(&macros);
}

void macroDictTestRemove() {
	MacroDict macros;
	char *tempStr;

	tStartSection("Macro Dict test remove");
	initMacroDict(&macros);

	tAssert("insert 1", 1 == macroDictInsert(&macros, strdup("key 1"), strdup("value 1")));
	tAssert("insert 2", 1 == macroDictInsert(&macros, strdup("key 2"), strdup("value 2")));
	tAssert("insert 3", 1 == macroDictInsert(&macros, strdup("key 3"), strdup("value 3")));

	tAssert("present 1", 1 == macroDictPresent(&macros, "key 2"));
	tempStr = macroDictRemove(&macros, "key 2");
	tAssert("present 2", 0 == macroDictPresent(&macros, "key 2"));
	tAssert("test value 1", 0 == strcmp("value 2", tempStr));
	free(tempStr);

	tAssert("present 3", 1 == macroDictPresent(&macros, "key 1"));
	tempStr = macroDictRemove(&macros, "key 1");
	tAssert("present 4", 0 == macroDictPresent(&macros, "key 1"));
	tAssert("test value 2", 0 == strcmp("value 1", tempStr));
	free(tempStr);


	freeMacroDict(&macros);
}

int macroDictPlayground(int argc, char **argv) {
	MacroDict macros;
	size_t lineSize;
	int lineRes;

	initMacroDict(&macros);

	macroDictInsert(&macros, strdup("1"), strdup("1"));
	macroDictInsert(&macros, strdup("10"), strdup("10"));

	while (1) {
		char *key, *value;

		key = NULL;
		value = NULL;

		lineRes = getline(&key, &lineSize, stdin);
		if (lineRes < 0) {
			free(key);
			break;
		}

		lineRes = getline(&value, &lineSize, stdin);
		if (lineRes < 0) {
			free(key);
			free(value);
			break;
		}

		key[strlen(key) - 1] = '\0';
		value[strlen(value) - 1] = '\0';

		macroDictInsert(&macros, key, value);
		printf("\n");
	}

	macroDictVPrint(&macros);

	freeMacroDict(&macros);
	return 0;
}

void macroDictTest() {
	macroDictTestEmpty();
	macroDictTestInsert();
	macroDictTestDelete();
	macroDictTestRemove();
}

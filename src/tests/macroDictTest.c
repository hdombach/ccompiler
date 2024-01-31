#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macroDictTest.h"
#include "../util/macroDict.h"
#include "test.h"
#include "../ast/macroDef.h"

void macroDictTestEmpty() {
	MacroDict macros;

	tStartSection("Macro Dict test empty");
	initMacroDict(&macros);

	T_ASSERT("Test size", macros.elementCount == 0);

	freeMacroDict(&macros);
}

void macroDictTestInsert() {
	MacroDict macros;
	ASTMacroDef macro;

	tStartSection("Macro Dict test insert");
	initMacroDict(&macros);

	initASTMacroDefn(&macro, strdup("value 1"));
	T_ASSERT("insert 1", 1 == macroDictInsert(&macros, strdup("key 1"), macro));

	initASTMacroDefn(&macro, strdup("value 2"));
	T_ASSERT("insert 2", 1 == macroDictInsert(&macros, strdup("key 2"), macro));

	initASTMacroDefn(&macro, strdup("value 3"));
	T_ASSERT("insert 3", 1 == macroDictInsert(&macros, strdup("key 3"), macro));

	initASTMacroDefn(&macro, strdup("value 4"));
	T_ASSERT("insert 4", 1 == macroDictInsert(&macros, strdup("key 4"), macro));

	initASTMacroDefn(&macro, strdup("value 5"));
	T_ASSERT("insert 5", 1 == macroDictInsert(&macros, strdup("key 5"), macro));

	initASTMacroDefn(&macro, strdup("value 6"));
	T_ASSERT("insert 6", 1 == macroDictInsert(&macros, strdup("key 6"), macro));

	initASTMacroDefn(&macro, strdup("value 7"));
	T_ASSERT("insert 7", 1 == macroDictInsert(&macros, strdup("key 7"), macro));

	initASTMacroDefn(&macro, strdup("value 8"));
	T_ASSERT("insert 8", 1 == macroDictInsert(&macros, strdup("key 8"), macro));

	initASTMacroDefn(&macro, strdup("value 9"));
	T_ASSERT("insert 9", 1 == macroDictInsert(&macros, strdup("key 9"), macro));

	initASTMacroDefn(&macro, strdup("value 10"));
	T_ASSERT("insert 10", 1 == macroDictInsert(&macros, strdup("key 10"), macro));

	initASTMacroDefn(&macro, strdup("value 11"));
	T_ASSERT("insert 11", 1 == macroDictInsert(&macros, strdup("key 11"), macro));

	initASTMacroDefn(&macro, strdup("value 12"));
	T_ASSERT("insert 12", 1 == macroDictInsert(&macros, strdup("key 12"), macro));

	initASTMacroDefn(&macro, strdup("value 13"));
	T_ASSERT("insert 13", 1 == macroDictInsert(&macros, strdup("key 13"), macro));

	initASTMacroDefn(&macro, strdup("value 14"));
	T_ASSERT("insert 14", 1 == macroDictInsert(&macros, strdup("key 14"), macro));

	initASTMacroDefn(&macro, strdup("value 15"));
	T_ASSERT("insert 15", 1 == macroDictInsert(&macros, strdup("key 15"), macro));

	initASTMacroDefn(&macro, strdup("value 16"));
	T_ASSERT("insert 16", 1 == macroDictInsert(&macros, strdup("key 16"), macro));


	T_ASSERT("is present 1", !macroDictPresent(&macros, "key 17"));

	initASTMacroDefn(&macro, strdup("value 17"));
	T_ASSERT("insert 17", 1 == macroDictInsert(&macros, strdup("key 17"), macro));
	T_ASSERT("is present 2", macroDictPresent(&macros, "key 17"));

	initASTMacroDefn(&macro, strdup("value 17"));
	T_ASSERT("insert 17 2", 0 == macroDictInsert(&macros, strdup("key 17"), macro));

	freeMacroDict(&macros);
}

void macroDictTestDelete() {
	MacroDict macros;
	ASTMacroDef macro;

	tStartSection("Macro Dict test delete");
	initMacroDict(&macros);

	initASTMacroDefn(&macro, strdup("value 1"));
	T_ASSERT("insert 1", 1 == macroDictInsert(&macros, strdup("key 1"), macro));

	initASTMacroDefn(&macro, strdup("value 2"));
	T_ASSERT("insert 2", 1 == macroDictInsert(&macros, strdup("key 2"), macro));

	initASTMacroDefn(&macro, strdup("value 3"));
	T_ASSERT("insert 3", 1 == macroDictInsert(&macros, strdup("key 3"), macro));

	T_ASSERT("present 1", 1 == macroDictPresent(&macros, "key 2"));
	macroDictDelete(&macros, "key 2");
	T_ASSERT("present 2", 0 == macroDictPresent(&macros, "key 2"));
	initASTMacroDefn(&macro, strdup("value 2"));
	T_ASSERT("insert 2", 1 == macroDictInsert(&macros, strdup("key 2"), macro));
	T_ASSERT("present 3", 1 == macroDictPresent(&macros, "key 2"));

	freeMacroDict(&macros);
}

void macroDictTestRemove() {
	MacroDict macros;
	ASTMacroDef macro, *tempMacro;

	tStartSection("Macro Dict test remove");
	initMacroDict(&macros);

	initASTMacroDefn(&macro, strdup("value 1"));
	T_ASSERT("insert 1", 1 == macroDictInsert(&macros, strdup("key 1"), macro));

	initASTMacroDefn(&macro, strdup("value 2"));
	T_ASSERT("insert 2", 1 == macroDictInsert(&macros, strdup("key 2"), macro));

	initASTMacroDefn(&macro, strdup("value 3"));
	T_ASSERT("insert 3", 1 == macroDictInsert(&macros, strdup("key 3"), macro));

	T_ASSERT("present 1", 1 == macroDictPresent(&macros, "key 2"));
	tempMacro = macroDictRemove(&macros, "key 2");
	T_ASSERT("present 2", 0 == macroDictPresent(&macros, "key 2"));
	T_ASSERT("test value 1", 0 == strcmp("value 2", tempMacro->name));
	freeASTMacroDef(tempMacro);
	free(tempMacro);

	T_ASSERT("present 3", 1 == macroDictPresent(&macros, "key 1"));
	tempMacro = macroDictRemove(&macros, "key 1");
	T_ASSERT("present 4", 0 == macroDictPresent(&macros, "key 1"));
	T_ASSERT("test value 2", 0 == strcmp("value 1", tempMacro->name));
	freeASTMacroDef(tempMacro);
	free(tempMacro);


	freeMacroDict(&macros);
}

int macroDictPlayground(int argc, char **argv) {
	MacroDict macros;
	size_t lineSize;
	int lineRes;

	initMacroDict(&macros);

	printf("Hello World!\n");

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

		//macroDictInsert(&macros, key, value);
		printf("\n");
	}

	printMacroDictV(&macros);

	freeMacroDict(&macros);
	return 0;
}

void macroDictTest() {
	macroDictTestEmpty();
	macroDictTestInsert();
	macroDictTestDelete();
	macroDictTestRemove();
}

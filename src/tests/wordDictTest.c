#include <string.h>

#include "wordDictTest.h"
#include "../util/wordDict.h"
#include "test.h"

void wordDictTestInsert() {
	WordDict dict;

	tStartSection("Word Dict test insert");

	initWordDict(&dict);

	tAssert("insert 1", 1 == wordDictInsert(&dict, strdup("value 1"), 1));
	tAssert("insert 2", 1 == wordDictInsert(&dict, strdup("value 2"), 2));
	tAssert("bad insert 1", 0 == wordDictInsert(&dict, strdup("value 1"), 5));
	tAssert("insert 3", 1 == wordDictInsert(&dict, strdup("value 3"), 1));
	tAssert("bad insert 2", 0 == wordDictInsert(&dict, strdup("value 3"), 10));

	freeWordDict(&dict);
}

void wordDictTestPresent() {
	WordDict dict;

	tStartSection("Word Dict test Present");

	initWordDict(&dict);

	tAssert("insert 1", 1 == wordDictInsert(&dict, strdup("important value 1"), 1));
	tAssert("insert 2", 1 == wordDictInsert(&dict, strdup("value the 2nd"), 1));
	tAssert("present 1", 1 == wordDictPresent(&dict, "important value 1"));
	tAssert("present 2", 1 == wordDictPresent(&dict, "value the 2nd"));
	tAssert("present 3", 0 == wordDictPresent(&dict, "3rd value"));
	tAssert("insert 3", 1 == wordDictInsert(&dict, strdup("3rd value"), 1));
	tAssert("present 4", 1 == wordDictPresent(&dict, "3rd value"));

	freeWordDict(&dict);
}

void wordDictTestGet() {
	WordDict dict;

	tStartSection("Word Dict test get");

	initWordDict(&dict);

	tAssert("insert 1", 1 == wordDictInsert(&dict, strdup("item 1"), 11));
	tAssert("insert 2", 1 == wordDictInsert(&dict, strdup("item 2"), 12));
	tAssert("get 1", 11 == *wordDictGet(&dict, "item 1"));
	tAssert("get 2", 12 == *wordDictGet(&dict, "item 2"));
	tAssert("get 3", NULL == wordDictGet(&dict, "item 3"));
	tAssert("insert 3", 1 == wordDictInsert(&dict, strdup("item 3"), 13));
	tAssert("get 4", 13 == *wordDictGet(&dict, "item 3"));

	freeWordDict(&dict);
}

void wordDictTestDelete() {
	WordDict dict;

	tStartSection("Word Dict test delete");

	initWordDict(&dict);

	tAssert("insert 1", 1 == wordDictInsert(&dict, strdup("thing 1"), 1));
	tAssert("insert 2", 1 == wordDictInsert(&dict, strdup("thing 2"), 1));
	tAssert("present 1", 1 == wordDictPresent(&dict, "thing 1"));
	tAssert("present 2", 1 == wordDictPresent(&dict, "thing 2"));
	
	wordDictDelete(&dict, "thing 2");
	tAssert("present 3", 1 == wordDictPresent(&dict, "thing 1"));
	tAssert("present 4", 0 == wordDictPresent(&dict, "thing 2"));

	wordDictDelete(&dict, "thing 1");
	tAssert("present 5", 0 == wordDictPresent(&dict, "thing 1"));
	tAssert("present 6", 0 == wordDictPresent(&dict, "thing 2"));

	freeWordDict(&dict);
}

void wordDictTestRemove() {
	WordDict dict;

	tStartSection("Word Dict test remove");

	initWordDict(&dict);

	tAssert("insert 1", 1 == wordDictInsert(&dict, strdup("thing 1"), 101));
	tAssert("insert 2", 1 == wordDictInsert(&dict, strdup("thing 2"), 102));
	tAssert("present 1", 1 == wordDictPresent(&dict, "thing 1"));
	tAssert("present 2", 1 == wordDictPresent(&dict, "thing 2"));
	
	tAssert("remove 1", 102 == wordDictRemove(&dict, "thing 2"));
	tAssert("present 3", 1 == wordDictPresent(&dict, "thing 1"));
	tAssert("present 4", 0 == wordDictPresent(&dict, "thing 2"));

	tAssert("remove 2", 101 == wordDictRemove(&dict, "thing 1"));
	tAssert("present 5", 0 == wordDictPresent(&dict, "thing 1"));
	tAssert("present 6", 0 == wordDictPresent(&dict, "thing 2"));

	freeWordDict(&dict);
}

void wordDictTest() {
	wordDictTestInsert();
	wordDictTestPresent();
	wordDictTestGet();
	wordDictTestDelete();
	wordDictTestRemove();
}


#include "../util/wordList.h"
#include "test.h"

void wordListTestEmpty() {
	WordList list;

	tStartSection("WordList test empty");
	initWorldList(&list);
	tAssert("Test size", list.size == 0);

	destroyWordList(&list);
}

void wordListTestApp() {
	WordList list;

	tStartSection("WordList test append");
	initWorldList(&list);

	wordListApp(&list, strdup("Item number 1"));
	tAssert("check 1", strcmp(wordListGet(&list, 0), "Item number 1") == 0);

	wordListApp(&list, strdup("Item number 2"));
	tAssert("check 2", strcmp(wordListGet(&list, 1), "Item number 2") == 0);

	wordListApp(&list, strdup("Item number 3"));
	tAssert("check 3", strcmp(wordListGet(&list, 2), "Item number 3") == 0);

	destroyWordList(&list);
}

void wordListTestIns() {
	WordList list;

	tStartSection("WordList test insert");
	initWorldList(&list);

	wordListIns(&list, strdup("Item number 3"), 0);
	wordListIns(&list, strdup("Item number 1"), 0);
	wordListIns(&list, strdup("Item number 2"), 1);

	tAssert("check size", list.size == 3);
	tAssert("check 1", strcmp(wordListGet(&list, 0), "Item number 1") == 0);
	tAssert("check 2", strcmp(wordListGet(&list, 1), "Item number 2") == 0);
	tAssert("check 3", strcmp(wordListGet(&list, 2), "Item number 3") == 0);

	destroyWordList(&list);
}

void wordListTestRem() {
	WordList list;

	tStartSection("WordList test rem");
	initWorldList(&list);

	wordListApp(&list, strdup("item 1"));
	wordListApp(&list, strdup("item 1.1"));
	wordListApp(&list, strdup("item 2"));
	wordListApp(&list, strdup("item 2.1"));

	tAssert("check size", list.size == 4);
	
	tAssert("remove 1", wordListRem(&list, 3) == DLIST_SUCCESS);
	tAssert("remove 2", wordListRem(&list, 3) == DLIST_INVALID_INDEX);
	tAssert("remove 3", wordListRem(&list, -1) == DLIST_INVALID_INDEX);
	tAssert("remove 4", wordListRem(&list, 1) == DLIST_SUCCESS);

	tAssert("check size 2", list.size == 2);
	tAssert("check 1", strcmp(wordListGet(&list, 0), "item 1") == 0);
	tAssert("check 2", strcmp(wordListGet(&list, 1), "item 2") == 0);

	destroyWordList(&list);
}

void wordListTest() {
	wordListTestEmpty();
	wordListTestApp();
	wordListTestIns();
	wordListTestRem();
}

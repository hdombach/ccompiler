#include "../util/dlist.h"
#include <stdlib.h>

#include "dlistTest.h"
#include "test.h"
#include "../util/callbacks.h"

/*
 * Struct for testing with
 */
typedef struct {
	int *data;
	int offset;
	int uuid;
} _TestObj;

int _TestObjUUID = 0;

void _initTestType(_TestObj *obj) {
	obj->data = malloc(sizeof(int) * 3);

	obj->offset = rand() % 10000;

	obj->data[0] = 1 + obj->offset;
	obj->data[1] = 2 + obj->offset;
	obj->data[2] = 3 + obj->offset;

	obj->uuid = _TestObjUUID++;
}

void _freeTestType(_TestObj *obj) {
	free(obj->data);
}

int _testTypeCmp(const _TestObj *lhs, const _TestObj *rhs) {
	return lhs->uuid == rhs->uuid;
}

void _testObjCpy(_TestObj *dest, _TestObj const *src) {
	_initTestType(dest);
	dest->uuid = src->uuid;
	dest->offset = src->offset;
	dest->data[0] = src->data[0];
	dest->data[1] = src->data[1];
	dest->data[2] = src->data[2];

}

int _intCmp(int const *lhs, int const *rhs) {
	return *lhs == *rhs;
}

/*
 * Hopefully, if there are any bugs in dlist cuasing
 * elements to overlap, this function will be able to detect that
 */
int _validTestType(_TestObj const *obj) {
	return obj->data[0] - obj->offset == 1 &&
		obj->data[1] - obj->offset == 2 &&
		obj->data[2] - obj->offset == 3;
}

void dlistTestEmpty() {
	DList list;

	tStartSection("DList test empty");
	initDList(&list, sizeof(int));
	tAssert("test size", 0 == list.size);

	freeDList(&list, NULL);
}

void dlistTestCmp() {
	DList list1;
	DList list2;
	int value;

	tStartSection("DList test compare function");

	initDList(&list1, sizeof(int));
	initDList(&list2, sizeof(int));
	tAssert("empty are equal", dlistCmp(&list1, &list2, (CmpFunc) _intCmp));

	value = 1;
	dlistApp(&list1, &value);
	tAssert("check 2", !dlistCmp(&list1, &list2, (CmpFunc) _intCmp));
	dlistApp(&list2, &value);
	tAssert("check 3", dlistCmp(&list1, &list2, (CmpFunc) _intCmp));

	value = 2;
	dlistApp(&list2, &value);
	value = 3;
	dlistApp(&list2, &value);
	tAssert("check 4", !dlistCmp(&list1, &list2, (CmpFunc) _intCmp));

	value = 2;
	dlistApp(&list1, &value);
	value = 3;
	dlistApp(&list1, &value);
	tAssert("check 5", dlistCmp(&list1, &list2, (CmpFunc) _intCmp));

	freeDList(&list1, (FreeFunc) NULL);
	freeDList(&list2, (FreeFunc) NULL);
}

void dlistTestApp() {
	DList list;
	int value;

	tStartSection("Appending to dlist");

	initDList(&list, sizeof(int));
	tAssert("List starts emtpy", 0 == list.size);

	value = 5;
	dlistApp(&list, &value);
	value = 6;
	dlistApp(&list, &value);
	value = 7;
	dlistApp(&list, &value);

	tAssert("List length", list.size == 3);
	tAssert("item 1", *(int*) dlistGetm(&list, 0) == 5);
	tAssert("item 2", *(int*) dlistGetm(&list, 1) == 6);
	tAssert("item 3", *(int*) dlistGetm(&list, 2) == 7);

	freeDList(&list, NULL);
}

void dlistTestIns() {
	DList list;
	int value;

	tStartSection("Insert into dlist");
	initDList(&list, sizeof(int));
	value = 1;
	tAssert("Invalid index 1", dlistIns(&list, &value, -1) == DLIST_INVALID_INDEX);
	tAssert("Invalid index 2", dlistIns(&list, &value, 1) == DLIST_INVALID_INDEX);

	tAssert("insert 1", dlistIns(&list, (value=3, &value), 0) == DLIST_SUCCESS); //3
	tAssert("insert 2", dlistIns(&list, (value=1, &value), 0) == DLIST_SUCCESS); //1, 3
	tAssert("insert 3", dlistIns(&list, (value=4, &value), 2) == DLIST_SUCCESS); //1, 3, 4
	tAssert("insert 4", dlistIns(&list, (value=2, &value), 1) == DLIST_SUCCESS); //1, 3, 4

	tAssert("check 1", *(int*) dlistGetm(&list, 0) == 1);
	tAssert("check 2", *(int*) dlistGetm(&list, 1) == 2);
	tAssert("check 3", *(int*) dlistGetm(&list, 2) == 3);
	tAssert("check 4", *(int*) dlistGetm(&list, 3) == 4);

	freeDList(&list, NULL);
}

void dlistTestRem() {
	DList list;
	int value;

	tStartSection("Removing from dlist");
	initDList(&list, sizeof(int));
	dlistApp(&list, (value=1, &value));
	dlistApp(&list, (value=2, &value));
	dlistApp(&list, (value=3, &value));
	dlistApp(&list, (value=4, &value));

	tAssert("invalid index 1", dlistRem(&list, -1, NULL) == DLIST_INVALID_INDEX);
	tAssert("invalid index 2", dlistRem(&list, 4, NULL) == DLIST_INVALID_INDEX);

	tAssert("remove 1", dlistRem(&list, 1, NULL) == DLIST_SUCCESS);
	tAssert("check 1", *(int*) dlistGetm(&list, 0) == 1);
	tAssert("check 2", *(int*) dlistGetm(&list, 1) == 3);
	tAssert("check 3", *(int*) dlistGetm(&list, 2) == 4);
	tAssert("check size 2", list.size == 3);

	tAssert("remove 2", dlistRem(&list, 2, NULL) == DLIST_SUCCESS);
	tAssert("check 4", *(int*) dlistGetm(&list, 0) == 1);
	tAssert("check 5", *(int*) dlistGetm(&list, 1) == 3);
	tAssert("check size 3", list.size == 2);

	tAssert("remove 3", dlistRem(&list, 0, NULL) == DLIST_SUCCESS);
	tAssert("check 5", *(int*) dlistGetm(&list, 0) == 3);
	tAssert("check size 4", list.size == 1);

	tAssert("remove 4", dlistRem(&list, 0, NULL) == DLIST_SUCCESS);
	tAssert("check size 5", list.size == 0);

	tAssert("invalid index 3", dlistRem(&list, 0, NULL) == DLIST_INVALID_INDEX);

	freeDList(&list, NULL);
}

void dlistTestDynObj() {
	_TestObj obj1;
	_TestObj obj2;
	_TestObj obj3;
	_TestObj tempObj;
	DList list1;
	DList list2;

	tStartSection("Testing dlist with objects");
	_initTestType(&obj1);
	_initTestType(&obj2);
	_initTestType(&obj3);
	initDList(&list1, sizeof(_TestObj));
	initDList(&list2, sizeof(_TestObj));

	_testObjCpy(&tempObj, &obj1);
	dlistApp(&list1, &tempObj);
	_testObjCpy(&tempObj, &obj2);
	dlistApp(&list1, &tempObj);
	_testObjCpy(&tempObj, &obj3);
	dlistApp(&list1, &tempObj);

	dlistApp(&list2, &obj2); //2
	dlistIns(&list2, &obj1, 0); //1, 2
	dlistIns(&list2, &obj3, 2); //1, 2, 3

	tAssert("compare 1", dlistCmp(&list1, &list2, (CmpFunc) _testTypeCmp));

	tAssert("valid 1", _validTestType(dlistGet(&list1, 0)));
	tAssert("valid 2", _validTestType(dlistGet(&list1, 1)));
	tAssert("valid 3", _validTestType(dlistGet(&list1, 2)));
	tAssert("valid 4", _validTestType(dlistGet(&list2, 0)));
	tAssert("valid 5", _validTestType(dlistGet(&list2, 1)));
	tAssert("valid 6", _validTestType(dlistGet(&list2, 2)));

	_testObjCpy(&tempObj, dlistGet(&list1, 0));
	dlistRem(&list1, 1, (FreeFunc) _freeTestType);
	dlistIns(&list1, &tempObj, 1);
	tAssert("valid 1", _validTestType(dlistGet(&list1, 0)));
	tAssert("valid 2", _validTestType(dlistGet(&list1, 1)));
	tAssert("valid 3", _validTestType(dlistGet(&list1, 2)));
	tAssert("compare 2", _testTypeCmp(dlistGet(&list1, 0), dlistGet(&list1, 1)));
	tAssert("compare 3", !dlistCmp(&list1, &list2, (CmpFunc) _testTypeCmp));

	freeDList(&list1, (FreeFunc) _freeTestType);
	freeDList(&list2, (FreeFunc) _freeTestType);
}

void dlistTestRemMult() {
	DList list;
	int value;

	tStartSection("Removing multiple from dlist");
	initDList(&list, sizeof(int));
	dlistApp(&list, (value=1, &value));
	dlistApp(&list, (value=2, &value));
	dlistApp(&list, (value=3, &value));
	dlistApp(&list, (value=4, &value));
	dlistApp(&list, (value=5, &value));
	dlistApp(&list, (value=6, &value));
	dlistApp(&list, (value=7, &value));

	dlistRemMult(&list, 2, 3, NULL);
	tAssert("index 0", *(int *) dlistGet(&list, 0) == 1);
	tAssert("index 1", *(int *) dlistGet(&list, 1) == 2);
	tAssert("index 2", *(int *) dlistGet(&list, 2) == 6);
	tAssert("index 3", *(int *) dlistGet(&list, 3) == 7);

	freeDList(&list, NULL);
}

void dlistTestInsMult() {
	DList list, elements;
	int value;

	tStartSection("Insert multiple into dlist");
	initDList(&list, sizeof(int));
	initDList(&elements, sizeof(int));

	dlistApp(&list, (value=1, &value));
	dlistApp(&list, (value=2, &value));
	dlistApp(&list, (value=3, &value));
	dlistApp(&list, (value=4, &value));
	dlistApp(&list, (value=5, &value));
	dlistApp(&elements, (value=6, &value));
	dlistApp(&elements, (value=7, &value));
	dlistApp(&elements, (value=8, &value));

	dlistInsMult(&list, &elements, 1);

	tAssert("index 0", *(int *) dlistGet(&list, 0) == 1);
	tAssert("index 1", *(int *) dlistGet(&list, 1) == 6);
	tAssert("index 2", *(int *) dlistGet(&list, 2) == 7);
	tAssert("index 3", *(int *) dlistGet(&list, 3) == 8);
	tAssert("index 4", *(int *) dlistGet(&list, 4) == 2);
	tAssert("index 5", *(int *) dlistGet(&list, 5) == 3);
	tAssert("index 6", *(int *) dlistGet(&list, 6) == 4);
	tAssert("index 7", *(int *) dlistGet(&list, 7) == 5);

	freeDList(&list, NULL);
}

void dlistTest() {
	dlistTestEmpty();
	dlistTestCmp();
	dlistTestApp();
	dlistTestIns();
	dlistTestRem();
	dlistTestDynObj();
	dlistTestRemMult();
	dlistTestInsMult();
}


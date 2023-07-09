#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dlist.h"

const int DLIST_INITIAL_CAP = 2;

void initDList(DList *list, size_t elementSize) {
	list->size = 0;
	list->capacity = DLIST_INITIAL_CAP;
	list->elSize = elementSize;
	list->data = malloc(list->elSize * list->capacity);
}

void freeDList(DList *list, DListFreeFunc freeFunc) {
	if (freeFunc) {
		for (int i = 0; i < list->size; i++) {
			freeFunc(dlistGetm(list, i));
		}
	}
	free(list->data);
}

void *dlistGetm(DList *list, int index) {
	//TODO: add debug boundary tests
	return list->data + index * list->elSize;
}

const void *dlistGet(DList const *list, int index) {
	return list->data + index * list->elSize;
}

void dlistApp(DList *list, const void *element) {
	if (list->size >= list->capacity) {
		dlistIncCap(list);
	}

	memcpy(dlistGetm(list, list->size), element, list->elSize);
	list->size++;
}

void dlistIncCap(DList *list) {
	list->capacity *= 2;
	list->data = realloc(list->data, list->capacity * list->elSize);
}

void dlistDecCap(DList *list) {
	if (list->capacity > DLIST_INITIAL_CAP) {
		list->capacity /= 2;
		list->data = realloc(list->data, list->capacity * list->elSize);
	}
}

DListErr dlistRem(DList *list, int index, DListFreeFunc freeFunc) {
	size_t mvLen;

	if (index < 0 || index >= list->size) {
		return DLIST_INVALID_INDEX;
	}
	if (freeFunc) {
		freeFunc(dlistGetm(list, index));
	}
	mvLen = (list->size - 1 - index) * list->elSize;
	memmove(dlistGetm(list, index), dlistGet(list, index + 1), mvLen);
	list->size--;
	if (list->size * 2 <= list->capacity) {
		dlistDecCap(list);
	}

	return DLIST_SUCCESS;
}

DListErr dlistIns(DList *list, const void *element, int index) {
	size_t mvLen;

	if (index < 0 ||  index > list->size) {
		return DLIST_INVALID_INDEX; 
	}
	if (list->size >= list->capacity) {
		dlistIncCap(list);
	}

	mvLen = (list->size - index) * list->elSize;
	memmove(dlistGetm(list, index + 1), dlistGet(list, index), mvLen);
	memcpy(dlistGetm(list, index), element, list->elSize);
	list->size++;

	return DLIST_SUCCESS;
}

void dlistRemAll(DList *list, DListFreeFunc freeFunc) {
	if (freeFunc) {
		for (int i = 0; i < list->size; i++) {
			freeFunc(dlistGetm(list, i));;
		}
	}
	list->capacity = DLIST_INITIAL_CAP;
	list->size = 0;
	list->data = realloc(list->data, list->capacity * list->elSize);
}

int dlistCmp(const DList *lhs, const DList *rhs, DListCmpFunc cmpFunc) {
	if (lhs->size != rhs->size) {
		return 0;
	}
	for (int i = 0; i < lhs->size; i++) {
		if (!cmpFunc(dlistGet(lhs, i), dlistGet(rhs, i))) {
			return 0;
		}
	}
	return 1;
}

void dlistPrint(const DList *list, DListPrintFunc printFunc) {
	printf("[");
	for (int i = 0; i < list->size; i++) {
		printFunc(dlistGet(list, i));
		if (i < list->size - 1) {
			printf(", ");
		}
	}
	printf("]");
}

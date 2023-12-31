#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dlist.h"
#include "callbacks.h"

const int DLIST_INITIAL_CAP = 2;

void initDList(DList *list, size_t elementSize) {
	list->size = 0;
	list->capacity = DLIST_INITIAL_CAP;
	list->elSize = elementSize;
	list->data = malloc(list->elSize * list->capacity);
}

void initDListCap(DList *list, size_t elementSize, size_t initialCap) {
	list->size = 0;
	list->capacity = initialCap;
	list->elSize = elementSize;
	list->data = malloc(list->elSize * list->capacity);
}

void initDListEmpty(DList *list, size_t elementSize) {
	list->size = 0;
	list->capacity = 0;
	list->elSize = elementSize;
	list->data = NULL;
}

void freeDList(DList *list, FreeFunc freeFunc) {
	if (!list->data) {
		return;
	}
	if (freeFunc) {
		for (int i = 0; i < list->size; i++) {
			freeFunc(dlistGetm(list, i));
		}
	}
	free(list->data);
	list->data = NULL;
}

void moveDList(DList *dest, DList *src) {
	*dest = *src;
	src->data = NULL;
}

void cpDList(DList *dest, const DList *src, CpFunc cpFunc) {
	dest->size = src->size;
	dest->capacity = src->capacity;
	dest->elSize = src->elSize;
	dest->data = malloc(sizeof(dest->capacity * dest->elSize));

	for (int i = 0; i < dest->size; i++) {
		if (cpFunc) {
			cpFunc(dlistGetm(dest, i), dlistGet(src, i));
		} else {
			memcpy(dlistGetm(dest, i), dlistGet(src, i), dest->elSize);
		}
	}
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
	if (list->data) {
		list->capacity *= 2;
		list->data = realloc(list->data, list->capacity * list->elSize);
	} else {
		list->capacity = DLIST_INITIAL_CAP;
		list->data = malloc(list->capacity * list->elSize);
	}
}

void dlistDecCap(DList *list) {
	if (list->capacity > DLIST_INITIAL_CAP) {
		list->capacity /= 2;
		list->data = realloc(list->data, list->capacity * list->elSize);
	}
}

DListErr dlistRem(DList *list, int index, FreeFunc freeFunc) {
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

void dlistRemLast(DList *list, FreeFunc freeFunc) {
	if (list->size == 0) {
		return;
	}
	freeFunc(dlistGetm(list, list->size - 1));
	list->size--;
}

DListErr dlistRemMult(DList *list, int start, int count, FreeFunc freeFunc) {
	size_t mvLen;

	if (start < 0 || start + count > list->size) {
		return DLIST_INVALID_INDEX;
	}
	if (freeFunc) {
		for (int i = start; i < start + count; i++) {
			freeFunc(dlistGetm(list, i));
		}
	}

	mvLen = (list->size - (start + count)) * list->elSize;
	memmove(dlistGetm(list, start), dlistGet(list, start + count), mvLen);
	list->size -= count;
	while (list->size * 2 <= list->capacity) {
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

DListErr dlistInsMult(DList *list, const DList *elements, int index) {
	size_t mvLen;

	if (index < 0 || index > list->size) {
		return DLIST_INVALID_INDEX;
	}
	while (list->size + elements->size > list->capacity) {
		dlistIncCap(list);
	}

	mvLen = (list->size - index) * list->elSize;
	memmove(dlistGetm(list, index + elements->size), dlistGet(list, index), mvLen);
	memcpy(dlistGetm(list, index), elements->data, elements->elSize * elements->size);
	list->size += elements->size;
	free(elements->data);

	return DLIST_SUCCESS;
}

void dlistRemAll(DList *list, FreeFunc freeFunc) {
	if (freeFunc) {
		for (int i = 0; i < list->size; i++) {
			freeFunc(dlistGetm(list, i));;
		}
	}
	//TODO: I am not sure whether it is faster or not to reallocate when decrease size
	//list->capacity = DLIST_INITIAL_CAP;
	//list->data = realloc(list->data, list->capacity * list->elSize);
	list->size = 0;
}

int dlistCmp(const DList *lhs, const DList *rhs, CmpFunc cmpFunc) {
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

int printDList(const DList *list, PrintFunc printFunc) {
	int n = 0;
	n += printf("[");
	for (int i = 0; i < list->size; i++) {
		n += printFunc(dlistGet(list, i));
		if (i < list->size - 1) {
			n += printf(", ");
		}
	}
	n += printf("]");
	return n;
}

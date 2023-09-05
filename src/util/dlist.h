#pragma once

#include <stdlib.h>

/*
 * A generic dynamic list implimentation
 */
typedef struct {
	void *data;
	int size;
	int capacity;
	size_t elSize;
} DList;

typedef enum {
	DLIST_SUCCESS = 0,
	DLIST_INVALID_ARG = 1,
	DLIST_INVALID_INDEX = 2,
} DListErr;

typedef void (*DListFreeFunc)(void*);
typedef int (*DListCmpFunc)(void const*, void const*);
typedef void (*DListCpFunc)(void *, void const*);
typedef int (*DListPrintFunc)(void const*);

void initDList(DList *list, size_t elementSize);
void freeDList(DList *list, DListFreeFunc freeFunc);
void moveDList(DList *dest, DList *src);

void *dlistGetm(DList *list, int index);
const void *dlistGet(DList const *list, int index);

/*
 * Appends an element to the list.
 * (performs a shallow copy of the element).
 */
void dlistApp(DList *list, const void *element);

/*
 * Increase the capacity of the list
 */
void dlistIncCap(DList *list);

/*
 * Decreases the capacity of the list
 */
void dlistDecCap(DList *list);

/*
 * Removes an element at the index
 */
DListErr dlistRem(DList *list, int index, DListFreeFunc);

void dlistRemLast(DList *list, DListFreeFunc);

/*
 * Removes range of elements
 */
DListErr dlistRemMult(DList *list, int start, int count, DListFreeFunc);

/*
 * Inserts an element at the index
 * Note: Takes ownership of element
 */
DListErr dlistIns(DList *list, const void *element, int index);

/*
 * Inserts a dlist into the dlist
 * Note: Takes onwership and frees elements
 */
DListErr dlistInsMult(DList *list, DList const *elements, int index);

void dlistRemAll(DList *list, DListFreeFunc);

/*
 * Compares two lists
 */
int dlistCmp(const DList *lhs, const DList *rhs, DListCmpFunc cmpFunc);

int printDList(const DList *list, DListPrintFunc printFunc);

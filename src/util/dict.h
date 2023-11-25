#pragma once

#include "callbacks.h"
#include <stddef.h>

typedef struct DictNode {
	struct DictNode *next;
	void *key;
	void *value;
} DictNode;

typedef struct {
	size_t allocatedSize;
	size_t elementCount;

	DictNode **nodes;
} Dict;

void initDict(Dict *dict);
void freeDict(Dict *dict, FreeFunc freeKeyFunc, FreeFunc freeValueFunc);
void cpDict(
		Dict *dest,
		Dict const *src,
		CpFunc cpKey,
		CpFunc cpValue,
		size_t keySize,
		size_t valueSize);

void initDictNode(
		DictNode *node,
		void *key,
		void *value,
		size_t keySize,
		size_t valueSize);

void freeDictNode(
		DictNode *node,
		FreeFunc freeKeyFunc,
		FreeFunc freeValueFunc);

/*
 * Inserts an item with the given key into the hash table
 * Returns 1 on success
 * Returns 0 if item already exists.
 */
int dictInsert(
		Dict *dict,
		void *key,
		void *value,
		size_t keySize,
		size_t valueSize,
		HashFunc hashFunc,
		CmpFunc keyCmpFunc,
		FreeFunc freeKeyFunc,
		FreeFunc freeValueFunc);

int dictPresent(
		Dict const *dict,
		void const *key,
		HashFunc hashFunc,
		CmpFunc cmpKeyFunc);

void const *dictGet(
		Dict const *dict,
		void const *key,
		HashFunc hashFunc,
		CmpFunc cmpKeyFunc);

void *dictGetm(
		Dict *dict,
		void const *key,
		HashFunc hashFunc,
		CmpFunc cmpKeyFunc);

void dictDelete(
		Dict *dict,
		void const *key,
		HashFunc hashFunc,
		CmpFunc cmpKeyFunc,
		FreeFunc freeKeyFunc,
		FreeFunc freeValueFunc);

void *dictRemove(
		Dict *dict,
		void const *key,
		HashFunc hashFunc,
		CmpFunc cmpKeyFunc,
		FreeFunc freeKeyFunc);

/*
 * Verbose print with info about layout
 */

int printDictV(
		Dict const *dict,
		PrintFunc printKeyFunc,
		PrintFunc printValueFunc);

int printDict(
		Dict const *dict,
		PrintFunc printKeyFunc,
		PrintFunc printValueFunc);

typedef void (*DictIterFunc)(void *key, void *value, void *context);

void dictIter(Dict *dict, DictIterFunc func, void *context);

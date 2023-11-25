#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dict.h"
#include "callbacks.h"
#include "hash.h"

const int DICT_INIT_SIZE = 16;

void initDict(Dict *dict) {
	dict->allocatedSize = DICT_INIT_SIZE;
	dict->elementCount = 0;
	dict->nodes = calloc(dict->allocatedSize, sizeof(DictNode*));
}

void freeDict(Dict *dict, FreeFunc freeKeyFunc, FreeFunc freeValueFunc) {
	for (int i = 0; i < dict->allocatedSize; i++) {
		DictNode *curNode, *nextNode;

		curNode = dict->nodes[i];
		while (curNode) {
			nextNode = curNode->next;
			freeDictNode(curNode, freeKeyFunc, freeValueFunc);
			free(curNode);
			curNode = nextNode;
		}
	}
	free(dict->nodes);
}

void cpDict(
		Dict *dest,
		Dict const *src,
		CpFunc cpKey,
		CpFunc cpValue,
		size_t keySize,
		size_t valueSize)
{
	dest->allocatedSize = src->allocatedSize;
	dest->elementCount = src->elementCount;

	dest->nodes = malloc(sizeof(DictNode*) * dest->allocatedSize);

	for (int i = 0; i < dest->allocatedSize; i++) {
		DictNode *curSrc = src->nodes[i];
		DictNode **curDest = dest->nodes + i;
		while (curSrc) {
			*curDest = malloc(sizeof(DictNode));

			(*curDest)->key = malloc(keySize);
			cpKey(&(*curDest)->key, curSrc->key);

			(*curDest)->value = malloc(valueSize);
			cpValue(&(*curDest)->value, curSrc->value);

			(*curDest)->next = NULL;

			curDest = &(*curDest)->next;
			curSrc = curSrc->next;
		}
	}
}

void initDictNode(
		DictNode *node,
		void *key,
		void *value,
		size_t keySize,
		size_t valueSize)
{
	node->key = malloc(keySize);
	memcpy(node->key, key, keySize);

	node->value = malloc(valueSize);
	memcpy(node->value, value, valueSize);
	node->next = NULL;
}

void freeDictNode(
		DictNode *node,
		FreeFunc freeKeyFunc,
		FreeFunc freeValueFunc)
{
	if (freeKeyFunc) {
		freeKeyFunc(node->key);
	}
	free(node->key);
	if (freeValueFunc) {
		freeValueFunc(node->value);
	}
	free(node->value);
}

int dictInsert(
		Dict *dict,
		void *key,
		void *value,
		size_t keySize,
		size_t valueSize,
		HashFunc hashFunc,
		CmpFunc keyCmpFunc,
		FreeFunc freeKeyFunc,
		FreeFunc freeValueFunc)
{
	hash_t index;
	DictNode **curNode;

	index = hashFunc(key) % dict->allocatedSize;
	curNode = &dict->nodes[index];
	while (*curNode) {
		if (keyCmpFunc((*curNode)->key, key)) {
			if (freeKeyFunc) {
				freeKeyFunc(key);
			}
			if (freeValueFunc) {
				freeValueFunc(value);
			}
			return 0;
		}
		curNode = &((*curNode)->next);
	}
	*curNode = malloc(sizeof(DictNode));
	initDictNode(*curNode, key, value, keySize, valueSize);
	dict->elementCount++;
	return 1;
}

int dictPresent(
		const Dict *dict,
		void const *key,
		HashFunc hashFunc,
		CmpFunc cmpKeyFunc)
{
	hash_t index;
	DictNode *curNode;

	index = hashFunc(key) % dict->allocatedSize;
	curNode = dict->nodes[index];
	while (curNode) {
		if (cmpKeyFunc(curNode->key, key)) {
			return 1;
		}
		curNode = curNode->next;
	}
	return 0;
}

void const *dictGet(Dict const *dict, void const *key, HashFunc hashFunc, CmpFunc cmpKeyFunc) {
	hash_t index;
	DictNode *curNode;

	index = hashFunc(key) % dict->allocatedSize;
	curNode = dict->nodes[index];
	while (curNode) {
		if (cmpKeyFunc(curNode->key, key)) {
			return curNode->value;
		}
		curNode = curNode->next;
	}
	return NULL;
}

void *dictGetm(Dict *dict, void const *key, HashFunc hashFunc, CmpFunc cmpKeyFunc) {
	hash_t index;
	DictNode *curNode;

	index = hashFunc(key) % dict->allocatedSize;
	curNode = dict->nodes[index];
	while (curNode) {
		if (cmpKeyFunc(curNode->key, key)) {
			return curNode->value;
		}
		curNode = curNode->next;
	}
	return NULL;
}

void dictDelete(
		Dict *dict,
		void const *key,
		HashFunc hashFunc,
		CmpFunc cmpKeyFunc,
		FreeFunc freeKeyFunc,
		FreeFunc freeValueFunc)
{
	hash_t index;
	DictNode **curNode;

	index = hashFunc(key) % dict->allocatedSize;
	curNode = &dict->nodes[index];
	while (*curNode) {
		if (cmpKeyFunc((*curNode)->key, key)) {
			DictNode *temp;
			temp = *curNode;
			*curNode = temp->next;
			freeDictNode(temp, freeKeyFunc, freeValueFunc);
			free(temp);
			dict->elementCount--;
			return;
		} else {
			curNode = &(*curNode)->next;
		}
	}
}

void *dictRemove(
		Dict *dict,
		void const *key,
		HashFunc hashFunc,
		CmpFunc cmpKeyFunc,
		FreeFunc freeKeyFunc)
{
	hash_t index;
	DictNode **curNode;

	index = hashFunc(key) % dict->allocatedSize;
	curNode = &dict->nodes[index];

	while (*curNode) {
		if (cmpKeyFunc((*curNode)->key, key)) {
			DictNode *temp;
			void *tempValue;

			temp = *curNode;
			*curNode = temp->next;

			if (freeKeyFunc) {
				freeKeyFunc(temp->key);
			}
			tempValue = temp->value;
			free(temp);
			dict->elementCount--;
			return tempValue;
		} else {
			curNode = &(*curNode)->next;
		}
	}
	return NULL;
}

int printDictV(
		Dict const *dict,
		PrintFunc printKeyFunc,
		PrintFunc printValueFunc)
{
	int n = 0;
	n += printf("[");
	for (int i = 0; i < dict->allocatedSize; i++) {
		DictNode *curNode;
		curNode = dict->nodes[i];
		n += printf("[");
		while (curNode) {
			n += printf("{");
			n += printKeyFunc(curNode->key);
			n += printf(": ");
			if (curNode->next) {
				n += printf(", ");
			}
			curNode = curNode->next;
		}
		n += printf("]");

		if (i < dict->allocatedSize - 1) {
			n += printf(", ");
		}
	}
	n += printf("]");

	return n;
}

int printDict(
		const Dict *dict,
		PrintFunc printKeyFunc,
		PrintFunc printValueFunc)
{
	int isFirst = 1, n = 0;

	n += printf("{");
	for (int i = 0; i < dict->allocatedSize; i++) {
		DictNode *curNode;
		curNode = dict->nodes[i];
		while (curNode) {
			if (isFirst) {
				isFirst = 0;
			} else {
				n += printf(", ");
			}
			n += printKeyFunc(curNode->key);
			n += printf(": ");
			n += printValueFunc(curNode->value);
			curNode = curNode->next;
		}
	}
	n += printf("}");

	return n;
}

void dictIter(Dict *dict, DictIterFunc func, void *context) {
	for (int i = 0; i < dict->allocatedSize; i++) {
		DictNode *curNode;
		curNode = dict->nodes[i];
		while (curNode) {
			func(curNode->key, curNode->value, context);
			curNode = curNode->next;
		}
	}
}

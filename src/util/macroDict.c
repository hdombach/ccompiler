#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>

#include "dstr.h"
#include "hash.h"
#include "macroDict.h"

const int MACRO_DICT_INIT_SIZE = 16;

int _cmpKey(MacroDictKey lhs, MacroDictKey rhs) {
	return strcmp(lhs, rhs) == 0;
}

int _hashKey(MacroDictKey key) {
	return hashStr((unsigned char const *) key);
}

void initMacroDict(MacroDict *macroDict) {
	macroDict->allocatedSize = 16;
	macroDict->elementCount = 0;
	macroDict->nodes = malloc(sizeof(MacroDictNode) * macroDict->allocatedSize);
	for (int i = 0; i < macroDict->allocatedSize; i++) {
		macroDict->nodes[i] = NULL;
	}
}

void freeMacroDict(MacroDict *macroDict) {
	for (int i = 0; i < macroDict->allocatedSize; i++) {
		MacroDictNode *curNode;
		MacroDictNode *nextNode;

		curNode = macroDict->nodes[i];
		while (curNode) {
			nextNode = curNode->next;
			freeMacroDictNode(curNode);
			free(curNode);
			curNode = nextNode;
		}
	}
	free(macroDict->nodes);
}

void initMacroDictNode(
		MacroDictNode *node,
		MacroDictMKey key,
		MacroDictValue value)
{
	node->key = key;

	node->value = malloc(sizeof(MacroDictValue));
	*node->value = value;
	node->next = NULL;
}

void freeMacroDictNode(MacroDictNode *node) {
	free(node->key);
	freeASTMacroDef(node->value);
	free(node->value);
}

int macroDictInsert(
		MacroDict *macroDict,
		MacroDictMKey key,
		MacroDictValue value)
{
	hash_t index;
	MacroDictNode **curNode;

	index = _hashKey(key) % macroDict->allocatedSize;
	curNode = &macroDict->nodes[index];
	while (*curNode) {
		if (_cmpKey((*curNode)->key, key)) {
			free(key);
			freeASTMacroDef(&value);
			return 0;
		}
		curNode = &((*curNode)->next);
	}
	*curNode = malloc(sizeof(MacroDictNode));
	initMacroDictNode(*curNode, key, value);
	return 1;
}

int macroDictPresent(const MacroDict *macroDict, MacroDictKey key) {
	hash_t index;
	MacroDictNode *curNode;

	index = _hashKey(key) % macroDict->allocatedSize;
	curNode = macroDict->nodes[index];
	while (curNode) {
		if (_cmpKey(curNode->key, key)) {
			return 1;
		}
		curNode = curNode->next;
	}
	return 0;
}

MacroDictValue const *macroDictGet(const MacroDict *macroDict, MacroDictKey key) {
	hash_t index;
	MacroDictNode *curNode;

	index = _hashKey(key) % macroDict->allocatedSize;
	curNode = macroDict->nodes[index];
	while (curNode) {
		if (_cmpKey(curNode->key, key)) {
			return curNode->value;
		}
		curNode = curNode->next;
	}
	return NULL;
}

MacroDictValue *macroDictGetm(MacroDict *macroDict, MacroDictKey key) {
	hash_t index;
	MacroDictNode *curNode;

	index = _hashKey(key) % macroDict->allocatedSize;
	curNode = macroDict->nodes[index];
	while (curNode) {
		if (_cmpKey(curNode->key, key)) {
			return curNode->value;
		}
		curNode = curNode->next;
	}
	return NULL;
}

void macroDictDelete(MacroDict *macroDict, MacroDictKey key) {
	hash_t index;
	MacroDictNode **curNode;

	index = _hashKey(key) % macroDict->allocatedSize;
	curNode = &macroDict->nodes[index];
	while (*curNode) {
		if (_cmpKey((*curNode)->key, key)) {
			MacroDictNode *temp;
			temp = *curNode;
			*curNode = temp->next;
			freeMacroDictNode(temp);
			free(temp);
			return;
		} else {
			 curNode = &(*curNode)->next;
		}
	}
}

MacroDictValue *macroDictRemove(MacroDict *macroDict, MacroDictKey key) {
	hash_t index;
	MacroDictNode **curNode;

	index = _hashKey(key) % macroDict->allocatedSize;
	curNode = &macroDict->nodes[index];
	while (*curNode) {
		if (_cmpKey((*curNode)->key, key)) {
			MacroDictNode *temp;
			MacroDictValue *tempValue;

			temp = *curNode;
			*curNode = temp->next;

			free(temp->key);
			tempValue = temp->value;
			free(temp);
			return tempValue;
		} else {
			 curNode = &(*curNode)->next;
		}
	}
	return NULL;
}

void printMacroDictV(const MacroDict *dict) {
	printf("[");
	for (int i = 0; i < dict->allocatedSize; i++) {
		MacroDictNode *curNode;
		curNode = dict->nodes[i];
		printf("[");
		while (curNode) {
			printf("{");
			printJsonStr(curNode->key);
			printf(": ");
			printASTMacroDef(curNode->value);
			printf("}");
			if (curNode->next) {
				printf(", ");
			}
			curNode = curNode->next;
		}
		printf("]");
		
		if (i < dict->allocatedSize - 1) {
			printf(",");
		}
	}
	printf("]");
}

void printMacroDict(MacroDict const *dict) {
	int isFirst = 1;

	printf("{");
	for (int i = 0; i < dict->allocatedSize; i++) {
		MacroDictNode *curNode;
		curNode = dict->nodes[i];
		while (curNode) {
			if (isFirst) {
				isFirst = 0;
			} else {
				printf(", ");
			}
			printJsonStr(curNode->key);
			printf(": ");
			printASTMacroDef(curNode->value);
			curNode = curNode->next;
		}
	}
	printf("}");
}

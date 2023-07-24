#pragma once

#include <stddef.h>

typedef char const * MacroDictKey;
typedef char const * MacroDictValue;
typedef char * MacroDictMKey;
typedef char * MacroDictMValue;

struct _MacroDictNode;

typedef struct _MacroDictNode {
	struct _MacroDictNode *next;
	MacroDictMKey key;
	MacroDictMValue value;
} MacroDictNode;

typedef struct {
	size_t allocatedSize;	
	size_t elementCount;

	MacroDictNode **nodes;
} MacroDict;

void initMacroDict(MacroDict *macroDict);
void freeMacroDict(MacroDict *macroDict);

void initMacroDictNode(
		MacroDictNode *node,
		MacroDictMKey key,
		MacroDictMValue value);
/*
 * Frees a node and returns the next
 */
void freeMacroDictNode(MacroDictNode *node);

/*
 * Inserts an item with the given key into the hash table
 * Returns 1 on success
 * Returns 0 if item already exists.
 */
int macroDictInsert(
		MacroDict *macroDict,
		MacroDictMKey key,
		MacroDictMValue value);

int macroDictPresent(MacroDict const *macroDict, MacroDictKey key);

MacroDictValue macroDictGet(MacroDict const *macroDict, MacroDictKey key);

MacroDictMValue macroDictGetm(MacroDict *macroDict, MacroDictKey key);

/*
 * Looks up an element by key and deletes it
 */
void macroDictDelete(MacroDict *macroDict, MacroDictKey key);

/*
 * Removes an element and returns it
 */
MacroDictMValue macroDictRemove(MacroDict *macroDict, MacroDictKey key);

/*
 * Verbose print with info about layout
 */
void macroDictVPrint(MacroDict const *macroDict);

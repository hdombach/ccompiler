#pragma once

#include <stddef.h>

#include "../ast/macroDef.h"

typedef char const * MacroDictKey;
typedef char * MacroDictMKey;
typedef ASTMacroDef MacroDictValue;

struct _MacroDictNode;

typedef struct _MacroDictNode {
	struct _MacroDictNode *next;
	MacroDictMKey key;
	MacroDictValue *value;
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
		MacroDictValue value);
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
		MacroDictValue value);

/*
 * Inserts the macros that are defined by compiler
 */
void macroDictInsertDefault(MacroDict *macros);

int macroDictPresent(MacroDict const *macroDict, MacroDictKey key);

MacroDictValue const *macroDictGet(MacroDict const *macroDict, MacroDictKey key);

MacroDictValue *macroDictGetm(MacroDict *macroDict, MacroDictKey key);

/*
 * Looks up an element by key and deletes it
 */
void macroDictDelete(MacroDict *macroDict, MacroDictKey key);

/*
 * Removes an element and returns it
 */
MacroDictValue *macroDictRemove(MacroDict *macroDict, MacroDictKey key);

/*
 * Verbose print with info about layout
 */
void printMacroDictV(MacroDict const *macroDict);

void printMacroDict(MacroDict const *macroDict);

#pragma once

#include <stddef.h>

#include "../ast/macroDef.h"
#include "dict.h"

//typedef char const * MacroDictKey;
//typedef char * MacroDictMKey;
//typedef ASTMacroDef MacroDictValue;

typedef Dict MacroDict;

void initMacroDict(MacroDict *macroDict);
void freeMacroDict(MacroDict *macroDict);

/*
 * Inserts an item with the given key into the hash table
 * Returns 1 on success
 * Returns 0 if item already exists.
 */
int macroDictInsert(
		MacroDict *macroDict,
		char *key,
		ASTMacroDef value);

/*
 * Inserts the macros that are defined by compiler
 */
void macroDictInsertDefault(MacroDict *macros);

int macroDictPresent(MacroDict const *macroDict, char const *key);

ASTMacroDef const *macroDictGet(MacroDict const *macroDict, char const *key);

ASTMacroDef *macroDictGetm(MacroDict *macroDict, char const *key);

/*
 * Looks up an element by key and deletes it
 */
void macroDictDelete(MacroDict *macroDict, char const *key);

/*
 * Removes an element and returns it
 */
ASTMacroDef *macroDictRemove(MacroDict *macroDict, char const *key);

/*
 * Verbose print with info about layout
 */
int printMacroDictV(MacroDict const *macroDict);

int printMacroDict(MacroDict const *macroDict);

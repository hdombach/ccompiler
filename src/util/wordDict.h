#pragma once

#include "dict.h"
#include "util.h"

/*
 * Maps a word to an int
 */
typedef Dict WordDict;

static void initWordDict(WordDict *dict) {
	initDict(dict);
}

static void freeWordDict(WordDict *dict) {
	freeDict(dict, (FreeFunc) freeStr, NULL);
}

/*
 * Inserts an item with the given key into the hash table
 * Returns 1 on success
 * Returns 0 if item already exists
 * Takes ownership of key
 */
static int wordDictInsert(
		WordDict *dict,
		char *word,
		int value)
{
	return dictInsert(
			dict,
			&word,
			&value,
			sizeof(char *),
			sizeof(int),
			(HashFunc) hashStrp,
			(CmpFunc) cmpStrp,
			(FreeFunc) freeStr,
			NULL);
}

static int wordDictPresent(WordDict const *dict, char const *key) {
	return dictPresent(dict, &key, (HashFunc) hashStrp, (CmpFunc) cmpStrp);
}

static int const *wordDictGet(WordDict const *dict, char const *key) {
	return (int const *) dictGet(
			dict,
			&key,
			(HashFunc) hashStrp,
			(CmpFunc) cmpStrp);
}

static int *wordDictGetm(WordDict *dict, char const *key) {
	return (int *) dictGetm(
			dict,
			&key,
			(HashFunc) hashStrp,
			(CmpFunc) cmpStrp);
}


static void wordDictDelete(WordDict *dict, char const *key) {
	dictDelete(
			dict,
			&key,
			(HashFunc) hashStrp,
			(CmpFunc) cmpStrp,
			(FreeFunc) freeStr,
			NULL);
}

static int wordDictRemove(WordDict *dict, char const *key) {
	return *(int *) dictRemove(
			dict,
			&key,
			(HashFunc) hashStrp,
			(CmpFunc) cmpStrp,
			(FreeFunc) freeStr);
}

static int printWordDict(WordDict const *dict) {
	return printDict(dict, (PrintFunc) printJsonStrp, (PrintFunc) printIntp);
}

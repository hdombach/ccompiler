#pragma once

#include <stdio.h>
#include <string.h>

#include "callbacks.h"
#include "dict.h"
#include "util.h"
#include "../ast/type.h"
#include "dstr.h"

typedef Dict TypeDict;

static void initTypeDict(TypeDict *dict) {
	initDict(dict);
}

static void freeTypeDict(TypeDict *dict) {
	freeDict(dict, (FreeFunc) freeStr, (FreeFunc) freeASTType);
}

/*
 * Takes ownership of key and value
 */
static int typeDictInsert(
		TypeDict *dict,
		ASTType value)
{
	if (!value.name) {
		fprintf(stderr, "INTERNAL type dict insert without name\n");
		return 0;
	}
	char *newName = strdup(value.name);
	return dictInsert(
			dict,
			&newName,
			&value,
			sizeof(char *),
			sizeof(ASTType),
			(HashFunc) hashStrp,
			(CmpFunc) cmpStrp,
			(FreeFunc) freeStr,
			(FreeFunc) freeASTType);
}

static int typeDictPresent(const Dict *dict, char const *key) {
	return dictPresent(dict, &key, (HashFunc) hashStrp, (CmpFunc) cmpStrp);
}

static ASTType const *typeDictGet(const TypeDict *dict, char const *key) {
	return (ASTType const *) dictGet(
			dict,
			&key,
			(HashFunc) hashStrp,
			(CmpFunc) cmpStrp);
}

/*
 * Note: dict does not update if you change name
 */
static ASTType *typeDictGetm(TypeDict *dict, char const *key) {
	return (ASTType *) dictGetm(
			dict,
			&key,
			(HashFunc) hashStrp,
			(CmpFunc) cmpStrp);
}

static void typeDictDelete(TypeDict *dict, char const *key) {
	dictDelete(
			dict,
			&key,
			(HashFunc) hashStrp,
			(CmpFunc) cmpStrp,
			(FreeFunc) freeStr,
			(FreeFunc) freeASTType);
}

static ASTType *typeDictRemove(TypeDict *dict, char const *key) {
	return (ASTType *) dictRemove(
			dict,
			&key,
			(HashFunc) hashStrp,
			(CmpFunc) cmpStrp,
			(FreeFunc) freeStr);
}

static int printTypeDict(TypeDict const *dict) {
	return printDict(dict, (PrintFunc) printJsonStrp, (PrintFunc) printASTType);
}

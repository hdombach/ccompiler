#pragma once

#include <string.h>
#include <stdio.h>

#include "callbacks.h"
#include "dlist.h"
#include "util.h"

typedef DList WordList;

static inline void initWorldList(WordList *list) {
	initDList(list, sizeof(char *));
}

static inline void destroyWordList(WordList *list) {
	freeDList(list, (FreeFunc) freeStr);
}

static inline char **wordListGetm(WordList *list, int index) {
	return (char **) dlistGetm(list, index);
}

static inline const char *wordListGet(WordList const *list, int index) {
	return *(char **) dlistGet(list, index);
}

static inline void wordListApp(WordList *list, char *element) {
	dlistApp(list, &element);
}

static inline DListErr wordListRem(WordList *list, int index) {
	return dlistRem(list, index, (FreeFunc) freeStr);
}

static inline DListErr wordListIns(WordList *list, char *element, int index) {
	return dlistIns(list, &element, index);
}

static int printWordList(WordList const *list) {
	return printDList(list, (PrintFunc) printStr);
}

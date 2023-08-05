#pragma once

#include <stdlib.h>

#include "dlist.h"
#include "../token.h"

typedef DList TokList;

static inline void initTokList(TokList *list) {
	initDList(list, sizeof(Token));
}

static inline void freeTokList(TokList *list) {
	freeDList(list, (DListFreeFunc) freeToken);
}

static inline Token *tokListGetm(TokList *list, int index) {
	return (Token *) dlistGetm(list, index);
}

static inline const Token *tokListGet(TokList const *list, int index) {
	return (Token *) dlistGet(list, index);
}

static inline void tokListApp(TokList *list, Token *element) {
	memcpy(tokListGetm(list, list->size - 1), element, sizeof(Token));
}

static inline DListErr tokListRem(TokList *list, int index) {
	return dlistRem(list, index, (DListFreeFunc) freeToken);
}

static inline DListErr tokListIns(TokList *list, Token *element, int index) {
	return dlistIns(list, element, index);
}

static inline void tokListRemAll(TokList *list) {
	dlistRemAll(list, (DListFreeFunc) freeToken);
}

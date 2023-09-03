#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
	dlistApp(list, element);
	//memcpy(tokListGetm(list, list->size - 1), element, sizeof(Token));
}

static inline DListErr tokListRem(TokList *list, int index) {
	return dlistRem(list, index, (DListFreeFunc) freeToken);
}

static inline void tokListRemLast(TokList *list) {
	dlistRemLast(list, (DListFreeFunc) freeToken);
}

static inline DListErr tokListIns(TokList *list, Token *element, int index) {
	return dlistIns(list, element, index);
}

static inline void tokListRemAll(TokList *list) {
	dlistRemAll(list, (DListFreeFunc) freeToken);
}

static inline void printrTokList(TokList *list) {
	int curCol, curLine = -1;
	Token *tok = tokListGetm(list, 0);
	Token *tokEnd = tokListGetm(list, list->size - 1);
	char const *content;
	for (;tok != tokEnd; tok++) {
		if (curLine != tok->posLine) {
			curLine = tok->posLine;
			printf("\n");
			for (curCol = 1; curCol < tok->posColumn; curCol++) {
				printf("\t");
			}
			//curCol = tok->posColumn;
		} else  {
			if (curCol < tok->posColumn) {
				printf(" ");
				curCol = tok->posColumn;
			}
		}

		curCol += printrToken(tok);
	}
}

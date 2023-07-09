#pragma once

#include <string.h>
#include <stdio.h>

#include "dlist.h"

typedef DList DStr;

extern inline void initDStr(DStr *dstr) {
	initDList(dstr, sizeof(char));
	dlistApp(dstr, "");
}

extern inline void freeDStr(DStr *dstr) {
	freeDList(dstr, NULL);
}

extern inline char *dstrGetm(DStr *dstr, int index) {
	return (char*) dlistGetm(dstr, index);
}

extern inline const char *dstrGet(DStr const *dstr, int index) {
	return (char*) dlistGet(dstr, index);
}

extern inline void dstrApp(DStr *dstr, char element) {
	*dstrGetm(dstr, dstr->size - 1) = element;
	dlistApp(dstr, "\0");
}

extern inline DListErr dstrRem(DStr *dstr, int index) {
	return dlistRem(dstr, index, NULL);
}

extern inline DListErr dstrIns(DStr *dstr, char element, int index) {
	return dlistIns(dstr, &element, index);
}

extern inline void dstrRemAll(DStr *dstr) {
	dlistRemAll(dstr, NULL);
	dlistApp(dstr, ""); //append \0
}

/*int dstrCmp(const DList *lhs, const DList *rhs) {
	return strcmp(lhs->data, rhs->data);
}*/

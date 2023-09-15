#pragma once

#include <string.h>
#include <stdio.h>

#include "dlist.h"

typedef DList DStr;

static inline void initDStr(DStr *dstr) {
	initDList(dstr, sizeof(char));
	dlistApp(dstr, "");
}

static inline void freeDStr(DStr *dstr) {
	freeDList(dstr, NULL);
}

static inline char *dstrGetm(DStr *dstr, int index) {
	return (char*) dlistGetm(dstr, index);
}

static inline const char *dstrGet(DStr const *dstr, int index) {
	return (char*) dlistGet(dstr, index);
}

static inline void dstrApp(DStr *dstr, char element) {
	*dstrGetm(dstr, dstr->size - 1) = element;
	dlistApp(dstr, "");
}

static inline void dstrAppStr(DStr *dstr, char const *str) {
	*dstrGetm(dstr, dstr->size - 1) = *str;
	str++;
	while (*str) {
		dlistApp(dstr, str);
		str++;
	}
	dlistApp(dstr, "");
}

static inline DListErr dstrRem(DStr *dstr, int index) {
	return dlistRem(dstr, index, NULL);
}

static inline DListErr dstrIns(DStr *dstr, char element, int index) {
	return dlistIns(dstr, &element, index);
}

static inline void dstrRemAll(DStr *dstr) {
	dlistRemAll(dstr, NULL);
	dlistApp(dstr, ""); //append \0
}



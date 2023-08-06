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
	dlistApp(dstr, "\0");
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

static inline void printJsonStr(char const *str) {
	printf("\"");
	while (*str) {
		if (*str == '\\') {
			printf("\\\\");
		} else if (*str == '"') {
			printf("\\\"");
		} else if (*str == '\n') {
			printf("\\\\n");
		} else if (*str == '\t') {
			printf("\\\\t");
		} else if (*str == '\0') {
			printf("\\\\0");
		} else if (*str == '\v') {
			printf("\\\\v");
		} else if (*str == '\r') {
			printf("\\\\r");
		} else if (*str == '\f') {
			printf("\\\\f");
		} else {
			printf("%c", *str);
		}
		str++;
	}
	printf("\"");
}

static inline void printJsonStrp(char const **str) {
	printJsonStr(*str);
}

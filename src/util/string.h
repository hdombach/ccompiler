#pragma once

#include <string.h>

#include "dlist.h"

typedef DList String;

inline void initString(String *string) {
	initDList(string, sizeof(char));
	dlistApp(string, "");
}

inline void freeString(String *string) {
	freeDList(string, NULL);
}

inline char *stringGetm(String *string, int index) {
	return (char*) dlistGetm(string, index);
}

inline const char *stringGet(String const *string, int index) {
	return (char*) dlistGet(string, index);
}

inline void stringApp(String *string, char element) {
	*stringGetm(string, string->size - 1) = element;
	dlistApp(string, "\0");
}

inline DListErr stringRem(String *string, int index) {
	return dlistRem(string, index, NULL);
}

inline DListErr stringIns(String *string, char element, int index) {
	return dlistIns(string, &element, index);
}

inline void stringRemAll(String *string) {
	dlistRemAll(string);
	dlistApp(string, ""); //append \0
}

int stringCmp(const DList *lhs, const DList *rhs) {
	return strcmp(lhs->data, rhs->data);
}

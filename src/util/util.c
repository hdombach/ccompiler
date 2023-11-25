#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "util.h"

int printStr(const char *str) {
	return printf("%s", str);
}

void freeStr(char **str) {
	if (*str) {
		free(*str);
	}
}

int cmpStr(const char *a, const char *b) {
	return 0 == strcmp(a, b);
}

int cmpStrp(const char **a, const char **b) {
	return cmpStr(*a, *b);
}

void cpStrp(char **dest, const char **src) {
	*dest = malloc(sizeof(char *));
	*dest = strdup(*src);
}

int printIntp(int const *value) {
	return printf("%d", *value);
}

int printJsonStr(char const *str) {
	int res = 0;
	if (!str) {
		res += printf("(null)");
		return res;
	}
	res += printf("\"");
	while (*str) {
		if (*str == '\\') {
			res += printf("\\\\");
		} else if (*str == '"') {
			res += printf("\\\"");
		} else if (*str == '\n') {
			res += printf("\\\\n");
		} else if (*str == '\t') {
			res += printf("\\\\t");
		} else if (*str == '\0') {
			res += printf("\\\\0");
		} else if (*str == '\v') {
			res += printf("\\\\v");
		} else if (*str == '\r') {
			res += printf("\\\\r");
		} else if (*str == '\f') {
			res += printf("\\\\f");
		} else {
			res += printf("%c", *str);
		}
		str++;
	}
	res += printf("\"");

	return res;
}

int printrStr(char const *str) {
	int res = 0;
	if (!str) {
		return res;
	}
	res += printf("\"");
	while (*str) {
		if (*str == '\\') {
			res += printf("\\\\");
		} else if (*str == '"') {
			res += printf("\\\"");
		} else if (*str == '\n') {
			res += printf("\\n");
		} else if (*str == '\t') {
			res += printf("\\t");
		} else if (*str == '\0') {
			res += printf("\\0");
		} else if (*str == '\v') {
			res += printf("\\v");
		} else if (*str == '\r') {
			res += printf("\\r");
		} else if (*str == '\f') {
			res += printf("\\f");
		} else {
			res += printf("%c", *str);
		}
		str++;
	}
	res += printf("\"");

	return res;
}

int printJsonStrp(char const **str) {
	return printJsonStr(*str);
}

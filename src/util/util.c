#include <stdlib.h>
#include <string.h>

#include "util.h"

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

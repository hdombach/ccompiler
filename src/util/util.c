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

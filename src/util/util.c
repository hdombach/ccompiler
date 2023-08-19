#include "util.h"
#include <stdlib.h>

void freeStr(char **str) {
	if (*str) {
		free(*str);
	}
}

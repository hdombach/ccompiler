#include "util.h"
#include <stdlib.h>

void freeStr(char **str) {
	free(*str);
}

#include "stages.h"
#include "lines.h"

#include <string.h>
#include <stdlib.h>

void stagesBackslashes(Lines *lines) {
	int i;
	size_t lineLen;
	char *curLine;

	i = 0;
	while (i < lines->size) {
		curLine = lines->data[i];
		lineLen = strlen(curLine);
		if (curLine[lineLen - 2] == '\\') {
			if (i >= lines->size - 1) {
				//return error
				return;
			}
			curLine[lineLen - 2] = '\0';
			curLine = realloc(curLine, lineLen + strlen(lines->data[i + 1]) - 1);
			strcat(curLine, lines->data[i + 1]);

			lines->data[i] = curLine;
		} else {
			i++;
		}
	}
}

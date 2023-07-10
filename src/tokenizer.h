#pragma once

#include "util/dlist.h"
#include "util/dstr.h"

#include <stdio.h>

typedef struct {
	int startColumn;
	int startLine;
	const char *filename;
	DStr curWord;
} TokenzState;

void initTokenzState(TokenzState *state, const char *filename);
void freeTokenzState(TokenzState *state);

DList tokenize(FILE *fp, const char *filename);


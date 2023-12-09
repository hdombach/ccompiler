#pragma once

#include "token.h"
#include "util/dlist.h"
#include "util/dstr.h"
#include "util/stream.h"
#include "util/tokList.h"

#include <stdio.h>

typedef enum {
	TOKENZ_STATE_NONE,
	TOKENZ_STATE_NUMBER,
	TOKENZ_STATE_STRING,
	TOKENZ_STATE_CHAR,
	TOKENZ_STATE_SYMBOL,
	TOKENZ_STATE_IDENTIFIER,
	TOKENZ_STATE_SINGLE_COMM,
	TOKENZ_STATE_MULTI_COMM,
	TOKENZ_STATE_MACRO,
} TokenzStateType;

typedef struct _TokenzState {
	int startColumn;
	int startLine;
	int curColumn;
	int curLine;
	char curChar;
	char wasBackslash;
	const char *filename;
	DStr curWord;
	int isMacro;
	TokenzStateType type;
	TokList tokens;
	TokenType lastSymbolType;
} TokenzState;

void initTokenzState(TokenzState *state, const char *filename);
void freeTokenzState(TokenzState *state);

TokList tokenize(Stream *stream, const char *filename);


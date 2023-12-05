#include "tokenizer.h"
#include "token.h"
#include "util/dlist.h"
#include "util/dstr.h"
#include "util/stream.h"
#include "util/tokList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *_SPECIAL_SYMB = "{}[]#()<>%:;.?*+-/^&|~!=,\"'";
const char *_NUM = "0123456789";
const char *_ALPH="_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char *_ALPH_NUM="_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const char *_WHITESPACE = " \t\v\f\n\r";
const char *_LINE_WHITESPACE = " \t";


void initTokenzState(TokenzState *state, const char *filename) {
	state->startColumn = 1;
	state->startLine = 1;
	state->curColumn = 1;
	state->curLine = 1;
	state->wasBackslash = 0;
	state->filename = filename;
	initDStr(&state->curWord);
	state->isMacro = 0;
	state->type = TOKENZ_STATE_NONE;
	initTokList(&state->tokens);
	state->lastSymbolType = TT_UNKNOWN;
}

void freeTokenzState(TokenzState *state) {
	freeDStr(&state->curWord);
	freeTokList(&state->tokens);
}

void _resetAccumulation(TokenzState *state) {
	state->startColumn = state->curColumn;
	state->startLine = state->curLine;
	state->type = TOKENZ_STATE_NONE;
	dstrRemAll(&state->curWord);
}

void _parseUnknown(TokenzState *state) {
	_resetAccumulation(state);
	state->startColumn--;
	if (state->curChar == '"') {
		state->type = TOKENZ_STATE_STRING;
	} else if (state->curChar == '\'') {
		state->type = TOKENZ_STATE_CHAR;
	} else if (state->curChar == '#') {
		state->type = TOKENZ_STATE_MACRO;
		state->isMacro = 1;
		dstrApp(&state->curWord, state->curChar);
	} else if (strchr(_NUM, state->curChar)) {
		state->type = TOKENZ_STATE_NUMBER;
		dstrApp(&state->curWord, state->curChar);
	} else if (strchr(_SPECIAL_SYMB, state->curChar)) {
		state->type = TOKENZ_STATE_SYMBOL;
		dstrApp(&state->curWord, state->curChar);
		state->lastSymbolType = findPunctuation(state->curWord.data);
	} else if (strchr(_ALPH, state->curChar)) {
		state->type = TOKENZ_STATE_IDENTIFIER;
		dstrApp(&state->curWord, state->curChar);
	}
}

void _parseNumber(TokenzState *state) {
	Token token;

	if (strchr(_ALPH_NUM, state->curChar) && state->curChar || state->curChar == '.') {
		dstrApp(&state->curWord, state->curChar);
	} else if (strchr("-+", state->curChar) && state->curChar) {
		if (state->curWord.size > 2 && *dstrGet(&state->curWord, state->curWord.size - 2) == 'e') {
			dstrApp(&state->curWord, state->curChar);
		} else {
			initNumbToken(&token, state);
			tokListApp(&state->tokens, &token);
			_parseUnknown(state);
		}
	} else {
			initNumbToken(&token, state);
			tokListApp(&state->tokens, &token);
			_parseUnknown(state);
	}
}

void _parseString(TokenzState *state) {
	Token token;

	if (state->wasBackslash) {
		if (state->curChar == '"') {
			dstrApp(&state->curWord, '"');
		} else if (state->curChar == 'n') {
			dstrApp(&state->curWord, '\n');
		} else if (state->curChar == 't') {
			dstrApp(&state->curWord, '\t');
		} else if (state->curChar == '0') {
			dstrApp(&state->curWord, '\0');
		} else if (state->curChar == 'v') {
			dstrApp(&state->curWord, '\v');
		} else if (state->curChar == 'r') {
			dstrApp(&state->curWord, '\r');
		} else if (state->curChar == 'f') {
			dstrApp(&state->curWord, '\f');
		} else if (state->curChar == '\\') {
			dstrApp(&state->curWord, '\\');
		} else {
			fprintf(stderr, "Unrecognized character after backslash: %c\n", state->curChar);
		}
	} else if (state->curChar == '"') {
		initStrToken(&token, state);
		tokListApp(&state->tokens, &token);
		_resetAccumulation(state);
	} else {
		dstrApp(&state->curWord, state->curChar);
	}
}

void _parseChar(TokenzState *state) {
	Token token;

	if (state->wasBackslash || state->curChar != '\'') {
		dstrApp(&state->curWord, state->curChar);
	} else {
		initCharToken(&token, state);
		tokListApp(&state->tokens, &token);
		_resetAccumulation(state);
	}
}

void _parseSymbol(TokenzState *state) {
	TokenType tempTokenType;
	Token token;

	if (*dstrGet(&state->curWord, 0) == '/') {
		if (state->curChar == '/') {
			state->type = TOKENZ_STATE_SINGLE_COMM;
			return;
		} else if (state->curChar == '*') {
			state->type = TOKENZ_STATE_MULTI_COMM;
			return;
		}
	}
	dstrApp(&state->curWord, state->curChar);
	tempTokenType = findPunctuation(state->curWord.data);
	if (tempTokenType != TT_UNKNOWN && state->curChar) {
		state->lastSymbolType = tempTokenType;
	} else if (strcmp(state->curWord.data, "..") == 0) {
		//This one edge case breaks the algorithm for 
		//determaining punctionation based tokens.
	} else if (state->lastSymbolType != TT_UNKNOWN) {
		initSymToken(&token, state, state->lastSymbolType);
		tokListApp(&state->tokens, &token);
		_parseUnknown(state);
	} else if (!strchr(_SPECIAL_SYMB, state->curChar)) {
		fprintf(stderr, "Unrecognized symbols %s\n", (char *) state->curWord.data);
		initSymToken(&token, state, TT_UNKNOWN);
		printToken(&token);
		tokListApp(&state->tokens, &token);
		_parseUnknown(state);
	}
}

void _parseIdentifier(TokenzState *state) {
	Token token;

	if (strchr(_ALPH_NUM, state->curChar) && state->curChar) {
		dstrApp(&state->curWord, state->curChar);
	} else {
		initIdentToken(&token, state);
		tokListApp(&state->tokens, &token);
		_parseUnknown(state);
	}
}

void _parseSingleComm(TokenzState *state) {
	if (state->curChar == '\n') {
		_resetAccumulation(state);
	}
}

void _parseMultiComm(TokenzState *state) {
	if (state->curChar == '/') {
		if (state->curWord.size > 2 && *dstrGet(&state->curWord, state->curWord.size - 2) == '*') {
			_resetAccumulation(state);
			state->wasBackslash = 0;
		}
	} else {
		dstrApp(&state->curWord, state->curChar);
	}
}

void _parseMarco(TokenzState *state) {
	Token token;

	if (strchr(_ALPH_NUM, state->curChar)) {
		dstrApp(&state->curWord, state->curChar);
	} else {
		initMacroToken(&token, state);
		tokListApp(&state->tokens, &token);
		_parseUnknown(state);
	}
}

/*
 * Generates tokens from file
 */
TokList tokenize(Stream *stream, const char *filename) {
	TokList result;
	Token token;
	TokenzState state;
	int curChar_i;

	initTokenzState(&state, filename);
	_resetAccumulation(&state);

	do {
		curChar_i = streamGetc(stream);
		state.curChar = curChar_i;
		if (state.curChar == '\n') {
			state.curLine++;
			state.curColumn = 1;
		} else {
			state.curColumn++;
		}

		if (state.wasBackslash && state.curChar == '\n') {
			state.wasBackslash = 0;
			continue;
		}
		if (state.curChar == '\\' && !state.wasBackslash) {
			state.wasBackslash = 1;
			continue;
		}

		if (state.type == TOKENZ_STATE_NUMBER) {
			_parseNumber(&state);
		} else if (state.type == TOKENZ_STATE_STRING) {
			_parseString(&state);
		} else if (state.type == TOKENZ_STATE_CHAR) {
			_parseChar(&state);
		} else if (state.type == TOKENZ_STATE_SYMBOL) {
			_parseSymbol(&state);
		} else if (state.type == TOKENZ_STATE_IDENTIFIER) {
			_parseIdentifier(&state);
		} else if (state.type == TOKENZ_STATE_SINGLE_COMM) {
			_parseSingleComm(&state);
		} else if (state.type == TOKENZ_STATE_MULTI_COMM) {
			_parseMultiComm(&state);
		} else if (state.type == TOKENZ_STATE_MACRO) {
			_parseMarco(&state);
		} else if (state.type == TOKENZ_STATE_NONE) {
			_parseUnknown(&state);
		}

		if (!state.wasBackslash && state.curChar == '\n') {
			if (state.isMacro) {
				initNewlineToken(&token, &state);
				tokListApp(&state.tokens, &token);
				state.isMacro = 0;
			}
		}

		state.wasBackslash = 0;
	} while (curChar_i > 0);

	initEOFToken(&token, &state);
	tokListApp(&state.tokens, &token);

	moveDList(&result, &state.tokens);
	freeTokenzState(&state);

	return result;
}

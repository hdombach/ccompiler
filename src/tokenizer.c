#include "tokenizer.h"
#include "token.h"
#include "util/dlist.h"
#include "util/dstr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *_SPECIAL_SYMB = "{}[]#()<>%:;.?*+-/^&|~!=,\"'";
const char *_NUM = "0123456789";
const char *_ALPH="_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char *_ALPH_NUM="_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const char *_WHITESPACE = " \t\v\f\n\r";

typedef enum {
	STATE_NONE,
	STATE_NUMBER,
	STATE_STRING,
	STATE_CHAR,
	STATE_SYMBOL,
	STATE_IDENTIFIER,
	STATE_SINGLE_COMM,
	STATE_MULTI_COMM,
	STATE_MACRO,
} _StateType;

void initTokenzState(TokenzState *state, const char *filename) {
	state->startColumn = 0;
	state->startLine = 0;
	state->filename = filename;
	initDStr(&state->curWord);
}

void freeTokenzState(TokenzState *state) {
	freeDStr(&state->curWord);
}

void _resetState(TokenzState *state, int column, int line) {
	state->startColumn = column;
	state->startLine = line;
	dstrRemAll(&state->curWord);
}

/*
 * Generates tokens from file
 */
DList tokenize(FILE *fp, const char *filename) {
	DList result;
	Token token;
	TokenzState state;
	int curColumn = 1;
	int curLine = 1;
	int curChar_i;
	char curChar;
	int wasBackslash = 0;
	TokenType lastTokType;
	_StateType stateType;

	initDList(&result, sizeof(Token));
	initTokenzState(&state, filename);
	_resetState(&state, curColumn, curLine);
	stateType = STATE_NONE;

	do {
		curChar_i = getc(fp);
		curChar = curChar_i;
		if (curChar == '\n') {
			curLine++;
			curColumn = 1;
		} else {
			curColumn++;
		}
		if (wasBackslash && curChar == '\n') {
			wasBackslash = 0;
		}
		if (curChar == '\\' && !wasBackslash) {
			wasBackslash = 1;
			continue;
		}

		if (stateType == STATE_NUMBER) {

			if (strchr(_ALPH_NUM, curChar) || curChar == '.') {
				dstrApp(&state.curWord, curChar);
			} else if (strchr("-+", curChar)) {
				if (state.curWord.size > 2 && *dstrGet(&state.curWord, state.curWord.size - 2) == 'e') {
					dstrApp(&state.curWord, curChar);
				} else {
					initNumbToken(&token, &state);
					dlistApp(&result, &token);
					_resetState(&state, curColumn, curLine);
					stateType = STATE_NONE;
				}
			} else {
					initNumbToken(&token, &state);
					dlistApp(&result, &token);
					_resetState(&state, curColumn, curLine);
					stateType = STATE_NONE;
			}

		} else if (stateType == STATE_STRING) {

			if (wasBackslash) {
				if (curChar == '"') {
					dstrApp(&state.curWord, '"');
				} else if (curChar == 'n') {
					dstrApp(&state.curWord, '\n');
				} else if (curChar == 't') {
					dstrApp(&state.curWord, '\t');
				} else if (curChar == '0') {
					dstrApp(&state.curWord, '\0');
				} else if (curChar == 'v') {
					dstrApp(&state.curWord, '\v');
				} else if (curChar == 'r') {
					dstrApp(&state.curWord, '\r');
				} else if (curChar == 'f') {
					dstrApp(&state.curWord, '\f');
				} else {
					fprintf(stderr, "Unrecosgnized character after backslash: %c\n", curChar);
				}
			} else if (curChar != '"') {
				dstrApp(&state.curWord, curChar);
			} else {
				initStrToken(&token, &state);
				dlistApp(&result, &token);
				_resetState(&state, curColumn, curLine);
				stateType = STATE_NONE;
				wasBackslash = 0;
				continue;
			}

		} else if (stateType == STATE_CHAR) {

			if (wasBackslash || curChar != '\'') {
				dstrApp(&state.curWord, curChar);
			} else {
				initCharToken(&token, &state);
				dlistApp(&result, &token);
				_resetState(&state, curColumn, curLine);
				stateType = STATE_NONE;
				wasBackslash = 0;
				continue;
			}

		} else if (stateType == STATE_SYMBOL) {

			TokenType tempTokenType;

			if (*dstrGet(&state.curWord, 0) == '/') {
				if (curChar == '/') {
					stateType = STATE_SINGLE_COMM;
					wasBackslash = 0;
					continue;
				} else if (curChar == '*') {
					stateType = STATE_MULTI_COMM;
					wasBackslash = 0;
					continue;
				}
			}
			dstrApp(&state.curWord, curChar);
			tempTokenType = findPunctuation(state.curWord.data);
			if (tempTokenType != TT_UNKNOWN) {
				lastTokType = tempTokenType;
			} else if (lastTokType != TT_UNKNOWN) {
				initToken(&token);
				token.type = lastTokType;
				token.filename = strdup(filename);
				token.posLine = state.startLine;
				token.posColumn = state.startColumn;
				dlistApp(&result, &token);
				_resetState(&state, curColumn, curLine);
				stateType = STATE_NONE;
			} else if (!strchr(_SPECIAL_SYMB, curChar)) {
				fprintf(stderr, "Unrecosgnized symbole %s\n", (char *) state.curWord.data);
				initToken(&token);
				token.type = TT_UNKNOWN;
				dlistApp(&result, &token);
				_resetState(&state, curColumn, curLine);
				stateType = STATE_NONE;
			}
			
		} else if (stateType == STATE_IDENTIFIER) {

			if (strchr(_ALPH_NUM, curChar)) {
				dstrApp(&state.curWord, curChar);
			} else {
				initIdentToken(&token, &state);
				dlistApp(&result, &token);
				_resetState(&state, curColumn, curLine);
				stateType = STATE_NONE;
			}

		} else if (stateType == STATE_SINGLE_COMM) {

			if (curChar == '\n') {
				stateType = STATE_NONE;
				_resetState(&state, curColumn, curLine);
			}

		} else if (stateType == STATE_MULTI_COMM) {

			if (curChar == '/') {
				if (state.curWord.size > 2 && *dstrGet(&state.curWord, state.curWord.size - 2) == '*') {
					_resetState(&state, curColumn, curLine);
					stateType = STATE_NONE;
					wasBackslash = 0;
					continue;
				}
			} else {
				dstrApp(&state.curWord, curChar);
			}

		} else if (stateType == STATE_MACRO) {

			if (strchr(_ALPH_NUM, curChar)) {
				dstrApp(&state.curWord, curChar);
			} else {
				initMacroToken(&token, &state);
				dlistApp(&result, &token);
				_resetState(&state, curColumn, curLine);
				stateType = STATE_NONE;
			}
		}

		if (stateType == STATE_NONE) {
			_resetState(&state, curColumn - 1, curLine);
			if (curChar == '"') {
				stateType = STATE_STRING;
			} else if (curChar == '\'') {
				stateType = STATE_CHAR;
			} else if (curChar == '#') {
				stateType = STATE_MACRO;
				dstrApp(&state.curWord, curChar);
			} else if (strchr(_NUM, curChar)) {
				stateType = STATE_NUMBER;
				dstrApp(&state.curWord, curChar);
			} else if (strchr(_SPECIAL_SYMB, curChar)) {
				stateType = STATE_SYMBOL;
				dstrApp(&state.curWord, curChar);
				lastTokType = findPunctuation(state.curWord.data);
			} else if (strchr(_ALPH, curChar)) {
				stateType = STATE_IDENTIFIER;
				dstrApp(&state.curWord, curChar);
			}
		}


		wasBackslash = 0;
	} while (curChar_i != EOF);

	freeTokenzState(&state);

	return result;
}

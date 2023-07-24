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
const char *_LINE_WHITESPACE = " \t";


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
	state->type = TOKENZ_STATE_NONE;
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

	initDList(&result, sizeof(Token));
	initTokenzState(&state, filename);
	_resetState(&state, curColumn, curLine);

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
			continue;
		}

		if (!wasBackslash && curChar == '\n') {
			state.isMacro = 0;
		}

		if (curChar == '\\' && !wasBackslash) {
			wasBackslash = 1;
			continue;
		}

		if (state.type == TOKENZ_STATE_NUMBER) {

			if (strchr(_ALPH_NUM, curChar) || curChar == '.') {
				dstrApp(&state.curWord, curChar);
			} else if (strchr("-+", curChar)) {
				if (state.curWord.size > 2 && *dstrGet(&state.curWord, state.curWord.size - 2) == 'e') {
					dstrApp(&state.curWord, curChar);
				} else {
					initNumbToken(&token, &state);
					dlistApp(&result, &token);
					_resetState(&state, curColumn, curLine);
				}
			} else {
					initNumbToken(&token, &state);
					dlistApp(&result, &token);
					_resetState(&state, curColumn, curLine);
			}

		} else if (state.type == TOKENZ_STATE_STRING) {

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
					fprintf(stderr, "Unrecognized character after backslash: %c\n", curChar);
				}
			} else if (curChar != '"') {
				dstrApp(&state.curWord, curChar);
			} else {
				initStrToken(&token, &state);
				dlistApp(&result, &token);
				_resetState(&state, curColumn, curLine);
				wasBackslash = 0;
				continue;
			}

		} else if (state.type == TOKENZ_STATE_CHAR) {

			if (wasBackslash || curChar != '\'') {
				dstrApp(&state.curWord, curChar);
			} else {
				initCharToken(&token, &state);
				dlistApp(&result, &token);
				_resetState(&state, curColumn, curLine);
				wasBackslash = 0;
				continue;
			}

		} else if (state.type == TOKENZ_STATE_SYMBOL) {

			TokenType tempTokenType;

			if (*dstrGet(&state.curWord, 0) == '/') {
				if (curChar == '/') {
					state.type = TOKENZ_STATE_SINGLE_COMM;
					wasBackslash = 0;
					continue;
				} else if (curChar == '*') {
					state.type = TOKENZ_STATE_MULTI_COMM;
					wasBackslash = 0;
					continue;
				}
			}
			dstrApp(&state.curWord, curChar);
			tempTokenType = findPunctuation(state.curWord.data);
			if (tempTokenType != TT_UNKNOWN) {
				lastTokType = tempTokenType;
			} else if (strcmp(state.curWord.data, "..") == 0) {
				//This one edge case breaks the algorithm for 
				//determaining punctionation based tokens.
			} else if (lastTokType != TT_UNKNOWN) {
				initSymToken(&token, &state, lastTokType);
				dlistApp(&result, &token);
				_resetState(&state, curColumn, curLine);
			} else if (!strchr(_SPECIAL_SYMB, curChar)) {
				fprintf(stderr, "Unrecognized symbols %s\n", (char *) state.curWord.data);
				initSymToken(&token, &state, TT_UNKNOWN);
				printToken(&token);
				dlistApp(&result, &token);
				_resetState(&state, curColumn, curLine);
			}
			
		} else if (state.type == TOKENZ_STATE_IDENTIFIER) {

			if (strchr(_ALPH_NUM, curChar)) {
				dstrApp(&state.curWord, curChar);
			} else {
				initIdentToken(&token, &state);
				dlistApp(&result, &token);
				_resetState(&state, curColumn, curLine);
			}

		} else if (state.type == TOKENZ_STATE_SINGLE_COMM) {

			if (curChar == '\n') {
				_resetState(&state, curColumn, curLine);
			}

		} else if (state.type == TOKENZ_STATE_MULTI_COMM) {

			if (curChar == '/') {
				if (state.curWord.size > 2 && *dstrGet(&state.curWord, state.curWord.size - 2) == '*') {
					_resetState(&state, curColumn, curLine);
					wasBackslash = 0;
					continue;
				}
			} else {
				dstrApp(&state.curWord, curChar);
			}

		} else if (state.type == TOKENZ_STATE_MACRO) {

			if (strchr(_ALPH_NUM, curChar)) {
				dstrApp(&state.curWord, curChar);
			} else {
				initMacroToken(&token, &state);
				dlistApp(&result, &token);
				_resetState(&state, curColumn, curLine);
			}
		}

		if (state.type == TOKENZ_STATE_NONE) {
			_resetState(&state, curColumn - 1, curLine);
			if (curChar == '"') {
				state.type = TOKENZ_STATE_STRING;
			} else if (curChar == '\'') {
				state.type = TOKENZ_STATE_CHAR;
			} else if (curChar == '#') {
				state.type = TOKENZ_STATE_MACRO;
				state.isMacro = 1;
				dstrApp(&state.curWord, curChar);
			} else if (strchr(_NUM, curChar)) {
				state.type = TOKENZ_STATE_NUMBER;
				dstrApp(&state.curWord, curChar);
			} else if (strchr(_SPECIAL_SYMB, curChar)) {
				state.type = TOKENZ_STATE_SYMBOL;
				dstrApp(&state.curWord, curChar);
				lastTokType = findPunctuation(state.curWord.data);
			} else if (strchr(_ALPH, curChar)) {
				state.type = TOKENZ_STATE_IDENTIFIER;
				dstrApp(&state.curWord, curChar);
			}
		}


		wasBackslash = 0;
	} while (curChar_i != EOF);

	freeTokenzState(&state);

	return result;
}

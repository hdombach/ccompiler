#include "tokenizer.h"
#include "token.h"
#include "util/dlist.h"
#include "util/dstr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//TODO: prob return #
const char *_SPECIAL_SYMB = "{}[]#()<>%:;.?*+-/^&|~!=,\"'#";
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
} _State;

DList tokenize(FILE *fp) {
	DList result;
	DStr curWord;
	Token token;
	int curColumn = 0;
	int curLine = 0;
	int curChar_i;
	char curChar;
	int wasBackslash = 0;
	TokenType lastTokType;
	_State state;

	initDList(&result, sizeof(Token));
	initDStr(&curWord);
	state = STATE_NONE;

	do {
		curChar_i = getc(fp);
		curChar = curChar_i;
		if (curChar == '\n') {
			curLine++;
			curColumn = 0;
		} else {
			curColumn++;
		}
		if (wasBackslash && curChar == '\n') {
			wasBackslash = 0;
		}
		if (curChar == '\\') {
			wasBackslash = 1;
			continue;
		} else if (curChar == '#') {
			wasBackslash = 0;
			continue;
			//TODO: get rid of this
		}

		if (state == STATE_NUMBER) {
			if (strchr(_ALPH_NUM, curChar) || curChar == '.') {
				dstrApp(&curWord, curChar);
			} else if (strchr("-+", curChar)) {
				if (curWord.size > 2 && *dstrGet(&curWord, curWord.size - 2) == 'e') {
					dstrApp(&curWord, curChar);
				} else {
					initNumbToken(&token, curWord.data);
					dlistApp(&result, &token);
					dstrRemAll(&curWord);
					state = STATE_NONE;
				}
			} else {
					initNumbToken(&token, curWord.data);
					dlistApp(&result, &token);
					dstrRemAll(&curWord);
					state = STATE_NONE;
			}
		} else if (state == STATE_STRING) {
			if (wasBackslash || curChar != '"') {
				dstrApp(&curWord, curChar);
			} else {
				initStrToken(&token, curWord.data);
				dlistApp(&result, &token);
				dstrRemAll(&curWord);
				state = STATE_NONE;
				wasBackslash = 0;
				continue;
			}
		} else if (state == STATE_CHAR) {
			if (wasBackslash || curChar != '\'') {
				dstrApp(&curWord, curChar);
			} else {
				initCharToken(&token, curWord.data);
				dlistApp(&result, &token);
				dstrRemAll(&curWord);
				state = STATE_NONE;
			}
		} else if (state == STATE_SYMBOL) {
			TokenType tempTokenType;
			dstrApp(&curWord, curChar);
			tempTokenType = findPunctuation(curWord.data);
			if (tempTokenType != TT_UNKNOWN) {
				lastTokType = tempTokenType;
			} else if (lastTokType != TT_UNKNOWN) {
				initToken(&token);
				token.type = lastTokType;
				dlistApp(&result, &token);
				dstrRemAll(&curWord);
				state = STATE_NONE;
			} else if (!strchr(_SPECIAL_SYMB, curChar)) {
				fprintf(stderr, "Unhandled error\n");
				initToken(&token);
				token.type = TT_UNKNOWN;
				token.contents = strdup(curWord.data);
				dlistApp(&result, &token);
				dstrRemAll(&curWord);
				state = STATE_NONE;

				//TODO error handling
			}
			
		} else if (state == STATE_IDENTIFIER) {
			if (strchr(_ALPH_NUM, curChar)) {
				dstrApp(&curWord, curChar);
			} else {
				initIdentToken(&token, curWord.data);
				dlistApp(&result, &token);
				dstrRemAll(&curWord);
				state = STATE_NONE;
			}
		} else if (state == STATE_SINGLE_COMM) {
			if (curChar == '\n') {
				state = STATE_NONE;
			}
		} else if (state == STATE_MULTI_COMM) {
			if (curChar == '/') {
				if (curWord.size > 2 && *dstrGet(&curWord, curWord.size - 2) == '*') {
					state = STATE_NONE;
					wasBackslash = 0;
					continue;
				}
			}
		}

		if (state == STATE_NONE) {
			if (curChar == '"') {
				state = STATE_STRING;
			} else if (curChar == '\'') {
				state = STATE_CHAR;
			} else if (strchr(_NUM, curChar)) {
				state = STATE_NUMBER;
				dstrApp(&curWord, curChar);
			} else if (strchr(_SPECIAL_SYMB, curChar)) {
				state = STATE_SYMBOL;
				dstrApp(&curWord, curChar);
				lastTokType = findPunctuation(curWord.data);
			} else if (strchr(_ALPH, curChar)) {
				state = STATE_IDENTIFIER;
				dstrApp(&curWord, curChar);
			}
		}


		wasBackslash = 0;
	} while (curChar_i != EOF);

	return result;
}

#include "tokenizer.h"
#include "token.h"
#include "util/dlist.h"
#include "util/string.h"

#include <stdio.h>
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
} _State;

DList tokenize(FILE *fp) {
	DList result;
	String curWord;
	Token token;
	int curColumn = 0;
	int curLine = 0;
	int curChar_i;
	char curChar;
	int wasBackslash = 0;
	TokenType lastTokType;
	_State state;

	initDList(&result, sizeof(Token));
	initString(&curWord);

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
		}

		if (state == STATE_NUMBER) {
			if (strchr(_ALPH_NUM, curChar) || curChar == '.') {
				stringApp(&curWord, curChar);
			} else if (strchr("-+", curChar)) {
				if (curWord.size > 2 && *stringGet(&curWord, curWord.size - 2) == 'e') {
					stringApp(&curWord, curChar);
				} else {
					initNumbToken(&token, curWord.data);
					dlistApp(&result, &token);
					dlistRemAll(&result);
					state = STATE_NONE;
				}
			} else {
					initNumbToken(&token, curWord.data);
					dlistApp(&result, &token);
					stringRemAll(&curWord);
					state = STATE_NONE;
			}
		} else if (state == STATE_STRING) {
			if (wasBackslash || curChar != '"') {
				stringApp(&curWord, curChar);
			} else {
				initStrToken(&token, curWord.data);
				dlistApp(&result, &token);
				stringRemAll(&curWord);
				state = STATE_NONE;
			}
		} else if (state == STATE_CHAR) {
			if (wasBackslash || curChar != '\'') {
				stringApp(&curWord, curChar);
			} else {
				initCharToken(&token, curWord.data);
				dlistApp(&result, &token);
				stringRemAll(&curWord);
				state = STATE_NONE;
			}
		} else if (state == STATE_SYMBOL) {
			TokenType tempTokenType;
			stringApp(&curWord, curChar);
			tempTokenType = findPunctuation(curWord.data);
			if (tempTokenType != TT_UNKNOWN) {
				lastTokType = tempTokenType;
			} else if (lastTokType != TT_UNKNOWN) {
				initSymToken(&token, curWord.data);
				dlistApp(&result, &token);
				stringRemAll(&curWord);
			} else if (!strchr(_SPECIAL_SYMB, curChar)) {
				//TODO error handling
			}
			
		} else if (state == STATE_IDENTIFIER) {
			if (strchr(_ALPH_NUM, curChar)) {
				stringApp(&curWord, curChar);
			} else {
				initIdentToken(&token, curWord.data);
				dlistApp(&result, &token);
				stringRemAll(&curWord);
			}
		} else if (state == STATE_SINGLE_COMM) {
			if (curChar == '\n') {
				state = STATE_NONE;
			}
		} else if (state == STATE_MULTI_COMM) {
			if (curChar == '/') {
				if (curWord.size > 2 && *stringGet(&curWord, curWord.size - 2) == '*') {
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
				stringApp(&curWord, curChar);
			} else if (strchr(_SPECIAL_SYMB, curChar)) {
				state = STATE_SYMBOL;
				stringApp(&curWord, curChar);
			} else if (strchr(_ALPH, curChar)) {
				state = STATE_IDENTIFIER;
				stringApp(&curWord, curChar);
			}
		}


		wasBackslash = 0;
	} while (curChar_i != EOF);

	return result;
}

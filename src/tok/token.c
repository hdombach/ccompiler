#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token.h"
#include "tokenizer.h"
#include "../util/log.h"
#include "../util/util.h"

void initToken(Token *token) {
	token->type = TT_UNKNOWN;
	token->contents = NULL;
	token->posLine = 0;
	token->posColumn = 0;
	token->filename = NULL;
	token->isMacro = 0;
}

void freeToken(Token *token) {
	if (token->contents) {
		free(token->contents);
	}
	if (token->filename) {
		free(token->filename);
	}
}

void initNumbToken(Token *token, const TokenzState *state) {
	initToken(token);
	token->type = TT_NUMB_CONSTANT;
	token->contents = strdup(state->curWord.data);
	token->posColumn = state->startColumn;
	token->posLine = state->startLine;
	token->filename = strdup(state->filename);
	token->isMacro = state->isMacro;
}

void initStrToken(Token *token, const TokenzState *state) {
	initToken(token);
	token->type = TT_STR_CONSTANT;
	token->contents = strdup(state->curWord.data);
	token->posColumn = state->startColumn;
	token->posLine = state->startLine;
	token->filename = strdup(state->filename);
	token->isMacro = state->isMacro;
}

void initCharToken(Token *token, const TokenzState *state) {
	initToken(token);
	token->type = TT_CHAR_CONSTANT;
	token->contents = strdup(state->curWord.data);
	token->posColumn = state->startColumn;
	token->posLine = state->startLine;
	token->filename = strdup(state->filename);
	token->isMacro = state->isMacro;

}

void initIdentToken(Token *token, const TokenzState *state) {
	TokenType type;

	initToken(token);
	type = findKeyword(state->curWord.data);
	if (type == TT_UNKNOWN) {
		token->type = TT_IDENTIFIER;
		token->contents = strdup(state->curWord.data);
	} else {
		token->type = type;
	}
	token->posColumn = state->startColumn;
	token->posLine = state->startLine;
	token->filename = strdup(state->filename);
	token->isMacro = state->isMacro;

}

void initSymToken(Token *token, const TokenzState *state, TokenType type) {
	initToken(token);
	token->type = type;
	if (type == TT_UNKNOWN) {
		token->contents = strdup(state->curWord.data);
	}
	token->posColumn = state->startColumn;
	token->posLine = state->startLine;
	token->filename = strdup(state->filename);
	token->isMacro = state->isMacro;

}

void initMacroToken(Token *token, const TokenzState *state) {

	initToken(token);
	token->posColumn = state->startColumn;
	token->posLine = state->startLine;
	token->filename = strdup(state->filename);
	token->isMacro = state->isMacro;

	token->type = findMacro(state->curWord.data);
	if (token->type == TT_UNKNOWN) {
		logCerr(CERR_TOKENIZER, NULL, "Unrecognized macro %s", state->curWord.data);
		return;
	}
}

void initEOFToken(Token *token, const struct _TokenzState *state) {
	initToken(token);
	token->posColumn = state->startColumn;
	token->posLine = state->startLine;
	token->filename = strdup(state->filename);
	token->isMacro = 0;
	token->type = TT_EOF;
}

void initNewlineToken(Token *token, const struct _TokenzState *state) {
	initToken(token);
	token->posColumn = state->startColumn;
	token->posLine = state->startLine;
	token->filename = strdup(state->filename);
	token->isMacro = 0;
	token->type = TT_NEWLINE;
}

void cpToken(Token *dest, Token const *tok) {
	dest->type = tok->type;
	if (tok->contents) {
		dest->contents = strdup(tok->contents);
	} else {
		dest->contents = NULL;
	}
	dest->posLine = tok->posLine;
	dest->posColumn = tok->posColumn;
	if (tok->filename) {
		dest->filename = strdup(tok->filename);
	} else {
		dest->filename = NULL;
	}
	dest->isMacro = tok->isMacro;
}

Token *dupToken(Token const *token) {
	Token *result = malloc(sizeof(Token));
	cpToken(result, token);
	return result;
}

int tokenBracketDepth(TokenType type) {
	switch (type) {
		case TT_O_PARAN:
		case TT_O_BRACE:
		case TT_O_CURLY:
			return 1;
		case TT_C_PARAN:
		case TT_C_BRACE:
		case TT_C_CURLY:
			return -1;
		default:
			return 0;
	}
}

int printToken(Token *token) {
	int n = 0;
	n += printf("{\"type\": \"%s\"", tokTypeStr(token->type));
	if (token->contents) {
		n += printf(", \"contents\": ");
		n += printJsonStr(token->contents);
	}

	if (token->isMacro) {
		n += printf(", \"isMacro\": \"true\"");
	}

	n += printf(", \"position\": \"");
	if (token->filename) {
		n += printf("%s:", token->filename);
	}
	n += printf("%d:%d\"}", token->posLine, token->posColumn);
	return n;
}

int printrToken(Token *token) {
	int res = 0;

	switch (token->type) {
		case TT_IDENTIFIER:
			res += printf("%s", token->contents);
			break;
		case TT_NUMB_CONSTANT:
			res += printf("%s", token->contents);
			break;
		case TT_CHAR_CONSTANT:
			res += printf("'");
			res += printf("%s", token->contents);
			res += printf("'");
			break;
		case TT_STR_CONSTANT:
			res += printrStr(token->contents);
			break;
		case TT_EOF:
			res += printf("//EOF");
			break;
		case TT_NEWLINE:
			break;
		default:
			res += printf("%s", tokTypeStr(token->type));
			break;
	}
	if (token->type == TT_MACRO_ENDIF) {
		res += printf(" line: %d", token->posLine);
	}

	return res;
}

#define U(NAME, STR)
#define X(NAME, STR) STR,

const char * TT_STRS[] = {
	X_TOKEN_TYPE
};

#undef X
#undef U

const char *tokTypeStr(TokenType type) {
	return TT_STRS[type];
}

TokenType findKeyword(const char *word) {
	for (TokenType type = TT_AUTO; type <= TT_WHILE; type++) {
		if (strcmp(tokTypeStr(type), word) == 0) {
			return type;
		}
	}
	return TT_UNKNOWN;
}

TokenType findPunctuation(const char *symb) {
	for (TokenType type = TT_O_CURLY; type <= TT_COMMA; type++) {
		if (strcmp(tokTypeStr(type), symb) == 0) {
			return type;
		}
	}
	return TT_UNKNOWN;
}

TokenType findMacro(const char *word) {
	for (TokenType type = TT_MACRO_IF; type <= TT_MACRO_DEFINED; type++) {
		if (strcmp(tokTypeStr(type), word) == 0) {
			return type;
		}
	}
	return TT_UNKNOWN;
}

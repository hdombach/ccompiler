#include "tokenParser.h"
#include <stdio.h>

enum {
	_ERROR_MSG_SIZE = 256,
};
char _errorMsg[_ERROR_MSG_SIZE];

TPResult tpError(const Token *tok, const char *msg) {
	snprintf(
			_errorMsg,
			_ERROR_MSG_SIZE,
			"%d:%d, %s",
			tok->posLine,
			tok->posColumn,
			msg);

	return TP_ERROR;
}

int tpMacroType(const Token *tok, TokenType type) {
	return tok->isMacro && tok->type == type;
}

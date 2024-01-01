#pragma once

#include "../tok/token.h"

typedef enum {
	TP_FAILED = 0,
	TP_ERROR = -1,
} TPResult;


TPResult tpError(Token const *tok, const char *msg);
const char *tpErrorMsg();
int tpMacroType(Token const *tok, TokenType type);

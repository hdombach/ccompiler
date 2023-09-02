#pragma once

#include "../token.h"
#include <stdio.h>

enum {
	AST_STATE_MSG_S = 256 
};

static char astErrMsgBuf[AST_STATE_MSG_S];
static char *astErrMsg = NULL;


int astMacro(Token const *tok, TokenType type);

#pragma once

#include "../token.h"
#include <stdio.h>

enum {
	AST_ERR_MSG_S = 256 
};

extern char astErrMsgBuf[AST_ERR_MSG_S];
extern char *astErrMsg;


int astMacro(Token const *tok, TokenType type);

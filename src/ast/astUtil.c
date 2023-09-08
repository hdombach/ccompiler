#include "astUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char astErrMsgBuf[AST_ERR_MSG_S];
char *astErrMsg = NULL;

int astMacro(Token const *tok, TokenType type) {
	return tok->isMacro && tok->type == type;
}

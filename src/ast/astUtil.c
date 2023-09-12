#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "astUtil.h"
#include "../token.h"

char astErrMsgBuf[AST_ERR_MSG_S];
char *_astErrMsg = NULL;
Token const *_astErrTok;

void astErr(char *msg, const Token *tok) {
	_astErrMsg = msg;
	_astErrTok = tok;
}

int astHasErr() {
	return _astErrMsg != NULL;
}

int fprintASTErr(FILE *fp) {
	if (_astErrTok) {
		return fprintf(fp, "%d:%d, %s\n", _astErrTok->posLine, _astErrTok->posColumn, _astErrMsg);
	} else {
		return fprintf(fp, "%s\n", _astErrMsg);
	}
}

int astMacro(Token const *tok, TokenType type) {
	return tok->isMacro && tok->type == type;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "astUtil.h"
#include "../tok/token.h"
#include "../util/log.h"

int astHasErr() {
	return cerrCount() > 0;
}

int astMacro(Token const *tok, TokenType type) {
	return tok->isMacro && tok->type == type;
}

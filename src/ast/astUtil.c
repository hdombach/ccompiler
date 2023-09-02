#include "astUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int astMacro(Token const *tok, TokenType type) {
	return tok->isMacro && tok->type == type;
}

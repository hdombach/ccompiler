#pragma once

#include "../tok/token.h"

int astHasErr();
int astMacro(Token const *tok, TokenType type);

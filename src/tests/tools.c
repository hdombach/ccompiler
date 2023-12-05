#include "tools.h"
#include "../util/dlist.h"
#include "../util/stream.h"
#include "../tokenizer.h"
#include "../token.h"
#include "test.h"
#include <stdio.h>

void tTokensSuccess(const char *code, TokenType *types) {
	Stream stream;
	initStreamStr(&stream, code);
	DList tokens = tokenize(&stream, "UNKNOWN");
	char msg[256];

	for (int i = 0; i < tokens.size; i++) {
		TokenType t = *(TokenType *) dlistGetm(&tokens, i);
		sprintf(msg, "%s != %s: \"%s\"", tokTypeStr(t), tokTypeStr(types[i]), code);
		tAssert(msg, types[i] == t);
	}
}

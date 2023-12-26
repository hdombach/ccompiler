#include <stdio.h>

#include "tools.h"
#include "../util/dlist.h"
#include "../util/stream.h"
#include "../tokenizer.h"
#include "../token.h"
#include "../ast/file.h"
#include "test.h"

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

void tTokensFailed(const char *code, CError *errors) {
	initCerr();
	cerrDisablePrint();
	Stream stream;
	initStreamStr(&stream, code);
	DList tokens = tokenize(&stream, "UNKNOWN");
	char msg[256];

	for (int i = 0; i < cerrCount(); i++) {
		snprintf(msg, sizeof(msg), "%s != %s: %s", cerrStr(errors[i]), cerrStr(getCerr()[i]), code);
		tAssert(msg, errors[i] == getCerr()[i]);
		if (getCerr()[i] == CERR_UNKNOWN || errors[i] == CERR_UNKNOWN) break;
	}
}

static void _tNode(ASTNode *node, ASTNodeType **types) {
	tAssert("", node->type == **types);
	(*types)++;
}

void tAstSuccess(const char *code, ASTNodeType *types) {
	Stream stream;
	initStreamStr(&stream, code);
	DList tokens = tokenize(&stream, "UNKNOWN");
	ASTFile astFile;

	if (parseASTFile(&astFile, tokListGetm(&tokens, 0))) {
		//astNodeTrav((ASTNode *) &astFile, ASTTravFunc beforeFunc, ASTTravFunc afterFunc, ASTTravCtx *parent)
	}
}

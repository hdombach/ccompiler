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
		T_ASSERT(msg, types[i] == t);
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
		T_ASSERT(msg, errors[i] == getCerr()[i]);
		if (getCerr()[i] == CERR_UNKNOWN || errors[i] == CERR_UNKNOWN) break;
	}
}

static void _tNode(ASTNode *node, ASTTravCtx *ctx) {
	ASTNodeType **types = (ASTNodeType **) ctx->customCtx;
	char msg[256];
	snprintf(msg, sizeof(msg), "%s != %s", astNodeTypeStr(node->type), astNodeTypeStr(**types));
	T_ASSERT(msg, node->type == **types);
	(*types)++;
}

void tAstSuccess(const char *code, ASTNodeType *types) {
	Stream stream;
	initStreamStr(&stream, code);
	DList tokens = tokenize(&stream, "UNKNOWN");
	ASTFile astFile;

	ASTNodeType **curType = &types;
	if (parseASTFile(&astFile, tokListGetm(&tokens, 0))) {
		astNodeTrav((ASTNode *) &astFile, (ASTTravFunc) _tNode, NULL, curType);
	} else {
		T_ASSERT("parseASTFile failed", 0);
	}
}

void tAstFailed(const char *code, CError *errors) {
	initCerr();
	cerrDisablePrint();
	Stream stream;
	initStreamStr(&stream, code);
	DList tokens = tokenize(&stream, "UNKNOWN");
	ASTFile file;
	parseASTFile(&file, tokListGetm(&tokens, 0));

	char msg[256];
	for (int i = 0; i < cerrCount(); i++) {
		snprintf(msg, sizeof(msg), "%s != %s: %s", cerrStr(errors[i]), cerrStr(getCerr()[i]), code);
		T_ASSERT(msg, errors[i] == getCerr()[i]);
		if (getCerr()[i] == CERR_UNKNOWN || errors[i] == CERR_UNKNOWN) break;
	}
}

static void _debugNode(ASTNode *node, ASTTravCtx *ctx) {
	printf(" \"%s\"", astNodeTypeStr(node->type));
}

void tAstDebug(const char *code) {
	Stream stream;
	initStreamStr(&stream, code);
	DList tokens = tokenize(&stream, "UNKNOWN");
	ASTFile astFile;

	if (parseASTFile(&astFile, tokListGetm(&tokens, 0))) {
		printf("nodes:");
		astNodeTrav((ASTNode *) &astFile, (ASTTravFunc) _debugNode, NULL, NULL);
		printf("\n");
	} else {
		T_ASSERT("parseASTFile failed", 0);
	}
}

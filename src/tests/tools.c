#include <stdio.h>

#include "tools.h"
#include "../util/dlist.h"
#include "../util/stream.h"
#include "../tok/tokenizer.h"
#include "../tok/token.h"
#include "../ast/file.h"
#include "test.h"
#include "../sem/typeGen.h"

void tTokensSuccess(const char *code, TokenType *types) {
	Stream stream;
	initCerr();
	initStreamStr(&stream, code);
	DList tokens = tokenize(&stream, "UNKNOWN");
	char msg[256];

	for (int i = 0; i < tokens.size; i++) {
		TokenType t = *(TokenType *) dlistGetm(&tokens, i);
		sprintf(msg, "%s != %s: \"%s\"", tokTypeStr(t), tokTypeStr(types[i]), code);
		T_ASSERT(msg, types[i] == t);
	}

	freeDList(&tokens, (FreeFunc) freeToken);
}

void tTokensFailed(const char *code, CError *errors) {
	Stream stream;
	initCerr();
	initStreamStr(&stream, code);
	DList tokens = tokenize(&stream, "UNKNOWN");
	char msg[256];

	for (int i = 0; i < cerrCount(); i++) {
		snprintf(msg, sizeof(msg), "%s != %s: %s", cerrStr(errors[i]), cerrStr(getCerr()[i]), code);
		T_ASSERT(msg, errors[i] == getCerr()[i]);
		if (getCerr()[i] == CERR_UNKNOWN || errors[i] == CERR_UNKNOWN) break;
	}

	freeDList(&tokens, (FreeFunc) freeToken);
}

void tTokensDebug(const char *code) {
	Stream stream;
	initCerr();
	initStreamStr(&stream, code);
	DList tokens = tokenize(&stream, "UNKNOWN");
	char msg[256];

	printf("tokens:");
	for (int i = 0; i < tokens.size; i++) {
		Token *tok = dlistGetm(&tokens, i);
		printf(" \"%s\"", tokTypeStr(tok->type));
	}
	printf("\n");
}

static ASTTravRes _tNode(ASTNode *node, ASTTravCtx *ctx) {
	ASTNodeType **types = (ASTNodeType **) ctx->customCtx;
	char msg[256];
	snprintf(msg, sizeof(msg), "%s != %s", astNodeTypeStr(node->type), astNodeTypeStr(**types));
	tAssert(NULL, -1, msg, node->type == **types, NULL);
	(*types)++;
	return ASTT_SUCCESS;
}

void tAstSuccess(const char *code, ASTNodeType *types) {
	Stream stream;
	initCerr();
	initStreamStr(&stream, code);
	DList tokens = tokenize(&stream, "UNKNOWN");
	ASTFile astFile;

	ASTNodeType **curType = &types;
	if (parseASTFile(&astFile, tokListGetm(&tokens, 0))) {
		astNodeTrav((ASTNode *) &astFile, (ASTTravFunc) _tNode, NULL, curType);
		freeASTFile(&astFile);
	} else {
		tAssert(NULL, -1, "parseASTFile failed", 0, NULL);
	}
	freeDList(&tokens, (FreeFunc) freeToken);
}

void tAstFailed(const char *code, CError *errors) {
	Stream stream;
	initCerr();
	initStreamStr(&stream, code);
	DList tokens = tokenize(&stream, "UNKNOWN");
	ASTFile file;
	if (parseASTFile(&file, tokListGetm(&tokens, 0))) {
		freeASTFile(&file);
	}

	char msg[256];
	for (int i = 0; i < cerrCount(); i++) {
		snprintf(msg, sizeof(msg), "%s != %s: %s", cerrStr(errors[i]), cerrStr(getCerr()[i]), code);
		T_ASSERT(msg, errors[i] == getCerr()[i]);
		if (getCerr()[i] == CERR_UNKNOWN || errors[i] == CERR_UNKNOWN) break;
	}

	freeDList(&tokens, (FreeFunc) freeToken);
}

static void _debugNode(ASTNode *node, ASTTravCtx *ctx) {
	printf(" \"%s\"", astNodeTypeStr(node->type));
}

void tAstDebug(const char *code) {
	Stream stream;
	initCerr();
	initStreamStr(&stream, code);
	DList tokens = tokenize(&stream, "UNKNOWN");
	ASTFile astFile;

	if (parseASTFile(&astFile, tokListGetm(&tokens, 0))) {
		printf("nodes:");
		astNodeTrav((ASTNode *) &astFile, (ASTTravFunc) _debugNode, NULL, NULL);
		printf("\n");
	} else {
		INT_ERROR("parseASTFile failed");
		int i = 0;
		fprintf(stdout, "Errors: [");
		while (1) {
			fprintf(stdout, "%s", cerrStr(getCerr()[i]));
			i++;
			if (i >= cerrCount()) break;
			fprintf(stdout, ", ");
		}
		fprintf(stdout, "]\n");
	}
}

static ASTTravRes _tLabel(ASTNode *node, ASTTravCtx *ctx) {
	if (!astNodeHasScope(node)) {
		return ASTT_SUCCESS;
	}

	char **labels = (char **) ctx->customCtx;
	WordDict *labelDict = &ctx->scope->labelDict;
	for (int i = 0; i < labelDict->elementCount; i++) {
		char msg[256];
		snprintf(msg, sizeof(msg), "missing label: \"%s\"", *labels);
		T_ASSERT(msg, wordDictPresent(labelDict, *labels));
		(labels)++;
	}
	ctx->customCtx = labels;
	return ASTT_SUCCESS;
}

void tTypeGenLabels(const char *code, const char **labels) {
	Stream stream;
	initCerr();
	initStreamStr(&stream, code);
	DList tokens = tokenize(&stream, "UNKNOWN");
	ASTFile astFile;

	T_ASSERT("parseASTFile failed", parseASTFile(&astFile, tokListGetm(&tokens, 0)));

	typeGen(&astFile);

	astNodeTrav((ASTNode *) &astFile, (ASTTravFunc) _tLabel, NULL, labels);

	T_ASSERT("no errors", cerrCount() == 0);

	freeDList(&tokens, (FreeFunc) freeToken);
	freeASTFile(&astFile);
}

void tTypeGenFailed(const char *code, CError *errors) {
	Stream stream;
	initCerr();
	initStreamStr(&stream, code);
	DList tokens = tokenize(&stream, "UNKNOWN");
	ASTFile astFile;

	T_ASSERT("parseASTFile failed", parseASTFile(&astFile, tokListGetm(&tokens, 0)));

	typeGen(&astFile);

	char msg[256];
	for (int i = 0; i < cerrCount(); i++) {
		snprintf(msg, sizeof(msg), "%s != %s: %s", cerrStr(errors[i]), cerrStr(getCerr()[i]), code);
		T_ASSERT(msg, errors[i] == getCerr()[i]);
		if (getCerr()[i] == CERR_UNKNOWN || errors[i] == CERR_UNKNOWN) break;
	}

	freeDList(&tokens, (FreeFunc) freeToken);
	freeASTFile(&astFile);
}

#include "file.h"
#include "declaration.h"
#include "funcDef.h"
#include "../sem/scope.h"
#include "astUtil.h"
#include "../util/callbacks.h"
#include "node.h"
#include <stdio.h>

int parseASTFileItem(
		ASTNode *item,
		const Token *tok,
		ASTScope *scope)
{
	int n = 0, res;

	initASTNode(item, tok);
	if (astHasErr()) {
		return 0;
	}

	if ((res = parseASTFuncDef((ASTFuncDef *) item, tok + n, scope))) {
		n += res;
	} else if ((res = parseASTDeclaration((ASTDeclaration *) item, tok + n, scope))) {
		n += res;
	} else {
		return 0;
	}

	return n;
}

void initASTFile(ASTFile *file, Token const *tok) {
	initASTNode((ASTNode *) file, tok);
	initDListEmpty(&file->items, AST_NODE_S);
	initASTScope(&file->scope);
}

void freeASTFile(ASTFile *file) {
	freeDList(&file->items, (FreeFunc) freeASTNode);
	freeASTScope(&file->scope);
}

int parseASTFile(ASTFile *file, const Token *tok) {
	AST_VALID(ASTFile);
	int n = 0, res;
	ASTNodeBuf tempBuf;

	initASTFile(file, tok);

	while (1) {
		if ((res = parseASTFileItem((ASTNode *) &tempBuf, tok + n, &file->scope))) {
			n += res;
			dlistApp(&file->items, &tempBuf);

		} else if (tok[n].type == TT_NEWLINE) {
			n++;
		} else {
			break;
		}
	}
	if (astHasErr()) {
		fprintASTErr(stderr);
	}

	file->node.type = AST_FILE;
	return n;
}

int printASTFile(const ASTFile *file) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"file\"");

	n += printf(", \"declerations\": ");
	n += printDList(&file->items, (PrintFunc) printASTNode);

	n += printf("}");

	return n;
}

ASTTravRes astFileTrav(
		ASTFile *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx)
{
	ASTTravRes result;

	for (int i = 0; i < node->items.size; i++) {
		ASTNode *item = dlistGetm(&node->items, i);
		result = astNodeTrav(item, beforeFunc, afterFunc, ctx);
		if (result == ASTT_FAILED) return ASTT_FAILED;
	}

	return ASTT_SUCCESS;
}

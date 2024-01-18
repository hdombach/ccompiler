#include "stringConstant.h"
#include "astUtil.h"
#include "node.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static ASTNodeVTable _stringConstVTable = {
	{
		(FreeFunc) freeASTStringConstant,
		(PrintFunc) printASTStringConstant,
	},
	(ASTChildCount) astStringConstantChildCount,
	(ASTGetChild) astStringConstantGetChild,
};

void initASTStringConstant(ASTStringConstant *node, const Token *tok) {
	initASTNode((ASTNode *)node, tok, &_stringConstVTable);
	node->value = NULL;
}

void freeASTStringConstant(ASTStringConstant *node) {
	if (node->value) free(node->value);
}

int parseASTStringConstant(ASTStringConstant *node, const Token *tok) {
	AST_VALID(ASTStringConstant);
	if (astHasErr()) return 0;

	initASTStringConstant(node, tok);

	if (tok->type != TT_STR_CONSTANT) return 0;
	if (!tok->contents) return 0;

	node->value = strdup(tok->contents);

	node->node.type = AST_STR_CONSTANT;

	return 1;
}

int printASTStringConstant(const ASTStringConstant *node) {
	int n = 0;

	n += printf("{");
	n += printf("\"node type\": \"%s\"", astNodeTypeStr(node->node.type));
	n += printf(", \"value\": \"%s\"", node->value);
	n += printf("}");

	return n;
}

int astStringConstantChildCount(const ASTStringConstant *node) {
	return 0;
}

ASTNode *astStringConstantGetChild(ASTStringConstant *node, int index) {
	return NULL;
}

#include "charConstant.h"
#include "astUtil.h"
#include "node.h"

void initASTCharConstant(ASTCharConstant *node, const Token *tok) {
	initASTNode((ASTNode *) node, tok);
	node->value = 0;
}

int parseASTCharConstant(ASTCharConstant *node, const Token *tok) {
	AST_VALID(ASTCharConstant);
	if (astHasErr()) return 0;

	initASTCharConstant(node, tok);

	if (tok->type != TT_CHAR_CONSTANT) return 0;
	if (!tok->contents) return 0;

	node->value = *tok->contents;

	node->node.type = AST_CHAR_CONSTANT;

	return 1;
}

int printAStCharConstant(const ASTCharConstant *node) {
	int n = 0;

	n += printf("{");
	n += printf("\"node type\": \"%s\"", astNodeTypeStr(node->node.type));
	n += printf(", \"value\": \"%c\"", node->value);
	n += printf("}");

	return n;
}

int astCharConstantChildCount(const ASTCharConstant *node) {
	return 0;
}

ASTNode *astCharConstantGetChild(ASTCharConstant *node, int index) {
	return NULL;
}

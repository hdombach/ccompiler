#include "expression.h"
#include "intConstant.h"
#include "astUtil.h"

void initASTExp(ASTExp *node) {
	node->type = ASTE_UNKNOWN;
}

void freeASTExp(ASTExp *node) {
	return;
}

int parseASTExp(ASTExp *node, Token const *tok) {
	int res, n = 0;

	initASTExp(node);

	if (astHasErr()) {
		return 0;
	}

	if ((res = parseASTIntConstant(&node->c.intConstant, tok))) {
		node->type = ASTE_INT_CONSTANT;
		return res;
	}
	return 0;
}

int printASTExp(const ASTExp *node) {
	int n = 0;

	switch (node->type) {
		case ASTE_INT_CONSTANT:
			n += printASTIntContant(&node->c.intConstant);
			break;
		default:
			n += printf("{\"type\": \"Expression\", \"value\": \"unknown\"}");
			break;
	}

	return n;
}

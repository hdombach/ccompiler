#include "expression.h"
#include "intConstant.h"
#include "astUtil.h"
#include "operation.h"

void initASTExp(ASTExp *node) {
	node->type = ASTE_UNKNOWN;
}

void freeASTExp(ASTExp *node) {
	switch (node->type) {
		case ASTE_OPERATION:
			freeASTOperation(&node->c.operation);
			break;
		default:
			break;
	}
	node->type = ASTE_UNKNOWN;
}

int parseASTExp(ASTExp *node, Token const *tok) {
	return parseASTExp15(node, tok);
}

int parseASTExpSing(ASTExp *node, Token const *tok) {
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

int parseASTExp15(ASTExp *node, const Token *tok) {
	int n = 0, res;

	initASTExp(node);
	if (astHasErr()) {
		freeASTExp(node);
		return 0;
	}
	
	if ((res = parseASTOperation15(&node->c.operation, tok + n))) {
		n += res;
		node->type = ASTE_OPERATION;
	} else if ((res = parseASTExpSing(node, tok))) {
		n += res;
	} else {
		freeASTExp(node);
		return 0;
	}

	return n;
}

int parseASTExp14(ASTExp *node, Token const *tok) {
	return parseASTExpSing(node, tok);
}

int printASTExp(const ASTExp *node) {
	int n = 0;

	switch (node->type) {
		case ASTE_INT_CONSTANT:
			n += printASTIntContant(&node->c.intConstant);
			break;
		case ASTE_OPERATION:
			n += printASTOperation(&node->c.operation);
			break;
		default:
			n += printf("{\"node type\": \"expression\", \"value\": \"unknown\"}");
			break;
	}

	return n;
}

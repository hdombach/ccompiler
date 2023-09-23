#include <string.h>

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
		case ASTE_IDENTIFIER:
			free(node->c.identifier);
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
		n += res;
		return res;
	} else if (tok[n].type == TT_IDENTIFIER) {
		node->type = ASTE_IDENTIFIER;
		node->c.identifier = strdup(tok[n].contents);
		n++;
	}
	return n;
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
	} else if ((res = parseASTExp14(node, tok + n))) {
		n += res;
	} else {
		freeASTExp(node);
		return 0;
	}

	return n;
}

int parseASTExp14(ASTExp *node, Token const *tok) {
	int n = 0, res;

	initASTExp(node);
	if (astHasErr()) {
		freeASTExp(node);
		return 0;
	}

	if ((res = parseASTOperation14(&node->c.operation, tok + n))) {
		n += res;
		node->type = ASTE_OPERATION;
	} else if ((res = parseASTExp13(node, tok + n))) {
		n += res;
	} else {
		freeASTExp(node);
		return 0;
	}

	return n;
}

int parseASTExp13(ASTExp *node, const Token *tok) {
	int n = 0, res;

	initASTExp(node);
	if (astHasErr()) {
		freeASTExp(node);
		return 0;
	}

	if ((res = parseASTOperation13(&node->c.operation, tok + n))) {
		n += res;
		node->type = ASTE_OPERATION;
	} else if ((res = parseASTExp12(node, tok + n))) {
		n += res;
	} else {
		freeASTExp(node);
		return 0;
	} 

	return n;
}

int parseASTExp12(ASTExp *node, const Token *tok) {
	int n = 0, res;

	initASTExp(node);
	if (astHasErr()) {
		freeASTExp(node);
		return 0;
	}

	if ((res = parseASTOperation12(&node->c.operation, tok + n))) {
		n += res;
		node->type = ASTE_OPERATION;
	} else if ((res = parseASTExp11(node, tok + n))) {
		n += res;
	} else {
		freeASTExp(node);
		return 0;
	}
	
	return n;
}

int parseASTExp11(ASTExp *node, const Token *tok) {
	int n = 0, res;

	initASTExp(node);
	if (astHasErr()) {
		freeASTExp(node);
		return 0;
	}

	if ((res = parseASTOperation11(&node->c.operation, tok + n))) {
		n += res;
		node->type = ASTE_OPERATION;
	} else if ((res = parseASTExp10(node, tok + n))) {
		n += res;
	} else {
		freeASTExp(node);
		return 0;
	}

	return n;
}

int parseASTExp10(ASTExp *node, Token const *tok) {
	int n = 0, res;

	initASTExp(node);
	if (astHasErr()) {
		freeASTExp(node);
		return 0;
	}

	if ((res = parseASTOperation10(&node->c.operation, tok + n))) {
		n += res;
		node->type = ASTE_OPERATION;
	} else if ((res = parseASTExp9(node, tok + n))) {
		n += res;
	} else {
		freeASTExp(node);
		return 0;
	}

	return n;
}

int parseASTExp9(ASTExp *node, Token const *tok) {
	int n = 0, res;

	initASTExp(node);
	if (astHasErr()) {
		freeASTExp(node);
		return 0;
	}

	if ((res =  parseASTOperation9(&node->c.operation, tok + n))) {
		n += res;
		node->type = ASTE_OPERATION;
	} else if ((res = parseASTExp8(node, tok + n))) {
		n += res;
	} else {
		freeASTExp(node);
		return 0;
	}

	return n;
}

int parseASTExp8(ASTExp *node, Token const *tok) {
	int n = 0, res;

	initASTExp(node);
	if (astHasErr()) {
		freeASTExp(node);
		return 0;
	}

	if ((res = parseASTOperation8(&node->c.operation, tok + n))) {
		n += res;
		node->type = ASTE_OPERATION;
	} else if ((res = parseASTExp7(node, tok + n))) {
		n += res;
	} else {
		freeASTExp(node);
		return 0;
	}

	return n;
}

int parseASTExp7(ASTExp *node, Token const *tok) {
	int n = 0, res;

	initASTExp(node);
	if (astHasErr()) {
		freeASTExp(node);
		return 0;
	}

	if ((res = parseASTOperation7(&node->c.operation, tok + n))) {
		n += res;
		node->type = ASTE_OPERATION;
	} else if ((res = parseASTExp6(node, tok + n))) {
		n += res;
	} else {
		freeASTExp(node);
		return 0;
	}

	return n;
}

int parseASTExp6(ASTExp *node, Token const *tok) {
	int n = 0, res;

	initASTExp(node);
	if (astHasErr()) {
		freeASTExp(node);
		return 0;
	}

	if ((res = parseASTOperation6(&node->c.operation, tok + n))) {
		n += res;
		node->type = ASTE_OPERATION;
	} else if ((res = parseASTExp5(node, tok + n))) {
		n += res;
	} else {
		freeASTExp(node);
		return 0;
	}

	return n;
}

int parseASTExp5(ASTExp *node, Token const *tok) {
	int n = 0, res;

	initASTExp(node);
	if (astHasErr()) {
		freeASTExp(node);
		return 0;
	}

	if ((res = parseASTOperation5(&node->c.operation, tok + n))) {
		n += res;
		node->type = ASTE_OPERATION;
	} else if ((res = parseASTExp4(node, tok + n))) {
		n += res;
	} else {
		freeASTExp(node);
		return 0;
	}

	return n;
}

int parseASTExp4(ASTExp *node, Token const *tok) {
	int n = 0, res;

	initASTExp(node);
	if (astHasErr()) {
		freeASTExp(node);
		return 0;
	}

	if ((res = parseASTOperation4(&node->c.operation, tok + n))) {
		n += res;
		node->type = ASTE_OPERATION;
	} else if ((res = parseASTExp3(node, tok + n))) {
		n += res;
	} else {
		freeASTExp(node);
		return 0;
	}

	return n;
}

int parseASTExp3(ASTExp *node, Token const *tok) {
	int n = 0, res;

	initASTExp(node);
	if (astHasErr()) {
		freeASTExp(node);
		return 0;
	}

	if ((res = parseASTOperation3(&node->c.operation, tok + n))) {
		n += res;
		node->type = ASTE_OPERATION;
	} else if ((res = parseASTExp2(node, tok + n))) {
		n += res;
	} else {
		freeASTExp(node);
		return 0;
	}

	return n;
}

int parseASTExp2(ASTExp *node, Token const *tok) {
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
		case ASTE_IDENTIFIER:
			n +=printf("{\"node type\": \"identifier\", \"value\": \"%s\"}", node->c.identifier); 
			break;
		default:
			n += printf("{\"node type\": \"expression\", \"value\": \"unknown\"}");
			break;
	}

	return n;
}

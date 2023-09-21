#include "statement.h"
#include "expression.h"
#include "astUtil.h"
void initASTStm(ASTStm *node) {
	node->type = ASTS_UNKNOWN;
}

void freeASTStm(ASTStm *node) {
	switch (node->type) {
		case ASTS_EXP:
			freeASTExp(&node->c.exp);
			break;
		default:
			break;
	}
}

int parseASTStm(ASTStm *node, const Token *tok) {
	int res, n = 0;

	if (astHasErr()) {
		return 0;
	}

	if ((res = parseASTExp(&node->c.exp, tok + n))) {
		node->type = ASTS_EXP;
		n += res;
		if (tok[n].type != TT_SEMI_COLON) {
			freeASTExp(&node->c.exp);
			return 0;
		}
		n++;
	}
	return n;
}

int printASTStm(ASTStm const *node) {
	int n = 0;

	switch (node->type) {
		case ASTS_EXP:
			n += printASTExp(&node->c.exp);
			break;
		default:
			n += printf("{\"type\": \"Statement\", \"value\": \"unknown\"}");
			break;
	
	}

	return n;
}

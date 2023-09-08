#include "initializer.h"
#include "astUtil.h"
#include "expression.h"

void initASTInitializer(ASTInitializer *initializer) {
	initializer->type = AST_IT_UNKNOWN;
}

void freeASTInitializer(ASTInitializer *initializer) {
	switch (initializer->type) {
		case AST_IT_EXP:
			freeASTExp(&initializer->c.exp);
			break;
		default:
			break;
	}
	initializer->type = AST_IT_UNKNOWN;
}

int parseASTInitializer(ASTInitializer *initializer, const Token *tok) {
	int n = 0, res;
	initASTInitializer(initializer);
	if (astHasErr()) {
		return 0;
	}

	if ((res = parseASTExp(&initializer->c.exp, tok + n))) {
		initializer->type = AST_IT_EXP;
		n += res;
	} else {
		freeASTExp(&initializer->c.exp);
	}

	return n;
}

int printASTInitializer(const ASTInitializer *initializer) {
	int n = 0;

	switch (initializer->type) {
		case AST_IT_EXP:
			n += printASTExp(&initializer->c.exp);
			break;
		default:
			break;
	}

	return n;
}

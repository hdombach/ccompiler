#include <stdlib.h>

#include "doWhile.h"
#include "expression.h"
#include "statement.h"
#include "astUtil.h"
#include "while.h"

void initASTDoWhile(ASTDoWhile *node) {
	node->expression = NULL;
	node->statement = NULL;
}

void freeASTDoWhile(ASTDoWhile *node) {
	if (node->expression) {
		freeASTExp(node->expression);
		free(node->expression);
	}

	if (node->statement) {
		freeASTStm(node->statement);
		free(node->statement);
	}
}

int parseASTDoWhile(
		ASTDoWhile *node,
		const struct Token *tok,
		struct ASTScope const *scope)
{
	int n = 0, res;
	ASTExp tempExp;
	ASTStm tempStm;

	initASTDoWhile(node);
	if (astHasErr()) {
		freeASTDoWhile(node);
		return 0;
	}

	if (tok[n].type == TT_DO) {
		n++;
	} else {
		freeASTDoWhile(node);
		return 0;
	}

	if ((res = parseASTStm(&tempStm, tok + n, scope))) {
		n += res;
		node->statement = malloc(sizeof(ASTStm));
		*node->statement = tempStm;
	} else {
		astErr("Expecting statement after do statement"	, tok + n);
		freeASTDoWhile(node);
		return 0;
	}

	if (tok[n].type == TT_WHILE) {
		n++;
	} else {
		freeASTDoWhile(node);
		return 0;
	}

	if (tok[n].type == TT_O_PARAN) {
		n++;
	} else {
		freeASTDoWhile(node);
		return 0;
	}

	if ((res = parseASTExp(&tempExp, tok + n, scope))) {
		n += res;
		node->expression = malloc(sizeof(ASTExp));
		*node->expression = tempExp;
	} else {
		astErr("Expected expression after do while statement", tok + n);
		freeASTDoWhile(node);
		return 0;
	}

	if (tok[n].type == TT_C_PARAN) {
		n++;
	} else {
		astErr("Expecting ) following do while", tok + n);
		freeASTDoWhile(node);
		return 0;
	}

	if (tok[n].type == TT_SEMI_COLON) {
		n++;
	} else {
		astErr("Expecting ; following do while", tok + n);
		freeASTDoWhile(node);
		return 0;
	}

	return n;
}

int printASTDoWhile(const ASTDoWhile *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"do while\"");

	if (node->statement) {
		n += printf(", \"statement\": ");
		n += printASTStm(node->statement);
	}

	if (node->expression) {
		n += printf(", \"expression\": ");
		n += printASTExp(node->expression);
	}

	n += printf("}");

	return n;
}

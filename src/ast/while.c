#include <stdlib.h>

#include "while.h"
#include "expression.h"
#include "statement.h"
#include "astUtil.h"

void initASTWhile(ASTWhile *node) {
	node->expression = NULL;
	node->statement = NULL;
}

void freeASTWhile(ASTWhile *node) {
	if (node->expression) {
		freeASTExp(node->expression);
		free(node->expression);
	}

	if (node->statement) {
		freeASTStm(node->statement);
		free(node->statement);
	}
}

int parseASTWhile(
		ASTWhile *node,
		const struct Token *tok,
		struct ASTScope const *scope)
{
	int n = 0, res;
	ASTExp tempExp;
	ASTStm tempStm;

	initASTWhile(node);
	if (astHasErr()) {
		freeASTWhile(node);
		return 0;
	}

	if (tok[n].type == TT_WHILE) {
		n++;
	} else {
		freeASTWhile(node);
		return 0;
	}

	if (tok[n].type == TT_O_PARAN) {
		n++;
	} else {
		astErr("Expecting ( after while", tok + n);
		freeASTWhile(node);
		return 0;
	}

	if ((res = parseASTExp(&tempExp, tok + n, scope))) {
		n += res;
		node->expression = malloc(sizeof(ASTExp));
		*node->expression = tempExp;
	} else {
		astErr("Expecting expression after while", tok + n);
		freeASTWhile(node);
		return 0;
	}

	if (tok[n].type == TT_C_PARAN) {
		n++;
	} else {
		astErr("Expecting ) after while", tok + n);
		freeASTWhile(node);
		return 0;
	}

	if ((res = parseASTStm(&tempStm, tok + n, scope))) {
		n += res;
		node->statement = malloc(sizeof(ASTStm));
		*node->statement = tempStm;
	} else {
		astErr("Expecting statement after while", tok + n);
		freeASTWhile(node);
		return 0;
	}
	
	return n;
}

int printASTWhile(const ASTWhile *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"while\"");

	if (node->expression) {
		n += printf(", \"expression\": ");
		n += printASTExp(node->expression);
	}

	if (node->statement) {
		n += printf(", \"statement\": ");
		n += printASTStm(node->statement);
	}

	n += printf("}");

	return n;
}

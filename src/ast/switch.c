#include <stdlib.h>

#include "switch.h"
#include "expression.h"
#include "statement.h"
#include "scope.h"
#include "astUtil.h"

void initASTSwitch(ASTSwitch *node) {
	node->expression = NULL;
	node->statement = NULL;
}

void freeASTSwitch(ASTSwitch *node) {
	if (node->expression) {
		freeASTExp(node->expression);
		free(node->expression);
	}

	if (node->statement) {
		freeASTStm(node->statement);
		free(node->statement);
	}
}

int parseASTSwitch(ASTSwitch *node, Token const *tok, ASTScope *scope) {
	int n = 0, res;
	ASTExp tempExp;
	ASTStm tempStatement;

	initASTSwitch(node);
	if (astHasErr()) {
		freeASTSwitch(node);
		return 0;
	}

	if (tok[n].type == TT_SWITCH) {
		n++;
	} else {
		freeASTSwitch(node);
		return 0;
	}

	if (tok[n].type == TT_O_PARAN) {
		n++;
	} else {
		astErr("Expecting ( following switch", tok + n);
		freeASTSwitch(node);
		return 0;
	}

	if ((res = parseASTExp(&tempExp, tok + n, scope))) {
		n += res;
		node->expression = malloc(sizeof(ASTExp));
		*node->expression = tempExp;
	} else {
		astErr("Expecting expression following switch", tok + n);
		freeASTSwitch(node);
		return 0;
	}

	if (tok[n].type == TT_C_PARAN) {
		n++;
	} else {
		astErr("Expected ) following switch", tok + n);
		freeASTSwitch(node);
		return 0;
	}

	if ((res = parseASTStm(&tempStatement, tok + n, scope))) {
		n += res;
		node->statement = malloc(sizeof(ASTStm));
		*node->statement = tempStatement;
	} else {
		astErr("Expecting statement following switch", tok + n);
		freeASTSwitch(node);
		return 0;
	}

	return n;
}

int printASTSwitch(const ASTSwitch *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"switch\"");

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

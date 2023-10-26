#include <stdlib.h>

#include "node.h"
#include "switch.h"
#include "expression.h"
#include "statement.h"
#include "../sem/scope.h"
#include "astUtil.h"

void initASTSwitch(ASTSwitch *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok);
	node->expression = NULL;
	node->statement = NULL;
}

void freeASTSwitch(ASTSwitch *node) {
	if (node->expression) {
		freeASTNode(node->expression);
		free(node->expression);
	}

	if (node->statement) {
		freeASTStm(node->statement);
		free(node->statement);
	}
}

int parseASTSwitch(ASTSwitch *node, Token const *tok, ASTScope *scope) {
	AST_VALID(ASTSwitch);
	int n = 0, res;
	ASTNodeBuf tempBuf;

	initASTSwitch(node, tok);
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

	if ((res = parseASTExp((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->expression = dupASTNode((ASTNode *) &tempBuf);
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

	if ((res = parseASTStm((ASTStm *) &tempBuf, tok + n, scope))) {
		n += res;
		node->statement = (ASTStm *) dupASTNode((ASTNode *) &tempBuf);
	} else {
		astErr("Expecting statement following switch", tok + n);
		freeASTSwitch(node);
		return 0;
	}

	node->node.type = AST_SWITCH;

	return n;
}

int printASTSwitch(const ASTSwitch *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"switch\"");

	if (node->expression) {
		n += printf(", \"expression\": ");
		n += printASTNode(node->expression);
	}

	if (node->statement) {
		n += printf(", \"statement\": ");
		n += printASTStm(node->statement);
	}

	n += printf("}");

	return n;
}

ASTTravRes astSwitchTrav(
		ASTSwitch *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx)
{
	ASTTravRes result;

	result = astNodeTrav(node->expression, beforeFunc, afterFunc, ctx);
	if (result == ASTT_FAILED) return ASTT_FAILED;

	result = astNodeTrav((ASTNode *) node->statement, beforeFunc, afterFunc, ctx);
	if (result == ASTT_FAILED) return ASTT_FAILED;

	return ASTT_SUCCESS;
}

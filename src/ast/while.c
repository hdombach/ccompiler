#include <stdlib.h>

#include "node.h"
#include "while.h"
#include "expression.h"
#include "statement.h"
#include "astUtil.h"

void initASTWhile(ASTWhile *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok);
	node->expression = NULL;
	node->statement = NULL;
}

void freeASTWhile(ASTWhile *node) {
	if (node->expression) {
		freeASTNode(node->expression);
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
	ASTNodeBuf tempBuf;

	initASTWhile(node, tok);
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

	if ((res = parseASTExp((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->expression = dupASTNode((ASTNode *) &tempBuf);
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

	if ((res = parseASTStm((ASTStm *) &tempBuf, tok + n, scope))) {
		n += res;
		node->statement = (ASTStm *) dupASTNode((ASTNode *) &tempBuf);
	} else {
		astErr("Expecting statement after while", tok + n);
		freeASTWhile(node);
		return 0;
	}

	node->node.type = AST_WHILE;
	
	return n;
}

int printASTWhile(const ASTWhile *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"while\"");

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

ASTTravRes astWhileTrav(
		ASTWhile *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc)
{
	ASTTravRes result;

	result = astNodeTrav(node->expression, beforeFunc, afterFunc);
	if (result == ASTT_FAILED) return ASTT_FAILED;

	result = astNodeTrav((ASTNode *) node->statement, beforeFunc, afterFunc);
	if (result == ASTT_FAILED) return ASTT_FAILED;

	return ASTT_SUCCESS;
}

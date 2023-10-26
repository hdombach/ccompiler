#include <stdlib.h>

#include "doWhile.h"
#include "expression.h"
#include "node.h"
#include "statement.h"
#include "astUtil.h"
#include "while.h"

void initASTDoWhile(ASTDoWhile *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok);
	node->expression = NULL;
	node->statement = NULL;
}

void freeASTDoWhile(ASTDoWhile *node) {
	if (node->expression) {
		freeASTNode(node->expression);
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
		struct ASTScope *scope)
{
	AST_VALID(ASTDoWhile);
	int n = 0, res;
	ASTNodeBuf tempBuf;
	ASTStm tempStm;

	initASTDoWhile(node, tok);
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

	if ((res = parseASTExp((ASTNode *) &tempBuf, tok + n, scope))) {
		n += res;
		node->expression = dupASTNode((ASTNode *) &tempBuf);
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

	node->node.type = AST_DO_WHILE;
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
		n += printASTNode(node->expression);
	}

	n += printf("}");

	return n;
}

ASTTravRes astDoWhileTrav(ASTDoWhile *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx)
{
	ASTTravRes result;

	result = astNodeTrav((ASTNode *) node->statement, beforeFunc, afterFunc, ctx);
	if (result == ASTT_FAILED) return ASTT_FAILED;

	result = astNodeTrav(node->expression, beforeFunc, afterFunc, ctx);
	if (result == ASTT_FAILED) return ASTT_FAILED;

	return ASTT_SUCCESS;
}

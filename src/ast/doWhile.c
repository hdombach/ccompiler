#include <stdlib.h>

#include "doWhile.h"
#include "expression.h"
#include "node.h"
#include "statement.h"
#include "astUtil.h"
#include "while.h"

void initASTDoWhile(ASTDoWhile *node) {
	initASTNode((ASTNode *) node);
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
		struct ASTScope const *scope)
{
	AST_VALID(ASTDoWhile);
	int n = 0, res;
	ASTNodeBuf tempBuf;
	ASTNode *tempNode = (ASTNode *) &tempBuf;
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

	if ((res = parseASTExp(tempNode, tok + n, scope))) {
		n += res;
		node->expression = dupASTNode((ASTNode *) &tempNode);
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

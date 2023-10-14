#include <stdio.h>
#include <stdlib.h>

#include "astUtil.h"
#include "expression.h"
#include "if.h"
#include "node.h"
#include "statement.h"
#include "../token.h"

void initASTIf(ASTIf *node) {
	initASTNode((ASTNode *) node);
	node->expression = NULL;
	node->trueStatement = NULL;
	node->falseStatement = NULL;
}

void freeASTIf(ASTIf *node) {
	if (node->expression) {
		freeASTNode(node->expression);
		free(node->expression);
	}

	if (node->trueStatement) {
		freeASTStm(node->trueStatement);
		free(node->trueStatement);
	}

	if (node->falseStatement) {
		freeASTStm(node->falseStatement);
		free(node->falseStatement);
	}
}

int parseASTIf(ASTIf *node, struct Token const *tok, struct ASTScope const *scope) {
	AST_VALID(ASTIf);
	int res, n = 0;
	ASTNodeBuf tempBuf;
	ASTNode *tempNode = (ASTNode *) &tempBuf;
	ASTStm tempStatement;

	initASTIf(node);

	if (astHasErr()) {
		freeASTIf(node);
		return 0;
	}

	if (tok[n].type == TT_IF) {
		n++;
	} else {
		freeASTIf(node);
		return 0;
	}

	if (tok[n].type == TT_O_PARAN) {
		n++;
	} else {
		astErr("expected (", tok + n);
		freeASTIf(node);
		return 0;
	}

	if ((res = parseASTExp(tempNode, tok + n, scope))) {
		n += res;
		node->expression = malloc(AST_NODE_S);
		mvASTNode(node->expression, tempNode);
	} else {
		astErr("expected expression", tok + n);
		freeASTIf(node);
		return 0;
	}

	if (tok[n].type == TT_C_PARAN) {
		n++;
	} else {
		astErr("expected )", tok + n);
		freeASTIf(node);
		return 0;
	}

	if ((res = parseASTStm(&tempStatement, tok + n, scope))) {
		n += res;
		node->trueStatement = malloc(sizeof(ASTStm));
		*node->trueStatement = tempStatement;
	} else {
		astErr("Expected statement", tok + n);
		freeASTIf(node);
		return 0;
	}

	if (tok[n].type == TT_ELSE) {
		n++;
	} else {
		node->node.type = AST_IF;
		return n;
	}

	if ((res = parseASTStm(&tempStatement, tok + n, scope))) {
		n += res;
		node->falseStatement = malloc(sizeof(ASTStm));
		*node->falseStatement = tempStatement;
	} else {
		astErr("Expected statement", tok + n);
		freeASTIf(node);
		return 0;
	}

	node->node.type = AST_IF;

	return n;
}

int printASTIf(ASTIf const *node) {
	int n = 0;

	n += printf("{");

	n += printf("\"node type\": \"if statement\"");

	if (node->expression) {
		n += printf(", \"expression\": ");
		n += printASTNode(node->expression);
	}

	if (node->trueStatement) {
		n += printf(", \"true statement\": ");
		n += printASTStm(node->trueStatement);
	}

	if (node->falseStatement) {
		n += printf(", \"false statement\": ");
		n += printASTStm(node->falseStatement);
	}

	n += printf("}");

	return n;
}

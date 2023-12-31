#include "compStatement.h"
#include "../util/dlist.h"
#include "declaration.h"
#include "node.h"
#include "statement.h"
#include "astUtil.h"
#include "../util/callbacks.h"

int parseASTCompItem(
		ASTNode *node,
		const Token *tok,
		struct ASTScope *scope)
{
	int res, n = 0;
	initASTNode(node, tok);

	if (astHasErr()) {
		return 0;
	}

	if ((res = parseASTStm((ASTStm *) node, tok + n, scope))) {
		n += res;
	} else if ((res = parseASTDeclaration((ASTDeclaration *) node, tok + n, scope))) {
		n += res;
	} else {
		freeASTNode(node);
		return 0;
	}

	return n;
}

void initASTCompStm(ASTCompStm *node, Token const *tok) {
	initASTNode((ASTNode *) node, tok);
	initDListEmpty(&node->items, AST_NODE_S);
	node->scope = malloc(sizeof(ASTScope));
	initASTScope(node->scope);
}

void freeASTCompStm(ASTCompStm *node) {
	freeDList(&node->items, (FreeFunc) freeASTNode);
	freeASTScope(node->scope);
	free(node->scope);
}

int parseASTCompStm(
		ASTCompStm *node,
		const Token *tok,
		struct ASTScope *scope)
{
	AST_VALID(ASTCompStm);
	int res, n = 0;
	ASTNodeBuf tempBuf;

	if (astHasErr()) {
		return 0;
	}

	if (tok[n].type != TT_O_CURLY) {
		return 0;
	}
	n++;

	initASTCompStm(node, tok);
	while (!astHasErr()) {
		if ((res = parseASTCompItem((ASTNode *) &tempBuf, tok + n, scope))) {
			dlistApp(&node->items, &tempBuf);
			n += res;
		} else {
			break;
		}
	}

	if (tok[n].type != TT_C_CURLY) {
		freeASTCompStm(node);
		return 0;
	}
	n++;

	node->node.type = AST_COMP_STM;

	return n;
}

int printASTCompStm(const ASTCompStm *node) {
	int n = 0;

	n += printDList(&node->items, (PrintFunc) printASTNode);

	return n;
}

int astCompStmChildCount(ASTCompStm const *node) {
	return node->items.size;
}

ASTNode *astCompStmGetChild(ASTCompStm *node, int index) {
	return dlistGetm(&node->items, index);
}

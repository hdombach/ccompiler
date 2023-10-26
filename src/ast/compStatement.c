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
	initASTScope(&node->scope);
}

void freeASTCompStm(ASTCompStm *node) {
	freeDList(&node->items, (FreeFunc) freeASTNode);
	freeASTScope(&node->scope);
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

ASTTravRes astCompStmTrav(
		ASTCompStm *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx)
{
	ASTTravRes result;
	
	for (int i = 0; i < node->items.size; i++) {
		ASTNode *item = dlistGetm(&node->items, i);
		result = astNodeTrav(item, beforeFunc, afterFunc, ctx);
		if (result == ASTT_FAILED) return ASTT_FAILED;
	}

	return ASTT_SUCCESS;
}

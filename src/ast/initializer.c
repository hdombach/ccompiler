#include "initializer.h"
#include "astUtil.h"
#include "expression.h"
#include "../sem/scope.h"
#include "node.h"

void initASTInitializerList(ASTInitializerList *list, Token const *tok) {
	initASTNode((ASTNode *) list, tok);
	initDListEmpty(&list->list, AST_NODE_S);
}

void freeASTInitializerList(ASTInitializerList *list) {
	freeDList(&list->list, (FreeFunc) freeASTNode);
}

int parseASTInitializerList(
		ASTInitializerList *list,
		const Token *tok,
		const struct ASTScope *scope)
{
	AST_VALID(ASTInitializerList);
	int res, n = 0;
	ASTNodeBuf tempBuf;
	initASTInitializerList(list, tok);
	if (astHasErr()) {
		freeASTInitializerList(list);
		return 0;
	}

	if (tok[n].type == TT_O_CURLY) {
		n++;
	} else {
		freeASTInitializerList(list);
		return 0;
	}

	while (1) {
		if ((res = parseASTInitializer((ASTNode *) &tempBuf, tok + n, scope))) {
			n += res;
			dlistApp(&list->list, &tempBuf);
		} else {
			break;
		}

		if (tok[n].type == TT_COMMA) {
			n++;
		} else {
			break;
		}
	}

	if (tok[n].type == TT_C_CURLY) {
		n++;
	} else {
		astErr("Expecting }", tok + n);
		freeASTInitializerList(list);
		return 0;
	}

	list->node.type = AST_INITIALIZER_LIST;
	return n;
}

int printASTInitializerList(const ASTInitializerList *list) {
	return printDList(&list->list, (PrintFunc) printASTNode);
}

ASTTravRes astInitializerListTrav(
		ASTInitializerList *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx)
{
	ASTTravRes result;

	for (int i = 0; i < node->list.size; i++) {
		ASTNode *item = dlistGetm(&node->list, i);
		result = astNodeTrav(item, beforeFunc, afterFunc, ctx);
		if (result == ASTT_FAILED) return ASTT_FAILED;
	}

	return ASTT_SUCCESS;
}

int parseASTInitializer(
		ASTNode *node,
		const Token *tok,
		ASTScope const *scope)
{
	int res;

	if ((res = parseASTInitializerList((ASTInitializerList *) node, tok, scope))) {
		return res;
	} else {
		return parseASTExp14(node, tok, scope);
	}
}

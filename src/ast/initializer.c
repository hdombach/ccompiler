#include "initializer.h"
#include "astUtil.h"
#include "../sem/scope.h"
#include "node.h"
#include "operation.h"
#include "../util/log.h"

static ASTNodeVTable _vtable = {
	{
		(FreeFunc) freeASTInitializerList,
		(PrintFunc) printASTInitializerList,
	},
	(ASTChildCount) astInitializerListChildCount,
	(ASTGetChild) astInitializerListGetChild,
};

void initASTInitializerList(ASTInitializerList *list, Token const *tok) {
	initASTNode((ASTNode *) list, tok, &_vtable);
	initDListEmpty(&list->list, AST_NODE_S);
}

void freeASTInitializerList(ASTInitializerList *list) {
	freeDList(&list->list, (FreeFunc) freeASTNode);
}

int parseASTInitializerList(
		ASTInitializerList *list,
		const Token *tok,
		struct ASTScope *scope)
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
		logCerr(CERR_BRACE, tok + n, "Expecting }");
		freeASTInitializerList(list);
		return 0;
	}

	list->node.type = AST_INITIALIZER_LIST;
	return n;
}

int printASTInitializerList(const ASTInitializerList *list) {
	return printDList(&list->list, (PrintFunc) printASTNode);
}

int astInitializerListChildCount(const ASTInitializerList *node) {
	return node->list.size;
}

ASTNode *astInitializerListGetChild(ASTInitializerList *node, int index) {
	return dlistGetm(&node->list, index);
}

int parseASTInitializer(
		ASTNode *node,
		const Token *tok,
		ASTScope *scope)
{
	int res;

	if ((res = parseASTInitializerList((ASTInitializerList *) node, tok, scope))) {
		return res;
	} else {
		return parseASTOperation14((ASTOperation *) node, tok, scope);
	}
}

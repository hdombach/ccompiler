#include "initializer.h"
#include "astUtil.h"
#include "expression.h"
#include "scope.h"
#include "node.h"

void initASTInitializer(ASTInitializer *initializer) {
	initializer->type = AST_IT_UNKNOWN;
}

void freeASTInitializer(ASTInitializer *initializer) {
	switch (initializer->type) {
		case AST_IT_NODE:
			freeASTNode(initializer->c.exp);
			break;
		case AST_IT_LIST:
			freeDList(&initializer->c.initializerList, (FreeFunc) freeASTInitializer);
			break;
		default:
			break;
	}
	initializer->type = AST_IT_UNKNOWN;
}

int parseASTInitializer(
		ASTInitializer *initializer,
		const Token *tok,
		ASTScope const *scope)
{
	int n = 0, res;
	ASTNodeBuf tempBuf;
	initASTInitializer(initializer);
	if (astHasErr()) {
		return 0;
	}

	if (tok[n].type == TT_O_CURLY) {
		initDList(&initializer->c.initializerList, sizeof(ASTInitializer));
		initializer->type = AST_IT_LIST;
		n++;
		while (1) {
			ASTInitializer temp;
			if ((res = parseASTInitializer(&temp, tok + n, scope))) {
				n += res;
				dlistApp(&initializer->c.initializerList, &temp);
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
			astErr("Expecing }", tok + n);
			freeASTInitializer(initializer);
			return 0;
		}
	} else if ((res = parseASTExp((ASTNode *) &tempBuf, tok + n, scope))) {
		initializer->c.exp = dupASTNode((ASTNode *) &tempBuf);
		initializer->type = AST_IT_NODE;
		n += res;
	} else {
		return 0;
	}

	return n;
}

int printASTInitializer(const ASTInitializer *initializer) {
	int n = 0;

	switch (initializer->type) {
		case AST_IT_NODE:
			n += printASTNode(initializer->c.exp);
			break;
		case AST_IT_LIST:
			n += printDList(
					&initializer->c.initializerList,
					(PrintFunc) printASTInitializer);
			break;
		default:
			break;
	}

	return n;
}

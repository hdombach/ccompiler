#include <string.h>

#include "expression.h"
#include "identifier.h"
#include "intConstant.h"
#include "astUtil.h"
#include "node.h"
#include "operation.h"
#include "scope.h"

int parseASTExp(ASTNode *node, Token const *tok, ASTScope const *scope) {
	return parseASTExp15(node, tok, scope);
}

int parseASTExpSing(ASTNode *node, Token const *tok, ASTScope const *scope) {
	//AST_VALID(ASTExp);
	int res, n = 0;

	initASTNode(node);

	if (astHasErr()) {
		return 0;
	}

	if ((res = parseASTIntConstant((ASTIntConstant *) node, tok))) {
		n += res;
		return res;
	} else if ((res = parseASTIdentifier((ASTIdentifier *) node, tok, scope))) {
		n += res;
		return res;
	} else if (tok[n].type == TT_O_PARAN) {
		n++;
		if ((res = parseASTExp(node, tok + n, scope))) {
			n += res;
		} else {
			freeASTNode(node);
			return 0;
		}

		if (tok[n].type == TT_C_PARAN) {
			n++;
		} else {
			freeASTNode(node);
			return 0;
		}
	} else {
		freeASTNode(node);
		return 0;
	}

	return n;
}

int parseASTExp15(ASTNode *node, const Token *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTNode(node);
	if (astHasErr()) {
		freeASTNode(node);
		return 0;
	}
	
	if ((res = parseASTOperation15((ASTOperation *) node, tok + n, scope))) {
		n += res;
	} else if ((res = parseASTExp14(node, tok + n, scope))) {
		n += res;
	} else {
		freeASTNode(node);
		return 0;
	}

	return n;
}

int parseASTExp14(ASTNode *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTNode(node);
	if (astHasErr()) {
		freeASTNode(node);
		return 0;
	}

	if ((res = parseASTOperation14((ASTOperation *) node, tok + n, scope))) {
		n += res;
	} else if ((res = parseASTExp13(node, tok + n, scope))) {
		n += res;
	} else {
		freeASTNode(node);
		return 0;
	}

	return n;
}

int parseASTExp13(ASTNode *node, const Token *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTNode(node);
	if (astHasErr()) {
		freeASTNode(node);
		return 0;
	}

	if ((res = parseASTOperation13((ASTOperation *) node, tok + n, scope))) {
		n += res;
	} else if ((res = parseASTExp12(node, tok + n, scope))) {
		n += res;
	} else {
		freeASTNode(node);
		return 0;
	} 

	return n;
}

int parseASTExp12(ASTNode *node, const Token *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTNode(node);
	if (astHasErr()) {
		freeASTNode(node);
		return 0;
	}

	if ((res = parseASTOperation12((ASTOperation *) node, tok + n, scope))) {
		n += res;
	} else if ((res = parseASTExp11(node, tok + n, scope))) {
		n += res;
	} else {
		freeASTNode(node);
		return 0;
	}
	
	return n;
}

int parseASTExp11(ASTNode *node, const Token *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTNode(node);
	if (astHasErr()) {
		freeASTNode(node);
		return 0;
	}

	if ((res = parseASTOperation11((ASTOperation *) node, tok + n, scope))) {
		n += res;
	} else if ((res = parseASTExp10(node, tok + n, scope))) {
		n += res;
	} else {
		freeASTNode(node);
		return 0;
	}

	return n;
}

int parseASTExp10(ASTNode *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTNode(node);
	if (astHasErr()) {
		freeASTNode(node);
		return 0;
	}

	if ((res = parseASTOperation10((ASTOperation *) node, tok + n, scope))) {
		n += res;
	} else if ((res = parseASTExp9(node, tok + n, scope))) {
		n += res;
	} else {
		freeASTNode(node);
		return 0;
	}

	return n;
}

int parseASTExp9(ASTNode *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTNode(node);
	if (astHasErr()) {
		freeASTNode(node);
		return 0;
	}

	if ((res =  parseASTOperation9((ASTOperation *) node, tok + n, scope))) {
		n += res;
	} else if ((res = parseASTExp8(node, tok + n, scope))) {
		n += res;
	} else {
		freeASTNode(node);
		return 0;
	}

	return n;
}

int parseASTExp8(ASTNode *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTNode(node);
	if (astHasErr()) {
		freeASTNode(node);
		return 0;
	}

	if ((res = parseASTOperation8((ASTOperation *) node, tok + n, scope))) {
		n += res;
	} else if ((res = parseASTExp7(node, tok + n, scope))) {
		n += res;
	} else {
		freeASTNode(node);
		return 0;
	}

	return n;
}

int parseASTExp7(ASTNode *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTNode(node);
	if (astHasErr()) {
		freeASTNode(node);
		return 0;
	}

	if ((res = parseASTOperation7((ASTOperation *) node, tok + n, scope))) {
		n += res;
	} else if ((res = parseASTExp6(node, tok + n, scope))) {
		n += res;
	} else {
		freeASTNode(node);
		return 0;
	}

	return n;
}

int parseASTExp6(ASTNode *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTNode(node);
	if (astHasErr()) {
		freeASTNode(node);
		return 0;
	}

	if ((res = parseASTOperation6((ASTOperation *) node, tok + n, scope))) {
		n += res;
	} else if ((res = parseASTExp5(node, tok + n, scope))) {
		n += res;
	} else {
		freeASTNode(node);
		return 0;
	}

	return n;
}

int parseASTExp5(ASTNode *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTNode(node);
	if (astHasErr()) {
		freeASTNode(node);
		return 0;
	}

	if ((res = parseASTOperation5((ASTOperation *) node, tok + n, scope))) {
		n += res;
	} else if ((res = parseASTExp4(node, tok + n, scope))) {
		n += res;
	} else {
		freeASTNode(node);
		return 0;
	}

	return n;
}

int parseASTExp4(ASTNode *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTNode(node);
	if (astHasErr()) {
		freeASTNode(node);
		return 0;
	}

	if ((res = parseASTOperation4((ASTOperation *) node, tok + n, scope))) {
		n += res;
	} else if ((res = parseASTExp3(node, tok + n, scope))) {
		n += res;
	} else {
		freeASTNode(node);
		return 0;
	}

	return n;
}

int parseASTExp3(ASTNode *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTNode(node);
	if (astHasErr()) {
		freeASTNode(node);
		return 0;
	}

	if ((res = parseASTOperation3((ASTOperation *) node, tok + n, scope))) {
		n += res;
	} else if ((res = parseASTExp2(node, tok + n, scope))) {
		n += res;
	} else {
		freeASTNode(node);
		return 0;
	}

	return n;
}

int parseASTExp2(ASTNode *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTNode(node);
	if (astHasErr()) {
		freeASTNode(node);
		return 0;
	}

	if ((res = parseASTOperation2((ASTOperation *) node, tok + n, scope))) {
		n += res;
	} else if ((res = parseASTExp1(node, tok + n, scope))) {
		n += res;
	} else {
		freeASTNode(node);
		return 0;
	}

	return n;
}

int parseASTExp1(ASTNode *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;
	ASTNodeBuf tempBuf;
	ASTNode *tempNode = (ASTNode *) &tempBuf;

	initASTNode(node);
	if (astHasErr()) {
		freeASTNode(node);
		return 0;
	}

	if ((res = parseASTExpSing(node, tok + n, scope))) {
		n += res;
	} else {
		freeASTNode(node);
		return 0;
	}

	while (1) {
		if ((res = parseASTOperation1((ASTOperation *) tempNode, tok + n, scope, (ASTNode *) node))) {
			mvASTNode(node, tempNode);
			n += res;
		} else {
			return n;
		}
	}
}

int printASTExp(const ASTNode *node) {
	return printASTNode(node);
}

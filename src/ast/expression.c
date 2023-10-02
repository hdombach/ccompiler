#include <string.h>

#include "expression.h"
#include "identifier.h"
#include "intConstant.h"
#include "astUtil.h"
#include "node.h"
#include "operation.h"
#include "scope.h"

void initASTExp(ASTExp *node) {
	node->node.type = AST_UNKNOWN;
}

void freeASTExp(ASTExp *node) {
	freeASTNode(node);
}

int parseASTExp(ASTExp *node, Token const *tok, ASTScope const *scope) {
	return parseASTExp15(node, tok, scope);
}

int parseASTExpSing(ASTExp *node, Token const *tok, ASTScope const *scope) {
	//AST_VALID(ASTExp);
	int res, n = 0;

	initASTExp(node);

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

int parseASTExp15(ASTExp *node, const Token *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTExp(node);
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

int parseASTExp14(ASTExp *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTExp(node);
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

int parseASTExp13(ASTExp *node, const Token *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTExp(node);
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

int parseASTExp12(ASTExp *node, const Token *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTExp(node);
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

int parseASTExp11(ASTExp *node, const Token *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTExp(node);
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

int parseASTExp10(ASTExp *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTExp(node);
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

int parseASTExp9(ASTExp *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTExp(node);
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

int parseASTExp8(ASTExp *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTExp(node);
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

int parseASTExp7(ASTExp *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTExp(node);
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

int parseASTExp6(ASTExp *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTExp(node);
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

int parseASTExp5(ASTExp *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTExp(node);
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

int parseASTExp4(ASTExp *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTExp(node);
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

int parseASTExp3(ASTExp *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTExp(node);
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

int parseASTExp2(ASTExp *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;

	initASTExp(node);
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

int parseASTExp1(ASTExp *node, Token const *tok, ASTScope const *scope) {
	int n = 0, res;
	ASTNodeBuf tempBuf;
	ASTNode *tempNode = &tempBuf;

	initASTExp(node);
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

int printASTExp(const ASTExp *node) {
	return printASTNode(node);
}

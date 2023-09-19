#include "compStatement.h"
#include "../util/dlist.h"
#include "statement.h"
#include "astUtil.h"
#include "../util/callbacks.h"

void initASTCompStm(ASTCompStm *node) {
	initDListEmpty(&node->statements, sizeof(ASTStm));
}

void freeASTCompStm(ASTCompStm *node) {
	freeDList(&node->statements, (FreeFunc) freeASTStm);
}

int parseASTCompStm(
		ASTCompStm *node,
		const Token *tok,
		struct ASTScope const *scope)
{
	int res, n = 0;
	ASTStm stm;

	if (astHasErr()) {
		return 0;
	}

	if (tok[n].type != TT_O_CURLY) {
		return 0;
	}
	n++;

	initASTCompStm(node);
	while (!astHasErr()) {
		if ((res = parseASTStm(&stm, tok + n))) {
			dlistApp(&node->statements, &stm);
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


	return n;
}

int printASTCompStm(const ASTCompStm *node) {
	int n = 0;

	n += printDList(&node->statements, (PrintFunc) printASTStm);

	return n;
}

#include "block.h"
#include "../util/dlist.h"
#include "statement.h"
#include "astUtil.h"

void initASTBlock(ASTBlock *node) {
	initDList(&node->statements, sizeof(ASTStm));
}

void freeASTBlock(ASTBlock *node) {
	freeDList(&node->statements, (DListFreeFunc) freeASTStm);
}

int parseASTBlock(ASTBlock *node, const Token *tok) {
	int res, n = 0;
	ASTStm stm;

	if (astErrMsg) {
		return 0;
	}

	if (tok[n].type != TT_O_CURLY) {
		return 0;
	}
	n++;

	initASTBlock(node);
	while (!astErrMsg) {
		if ((res = parseASTStm(&stm, tok + n))) {
			dlistApp(&node->statements, &stm);
			n += res;
		} else {
			break;
		}
	}

	if (tok[n].type != TT_C_CURLY) {
		freeASTBlock(node);
		return 0;
	}
	n++;


	return n;
}

int printASTBlock(const ASTBlock *node) {
	int n = 0;

	n += printDList(&node->statements, (DListPrintFunc) printASTStm);

	return n;
}

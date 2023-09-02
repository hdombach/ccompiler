#include <stdlib.h>
#include <string.h>

#include "astUtil.h"
#include "macroUndef.h"

void initASTMacroUndef(ASTMacroUndef *node) {
	node->name = NULL;
}

void freeASTMacroUndef(ASTMacroUndef *node) {
	if (node->name) {
		free(node->name);
	}
}

int parseASTMacroUndef(ASTMacroUndef *node, const Token *tok) {
	int n = 0;

	initASTMacroUndef(node);
	if (astMacro(tok + n, TT_MACRO_UNDEF)) {
		n++;
	} else {
		freeASTMacroUndef(node);
		return 0;
	}

	if (astMacro(tok + n, TT_IDENTIFIER)) {
		node->name = strdup(tok[n].contents);
		n++;
	} else {
		freeASTMacroUndef(node);
		return 0;
	}

	return n;
}

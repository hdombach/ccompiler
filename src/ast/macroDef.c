#include <string.h>

#include "macroDef.h"
#include "../token.h"
#include "../util/dlist.h"
#include "tokenParser.h"
#include "../util/dstr.h"
#include "../util/util.h"

void initASTMacroDefNode(ASTMacroDefNode *node) {
	node->token = NULL;
	node->paramIndex = -1;
}

void freeASTMacroDefNode(ASTMacroDefNode *node) {
	if (node->token) {
		freeToken(node->token);
	}
}

void initASTMacroDef(ASTMacroDef *def) {
	def->name = NULL;
	initDList(&def->paramNames, sizeof(char *));
	initDList(&def->nodes, sizeof(ASTMacroDefNode));
}

void freeASTMacroDef(ASTMacroDef *def) {
	free(def->name);
	freeDList(&def->paramNames, (DListFreeFunc) freeStr);
	freeDList(&def->nodes, (DListFreeFunc) freeASTMacroDefNode);
}

int _parseParam(ASTMacroDef *def, const Token *tok) {
	int res, n;
	char *tempStr;
	n = 0;

	if (!tok[n].isMacro) return TP_FAILED;
	if (tok[n].type != TT_O_PARAN) return TP_FAILED;
	n++;

	if (!tok[n].isMacro) return TP_ERROR;
	if (tok[n].type != TT_IDENTIFIER)
		return tpError(tok + n, "Expected macro paramater name");
	tempStr = strdup(tok[n].contents);
	dlistApp(&def->paramNames, &tempStr);
	n++;

	while (1) {
		if (!tok[n].isMacro) return tpError(tok + n, "Expected )");
		if (tok[n].type != TT_COMMA) break;
		n++;

		if (!tpMacroType(tok + n, TT_IDENTIFIER))
			return tpError(tok + n, "Expected param name after ,");
		tempStr = strdup(tok[n].contents);
		dlistApp(&def->paramNames, &tempStr);
		n++;
	}

	if (!tpMacroType(tok + n, TT_C_PARAN))
		return tpError(tok + n, "Expected )");
	n++;

	return n;
}

int _parseReplList(ASTMacroDef *def, const Token *tok) {
	int res, n;
	char *tempStr;

	n = 0;

	while (1) {
		ASTMacroDefNode node;

		if (!tok[n].isMacro) break;

		initASTMacroDefNode(&node);
		tokenDup(&tok[n], node.token);

		if (node.token->type == TT_IDENTIFIER) {
			for (int i = 0; i < def->paramNames.size; i++) {
				if (strcmp(*(char**) dlistGet(&def->paramNames, i), node.token->contents) == 0) {
					node.paramIndex = i;
				}
			}
		}

		dlistApp(&def->nodes, &node);
	}
	return n;
}

int parseASTMacroDef(ASTMacroDef *def, const Token *tok) {
	int res, n;

	n = 0;
	if (!tok[n].isMacro) return TP_FAILED;
	if (tok[n].type != TT_MACRO_DEFINE) return TP_FAILED;
	n++;

	if (!tpMacroType(tok + n, TT_IDENTIFIER))
		return tpError(tok + n, "Expect macro name");
	if (def->name) {
		free(def->name);
	}
	def->name = strdup(tok[n].contents);
	n++;

	n += res = _parseParam(def, tok + n);
	if (res < 0) return res;

	n += res = _parseReplList(def, tok + n);
	if (res < 0) return res;

	return n;
}

void printASTMacroDefNode(ASTMacroDefNode const *node) {
	printToken(node->token);
}

void printASTMacroDef(ASTMacroDef const *def) {
	printf("{");

	printf("\"name\": ");
	printJsonStr(def->name);

	printf(", \"params\": ");
	dlistPrint(&def->paramNames, (DListPrintFunc) printJsonStr);

	printf(", \"content\": ");
	dlistPrint(&def->nodes, (DListPrintFunc) printASTMacroDefNode);

	printf("}");
}

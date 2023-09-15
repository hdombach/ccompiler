#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "macroDef.h"
#include "../token.h"
#include "../util/dlist.h"
#include "tokenParser.h"
#include "../util/util.h"
#include "astUtil.h"
#include "../util/callbacks.h"

void _initASTMacroDefNode(ASTMacroDefNode *node) {
	node->token = NULL;
	node->paramIndex = -1;
}

void _freeASTMacroDefNode(ASTMacroDefNode *node) {
	if (node->token) {
		freeToken(node->token);
		free(node->token);
	}
}

void initASTMacroDef(ASTMacroDef *def) {
	def->name = NULL;
	def->insertFunc = NULL;
	initDList(&def->paramNames, sizeof(char *));
	initDList(&def->nodes, sizeof(ASTMacroDefNode));
}

void initASTMacroDefn(ASTMacroDef *def, char *name) {
	def->name = name;
	def->insertFunc = NULL;
	initDList(&def->paramNames, sizeof(char *));
	initDList(&def->nodes, sizeof(ASTMacroDefNode));
}

void initASTMacroDefDefault(
		ASTMacroDef *def,
		char *name,
		ASTMacroDefInsert insertFunc)
{
	def->name = name;
	def->insertFunc = insertFunc;
	initDList(&def->paramNames, sizeof(char *));
	initDList(&def->nodes, sizeof(ASTMacroDefNode));
}

void freeASTMacroDef(ASTMacroDef *def) {
	if (def->name) {
		free(def->name);
	}
	freeDList(&def->paramNames, (FreeFunc) freeStr);
	freeDList(&def->nodes, (FreeFunc) _freeASTMacroDefNode);
}

int _parseParam(ASTMacroDef *def, Token const *tok) {
	char *tempStr;
	int n = 0;

	if (astHasErr()) {
		return 0;
	}

	if (astMacro(tok + n, TT_O_PARAN)) {
		n++;
	} else {
		return 0;
	}

	if (astMacro(tok + n, TT_IDENTIFIER)) {
		tempStr = strdup(tok[n].contents);
		dlistApp(&def->paramNames, &tempStr);
		n++;
	} else {
		astErr("Expected param name", tok + n);
		return 0;
	}

	while (1) {
		int subN = n;
		if (!tok[subN].isMacro) {
			astErr("Expected )", tok + n);
			return 0;
		}
		if (tok[subN].type != TT_COMMA) {
			break;
		}
		subN++;

		if (!astMacro(tok + subN, TT_IDENTIFIER)) {
			astErr("Expected macro param name", tok + n);
			return 0;
		}
		tempStr = strdup(tok[subN].contents);
		dlistApp(&def->paramNames, &tempStr);
		subN++;

		n = subN;
	}

	if (!astMacro(tok + n, TT_C_PARAN)) {
		return 0;
	}
	n++;

	return n;
}

int _parseReplList(ASTMacroDef *def, Token const *tok) {
	char *tempStr;
	int n = 0;

	if (astHasErr()) {
		return 0;
	}


	while (1) {
		ASTMacroDefNode node;

		if (!tok[n].isMacro) {
			break;
		}

		_initASTMacroDefNode(&node);
		node.token = malloc(sizeof(Token));
		cpToken(node.token, tok + n);

		if (node.token->type == TT_IDENTIFIER) {
			for (int i = 0; i < def->paramNames.size; i++) {
				if (strcmp(*(char**) dlistGet(&def->paramNames, i), node.token->contents) == 0) {
					node.paramIndex = i;
					break;
				}
			}
		}

		dlistApp(&def->nodes, &node);
		n++;
	}
	return n;
}

int parseASTMacroDef(ASTMacroDef *def, Token const *tok) {
	int n = 0, res;

	if (astHasErr()) {
		return 0;
	}

	initASTMacroDef(def);

	if (astMacro(tok + n, TT_MACRO_DEFINE)) {
		n++;
	} else {
		freeASTMacroDef(def);
		return 0;
	}

	if (astMacro(tok + n, TT_IDENTIFIER)) {
		def->name = strdup(tok[n].contents);
		n++;
	} else {
		astErr("Expected name after macro definition", tok + n);
		freeASTMacroDef(def);
		return 0;
	}

	if ((res = _parseParam(def, tok + n))) {
		n += res;
	} else if (astHasErr()) {
		freeASTMacroDef(def);
		return 0;
	}

	if ((res = _parseReplList(def, tok + n))) {
		n += res;
	} else if (astHasErr()) {
		freeASTMacroDef(def);
		return 0;
	}

	return n;
}

int printASTMacroDefNode(ASTMacroDefNode const *node) {
	int n = 0;
	if (node->paramIndex >= 0) {
		n += printf("{");

		n += printf("\"paramIndex\": %d", node->paramIndex);

		n += printf(", \"token\": ");
		n += printToken(node->token);

		n += printf("}");
	} else {
		n += printToken(node->token);
	}

	return n;
}

int printASTMacroDef(ASTMacroDef const *def) {
	int n = 0;
	n += printf("{");

	n += printf("\"type\": \"Macro Definition\"");

	n += printf(", \"name\": ");
	n += printJsonStr(def->name);

	n += printf(", \"params\": ");
	n += printDList(&def->paramNames, (PrintFunc) printJsonStrp);

	n += printf(", \"content\": ");
	n += printDList(&def->nodes, (PrintFunc) printASTMacroDefNode);

	n += printf("}");
	return n;
}

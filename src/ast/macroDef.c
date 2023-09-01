#include <malloc/_malloc.h>
#include <stdio.h>
#include <string.h>

#include "macroDef.h"
#include "../token.h"
#include "../util/dlist.h"
#include "tokenParser.h"
#include "../util/dstr.h"
#include "../util/util.h"
#include "astState.h"

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
	initDList(&def->paramNames, sizeof(char *));
	initDList(&def->nodes, sizeof(ASTMacroDefNode));
}

void initASTMacroDefn(ASTMacroDef *def, char *name) {
	def->name = name;
	initDList(&def->paramNames, sizeof(char *));
	initDList(&def->nodes, sizeof(ASTMacroDefNode));
}

void freeASTMacroDef(ASTMacroDef *def) {
	if (def->name) {
		free(def->name);
	}
	freeDList(&def->paramNames, (DListFreeFunc) freeStr);
	freeDList(&def->nodes, (DListFreeFunc) _freeASTMacroDefNode);
}

int _parseParam(ASTMacroDef *def, Token *tok) {
	char *tempStr;
	int n = 0;

	if (astErrMsg) {
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
		astErrMsg = "Expected param name";
		return 0;
	}

	while (1) {
		int subN = n;
		if (!tok[subN].isMacro) {
			astErrMsg = "Expected )";
			return 0;
		}
		if (tok[subN].type != TT_COMMA) {
			break;
		}
		subN++;

		if (!astMacro(tok + subN, TT_IDENTIFIER)) {
			astErrMsg = "Expected macro param name";
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

void _parseReplList(ASTMacroDef *def, ASTState *parentState) {
	char *tempStr;
	Token *tempTok;
	ASTState state, subState;

	state = *parentState;

	if (!astValid(&state)) {
		return;
	}


	while (1) {
		ASTMacroDefNode node;

		subState = state;

		tempTok = astPop(&subState);
		if (!tempTok) break;
		if (!tempTok->isMacro) subState.status = AST_STATUS_FAILED;

		if (!astValid(&subState)) {
			break;
		}

		_initASTMacroDefNode(&node);
		node.token = malloc(sizeof(Token));
		tokenDup(tempTok, node.token);

		if (node.token->type == TT_IDENTIFIER) {
			for (int i = 0; i < def->paramNames.size; i++) {
				if (strcmp(*(char**) dlistGet(&def->paramNames, i), node.token->contents) == 0) {
					node.paramIndex = i;
					break;
				}
			}
		}

		dlistApp(&def->nodes, &node);
		astMergeState(&state, &subState);
	}
	astMergeState(parentState, &state);
}

int parseASTMacroDef(ASTMacroDef *def, ASTState *parentState) {
	ASTState state, subState;
	Token *tempToken;

	if (!astValid(parentState)) {
		return 0;
	}

	initASTMacroDef(def);

	state = *parentState;
	astExpMacro(&state, TT_MACRO_DEFINE);

	tempToken = astReqMacro(&state, TT_IDENTIFIER);
	if (tempToken) {
		def->name = strdup(tempToken->contents);
	} else {
		freeASTMacroDef(def);
		return 0;
	}

	int res;
	if (astValid(&state) && (res = _parseParam(def, state.tok))) {
		state.tok += res;
	} else if (astErrMsg) {
		astError(&state, astErrMsg);
	}
	_parseReplList(def, &state);

	astMergeState(parentState, &state);
	if (astValid(&state)) {
		return 1;
	} else {
		freeASTMacroDef(def);
		return 0;
	}
}

void printASTMacroDefNode(ASTMacroDefNode const *node) {
	if (node->paramIndex >= 0) {
		printf("{");

		printf("\"paramIndex\": %d", node->paramIndex);

		printf(", \"token\": ");
		printToken(node->token);

		printf("}");
	} else {
		printToken(node->token);
	}
}

void printASTMacroDef(ASTMacroDef const *def) {
	printf("{");

	printf("\"type\": \"Macro Definition\"");

	printf(", \"name\": ");
	printJsonStr(def->name);

	printf(", \"params\": ");
	dlistPrint(&def->paramNames, (DListPrintFunc) printJsonStrp);

	printf(", \"content\": ");
	dlistPrint(&def->nodes, (DListPrintFunc) printASTMacroDefNode);

	printf("}");
}

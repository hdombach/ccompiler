#include "macroInclude.h"
#include "astState.h"
#include "../util/dstr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initASTMacroIncl(ASTMacroIncl *node) {
	node->name = NULL;
}

void freeASTMacroIncl(ASTMacroIncl *node) {
	if (node->name) {
		free(node->name);
	}
}

int parseASTMacroIncl(ASTMacroIncl *node, ASTState *parentState) {
	ASTState state;
	Token *tempToken;
	DStr str;

	if (!astValid(parentState)) {
		return 0;
	}

	initASTMacroIncl(node);

	state = *parentState;
	astExpMacro(&state, TT_MACRO_INCLUDE);
	if (!astValid(&state)) {
		return 0;
	}

	tempToken = astPop(&state);
	if (!astValid(&state)) {
		astError(&state, "Expecting token following #include");
	} else if (tempToken->type == TT_STR_CONSTANT) {
		node->name = strdup(tempToken->contents);
		node->type = AST_MIT_DIRECT;
	} else if (tempToken->type == TT_LESS) {
		DStr currFile;

		initDStr(&currFile);
		while (1) {
			tempToken = astPop(&state);
			if (!astValid(&state) || tempToken->type == TT_GREATER) {
				break;
			}

			if (tempToken->type == TT_IDENTIFIER) {
				dstrAppStr(&currFile, tempToken->contents);
			} else if (tempToken->type == TT_PERIOD) {
				dstrApp(&currFile, '.');
			} else if (tempToken->type == TT_DIV) {
				dstrApp(&currFile, '/');
			} else {
				astError(&state, "Unexpected token in library file name.");
				break;
			}
		}
		node->name = currFile.data;
		node->type = AST_MIT_LIBRARY;

		//Don'e need to free currFile since node now owns data
	} else {
		astError(&state, "Unexpected token following #include");
	}

	astMergeState(parentState, &state);

	return 1;
}

void printASTMacroInclType(ASTMacroInclType type) {
	switch (type) {
		case AST_MIT_DIRECT:
			printf("Local File");
			break;
		case AST_MIT_LIBRARY:
			printf("library File");
			break;
	}
}

void printASTMacroIncl(const ASTMacroIncl *node) {
	printf("{");

	printf("\"type\": \"Macro Include\"");

	printf(", \"filename\": ");
	printJsonStr(node->name);

	printf(", \"filetype\": \"");
	printASTMacroInclType(node->type);
	printf("\"");

	printf("}");
}

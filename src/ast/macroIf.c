#include <stdio.h>
#include <stdlib.h>

#include "macroIf.h"
#include "astState.h"
#include "../util/macroDict.h"

void freeASTMacroIf(ASTMacroIf *node) {
	if (node->next) {
		freeASTMacroIf(node->next);
		free(node->next);
	}
}

int parseASTMacroIf(ASTMacroIf *node, ASTState *parentState, MacroDict *macros) {
	ASTState state;
	Token *tok;
	char isTrue;
	ASTMacroIf *last = node;
	int i = 0;

	if (!astValid(parentState)) {
		return 0;
	}

	state = *parentState;

	tok = astPopMacro(&state);
	if (!tok) {
		return 0;
	} else if (tok->type == TT_MACRO_IF) {
		fprintf(stderr, "Macro if is not implimented yet.\n");
		//TODO
		return 0;
	} else if (tok->type == TT_MACRO_IFDEF) {
		last->start = i;
		if ((tok = astExpMacro(&state, TT_IDENTIFIER))) {
			isTrue = macroDictPresent(macros, tok->contents);
			i += 2;
			last->end = i;
		} else {
			return 0;
		}
	} else {
		return 0;
	}

	while (1) {
		if (!isTrue) {
			last->end++;
		}
		i++;
		tok = astPop(&state);
		if (!tok || tok->type == TT_EOF) {
			astError(parentState, "Expecting #endif");
			return 0;
		}
		if (tok->type == TT_MACRO_ELSE) {
			if (isTrue) {
				last->next = malloc(sizeof(ASTMacroIf));
				last = last->next;
				last->start = i-1;
				last->end = i+1;
			}
			isTrue = !isTrue;
		} else if (tok->type == TT_MACRO_ENDIF) {
			if (isTrue) {
				last->next = malloc(sizeof(ASTMacroIf));
				last = last->next;
				last->start = i-1;
				last->end = i+1;
			}
			break;
		} else if (tok->type == TT_MACRO_ELIF) {
			fprintf(stderr, "Macro elif is not implimented yet\n");
			//TODO
			return 0;
		}
	}

	return 1;
}

void printASTMacroIf(const ASTMacroIf *node) {
	printf("{");
	printf("\"type\": \"Macro if\"");

	printf(", \"ranges\": [");
	ASTMacroIf const *cur = node;
	printf("\"%d-%d\"", cur->start, cur->end);
	while (cur->next) {
		cur = cur->next;
		printf(", \"%d-%d\"", cur->start, cur->end);
	}
	printf("]");

	printf("}");
}

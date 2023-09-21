#include <stdio.h>
#include <stdlib.h>

#include "macroIf.h"
#include "astUtil.h"
#include "../util/macroDict.h"

void initASTMacroIf(ASTMacroIf *node) {
	node->next = NULL;
}

void freeASTMacroIf(ASTMacroIf *node) {
	if (node->next) {
		freeASTMacroIf(node->next);
		free(node->next);
	}
}

int _isMacroIf(TokenType type) {
	return type == TT_MACRO_IF ||
		type == TT_MACRO_IFDEF ||
		type == TT_MACRO_IFNDEF;
}

int parseASTMacroIf(ASTMacroIf *node, Token const *tok, MacroDict *macros) {
	char isTrue;
	initASTMacroIf(node);
	ASTMacroIf *last = node;
	int n = 0, resN = 0;

	if (astHasErr()) {
		return 0;
	}

	if (astMacro(tok + n, TT_MACRO_IF)) {
		fprintf(stderr, "Macro if is not implimented yet.\n");
		//TODO;
		freeASTMacroIf(node);
		return 0;
	} else if (astMacro(tok + n, TT_MACRO_IFDEF)) {
		last->start = n;
		n++;
		if (astMacro(tok + n, TT_IDENTIFIER)) {
			isTrue = macroDictPresent(macros, tok[n].contents);
			n++;
			last->end = n;
		} else {
			freeASTMacroIf(node);
			return 0;
		}
		resN = n;
	} else if (astMacro(tok + n, TT_MACRO_IFNDEF)) {
		last->start = n;
		n++;
		if (astMacro(tok + n, TT_IDENTIFIER)) {
			isTrue = !macroDictPresent(macros, tok[n].contents);
			n++;
			last->end = n;
		} else {
			freeASTMacroIf(node);
			return 0;
		}
		resN = n;
	} else {
		freeASTMacroIf(node);
		return 0;
	}

	while (1) {
		if (!isTrue) {
			last->end++;
		}
		if (tok[n].type == TT_EOF) {
			astErr("Expecting #endif", tok + n);
			freeASTMacroIf(node);
			return 0;
		} else if (_isMacroIf(tok[n].type)) {
			n++;
			while (1) {
				if (!isTrue) {
					last->end++;
				}
				if (tok[n].type == TT_EOF) {
					astErr("Expected #endif", tok + n);
					freeASTMacroIf(node);
					return 0;
				} else if (tok[n].type == TT_MACRO_ENDIF) {
					if (!isTrue) {
						last->end++;
					}
					n++;
					break;
				}
				n++;
			}
		} else if (tok[n].type == TT_MACRO_ELSE) {
			if (isTrue) {
				last->next = malloc(sizeof(ASTMacroIf));
				last = last->next;
				last->start = n;
				last->end = n+2;
			}
			isTrue = !isTrue;
		} else if (tok[n].type == TT_MACRO_ENDIF) {
			if (isTrue) {
				last->next = malloc(sizeof(ASTMacroIf));
				last = last->next;
				last->start = n;
				last->end = n+2;
			}
			break;
		} else if (tok->type == TT_MACRO_ELIF) {
			fprintf(stderr, "Macro elif is not implimented yet\n");
			//TODO
			freeASTMacroIf(node);
			return 0;
		}
		n++;
	}

	return resN;
}

void printASTMacroIf(const ASTMacroIf *node) {
	printf("{");
	printf("\"node type\": \"Macro if\"");

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

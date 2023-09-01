#pragma once

#include "astState.h"
#include "../util/macroDict.h"

/*
 * Returns what indexes should be removed
 */
typedef struct _ASTMacroIf {
	int start, end;
	struct _ASTMacroIf *next;
} ASTMacroIf;

void freeASTMacroIf(ASTMacroIf *node);

int parseASTMacroIf(ASTMacroIf *node, ASTState *state, MacroDict *dict);

void printASTMacroIf(ASTMacroIf const *node);

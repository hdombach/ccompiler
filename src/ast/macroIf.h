#pragma once

#include "astUtil.h"
#include "../util/macroDict.h"

/*
 * Returns what indexes should be removed
 */
typedef struct _ASTMacroIf {
	int start, end;
	struct _ASTMacroIf *next;
} ASTMacroIf;

void initASTMacroIf(ASTMacroIf *node);
void freeASTMacroIf(ASTMacroIf *node);

int parseASTMacroIf(ASTMacroIf *node, Token const *tok, MacroDict *dict);

void printASTMacroIf(ASTMacroIf const *node);

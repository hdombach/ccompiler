#pragma once

#include "../token.h"

typedef struct {
	char *name;
} ASTMacroUndef;

void initASTMacroUndef(ASTMacroUndef *node);
void freeASTMacroUndef(ASTMacroUndef *node);

int parseASTMacroUndef(ASTMacroUndef *node, Token const *tok);

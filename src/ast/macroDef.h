#pragma once

#include "../token.h"
#include "../util/dlist.h"

typedef struct _MacroDefNode {
	Token *token;
	int paramIndex;
} ASTMacroDefNode;

typedef struct {
	char *name;
	DList paramNames;
	DList nodes;
} ASTMacroDef;

void initASTMacroDefNode(ASTMacroDefNode *node);
void freeASTMacroDefNode(ASTMacroDefNode *node);

void initASTMacroDef(ASTMacroDef *def);
void freeASTMacroDef(ASTMacroDef *def);

int parseASTMacroDef(ASTMacroDef *def, Token const *tokens);

void printASTMacroDefNode(ASTMacroDefNode const *node);
void printASTMacroDef(ASTMacroDef const *def);

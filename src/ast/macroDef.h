#pragma once

#include "../token.h"
#include "../util/dlist.h"
#include "astState.h"

typedef struct _MacroDefNode {
	Token *token;
	int paramIndex;
} ASTMacroDefNode;

typedef struct {
	char *name;
	DList paramNames;
	DList nodes;
} ASTMacroDef;

void _initASTMacroDefNode(ASTMacroDefNode *node);
void _freeASTMacroDefNode(ASTMacroDefNode *node);

void initASTMacroDef(ASTMacroDef *def);
void initASTMacroDefn(ASTMacroDef *def, char *name);
void freeASTMacroDef(ASTMacroDef *def);

int parseASTMacroDef(ASTMacroDef *def, Token const *tok);

void printASTMacroDefNode(ASTMacroDefNode const *node);
void printASTMacroDef(ASTMacroDef const *def);

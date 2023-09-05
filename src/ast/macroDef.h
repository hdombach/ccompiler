#pragma once

#include "../token.h"
#include "../util/dlist.h"
#include "../util/tokList.h"
#include "astUtil.h"

typedef struct _MacroDefNode {
	Token *token;
	int paramIndex;
} ASTMacroDefNode;

typedef void (*ASTMacroDefInsert)(TokList*, Token const*);

typedef struct {
	char *name;
	DList paramNames;
	DList nodes;
	ASTMacroDefInsert insertFunc;
} ASTMacroDef;

void _initASTMacroDefNode(ASTMacroDefNode *node);
void _freeASTMacroDefNode(ASTMacroDefNode *node);

void initASTMacroDef(ASTMacroDef *def);
void initASTMacroDefn(ASTMacroDef *def, char *name);
void initASTMacroDefDefault(
		ASTMacroDef *def,
		char *name,
		ASTMacroDefInsert insertFunc);
void freeASTMacroDef(ASTMacroDef *def);

int parseASTMacroDef(ASTMacroDef *def, Token const *tok);

int printASTMacroDefNode(ASTMacroDefNode const *node);
int printASTMacroDef(ASTMacroDef const *def);

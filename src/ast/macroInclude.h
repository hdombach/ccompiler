#pragma once

#include "astUtil.h"
typedef enum {
	AST_MIT_DIRECT,
	AST_MIT_LIBRARY,
} ASTMacroInclType;

typedef struct {
	ASTMacroInclType type;
	char *filename;
} ASTMacroIncl;

void initASTMacroIncl(ASTMacroIncl *node);
void freeASTMacroIncl(ASTMacroIncl *node);

int parseASTMacroIncl(ASTMacroIncl *node, Token const *tok);

void printASTMacroIncl(ASTMacroIncl const *node);
void printASTMacroInclType(ASTMacroInclType type);

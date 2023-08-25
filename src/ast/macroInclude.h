#pragma once

#include "astState.h"
typedef enum {
	AST_MIT_DIRECT,
	AST_MIT_LIBRARY,
} ASTMacroInclType;

typedef struct {
	ASTMacroInclType type;
	char *name;
} ASTMacroIncl;

void initASTMacroIncl(ASTMacroIncl *node);
void freeASTMacroIncl(ASTMacroIncl *node);

int parseASTMacroIncl(ASTMacroIncl *node, ASTState *state);

void printASTMacroIncl(ASTMacroIncl const *node);
void printASTMacroInclType(ASTMacroInclType type);

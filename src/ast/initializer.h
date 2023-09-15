#pragma once

#include "expression.h"
#include "../token.h"

typedef enum {
	AST_IT_UNKNOWN,
	AST_IT_EXP,
	AST_IT_LIST,
} ASTInitializerType;

typedef struct {
	ASTInitializerType type;
	union {
		ASTExp exp;
	} c;
} ASTInitializer;

void initASTInitializer(ASTInitializer *initializer);
void freeASTInitializer(ASTInitializer *initializer);
int parseASTInitializer(ASTInitializer *initializer, Token const *tok);
int printASTInitializer(ASTInitializer const *initializer);

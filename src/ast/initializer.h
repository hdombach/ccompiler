#pragma once

#include "expression.h"
#include "../token.h"
#include "../util/dlist.h"

typedef enum {
	AST_IT_UNKNOWN,
	AST_IT_EXP,
	AST_IT_LIST,
} ASTInitializerType;

typedef struct {
	ASTInitializerType type;
	union {
		ASTExp exp;
		DList initializerList;
	} c;
} ASTInitializer;

void initASTInitializer(ASTInitializer *initializer);
void freeASTInitializer(ASTInitializer *initializer);
int parseASTInitializer(ASTInitializer *initializer, Token const *tok);
int printASTInitializer(ASTInitializer const *initializer);

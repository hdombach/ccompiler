#pragma once

#include "../token.h"
#include "node.h"

typedef struct {
	ASTNode node;
	unsigned long int value;
} ASTIntConstant;

int parseASTIntConstant(ASTIntConstant *node, Token const *tok);

int printASTIntContant(ASTIntConstant const *node);

int astIntConstantChildCount(ASTIntConstant const *node);
ASTNode *astIntConstantGetChild(ASTIntConstant *node, int index);

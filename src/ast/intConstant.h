#pragma once

#include "../token.h"

typedef struct {
	unsigned long int value;
} ASTIntConstant;

int parseASTIntConstant(ASTIntConstant *node, Token const *tok);

int printASTIntContant(ASTIntConstant const *node);

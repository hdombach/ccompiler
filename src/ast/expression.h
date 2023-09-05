#pragma once

#include "intConstant.h"

typedef enum {
	ASTE_UNKNOWN,
	ASTE_INT_CONSTANT,
} ASTExpType;

typedef struct {
	ASTExpType type;
	union {
		ASTIntConstant intConstant;
	} c;
} ASTExp;

void initASTExp(ASTExp *node);
void freeASTExp(ASTExp *node);

int parseASTExp(ASTExp *node, Token const *tok);

int printASTExp(ASTExp const *node);

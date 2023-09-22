#pragma once

#include "intConstant.h"
#include "operation.h"

typedef enum {
	ASTE_UNKNOWN,
	ASTE_INT_CONSTANT,
	ASTE_OPERATION,
} ASTExpType;

typedef struct ASTExp {
	ASTExpType type;
	union {
		ASTIntConstant intConstant;
		ASTOperation operation;	
	} c;
} ASTExp;

void initASTExp(ASTExp *node);
void freeASTExp(ASTExp *node);

int parseASTExp(ASTExp *node, Token const *tok);
int parseASTExpSing(ASTExp *node, Token const *tok);
int parseASTExp15(ASTExp *node, Token const *tok);
int parseASTExp14(ASTExp *node, Token const *tok);
int parseASTExp13(ASTExp *node, Token const *tok);

int printASTExp(ASTExp const *node);

#pragma once

#include "intConstant.h"
#include "operation.h"

typedef enum {
	ASTE_UNKNOWN,
	ASTE_INT_CONSTANT,
	ASTE_OPERATION,
	ASTE_IDENTIFIER,
} ASTExpType;

typedef struct ASTExp {
	ASTExpType type;
	union {
		ASTIntConstant intConstant;
		ASTOperation operation;	
		char *identifier;
	} c;
} ASTExp;

void initASTExp(ASTExp *node);
void freeASTExp(ASTExp *node);

int parseASTExp(ASTExp *node, Token const *tok);
int parseASTExpSing(ASTExp *node, Token const *tok);
int parseASTExp15(ASTExp *node, Token const *tok);
int parseASTExp14(ASTExp *node, Token const *tok);
int parseASTExp13(ASTExp *node, Token const *tok);
int parseASTExp12(ASTExp *node, Token const *tok);
int parseASTExp11(ASTExp *node, Token const *tok);
int parseASTExp10(ASTExp *node, Token const *tok);
int parseASTExp9(ASTExp *node, Token const *tok);
int parseASTExp8(ASTExp *node, Token const *tok);
int parseASTExp7(ASTExp *node, Token const *tok);

int printASTExp(ASTExp const *node);

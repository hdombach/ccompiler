#pragma once

#include "expression.h"

typedef enum {
	ASTS_UNKNOWN,
	ASTS_EXP,
} ASTStmType;

typedef struct {
	ASTStmType type;	
	union {
		ASTExp exp;
	} c;
} ASTStm;

void initASTStm(ASTStm *node);
void freeASTStm(ASTStm *node);

int parseASTStm(ASTStm *node, Token const *tok);

int printASTStm(ASTStm *node);

#pragma once

#include "expression.h"

typedef enum {
	ASTS_UNKNOWN,
	ASTS_EXP,
} ASTStmType;

struct ASTScope;

typedef struct {
	ASTStmType type;	
	union {
		ASTExp exp;
	} c;
} ASTStm;

void initASTStm(ASTStm *node);
void freeASTStm(ASTStm *node);

int parseASTStm(ASTStm *node, Token const *tok, struct ASTScope *scope);

int printASTStm(ASTStm const *node);

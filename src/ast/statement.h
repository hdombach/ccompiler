#pragma once

#include "expression.h"
#include "if.h"

typedef enum {
	ASTS_UNKNOWN,
	ASTS_COMPOUND,
	ASTS_EXP,
	ASTS_IF,
} ASTStmType;

struct ASTScope;
struct ASTCompStm;

typedef struct ASTStm {
	ASTStmType type;	
	union {
		struct ASTCompStm *compStm;
		ASTExp exp;
		ASTIf ifStm;
	} c;
} ASTStm;

void initASTStm(ASTStm *node);
void freeASTStm(ASTStm *node);

int parseASTStm(ASTStm *node, Token const *tok, struct ASTScope *scope);

int printASTStm(ASTStm const *node);

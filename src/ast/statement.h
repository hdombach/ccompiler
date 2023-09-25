#pragma once

#include "expression.h"
#include "if.h"
#include "label.h"

typedef enum {
	ASTS_UNKNOWN,
	ASTS_COMPOUND,
	ASTS_EXP,
	ASTS_IF,
	ASTS_EMPTY,
	ASTS_BREAK,
	ASTS_CONTINUE,
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
	ASTLabel *label;
} ASTStm;

void initASTStm(ASTStm *node);
void freeASTStm(ASTStm *node);

int parseASTStm(ASTStm *node, Token const *tok, struct ASTScope *scope);

int printASTStm(ASTStm const *node);

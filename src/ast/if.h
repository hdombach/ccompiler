#pragma once

#include "node.h"

struct ASTStm;
struct Token;
struct ASTScope;
struct ASTNode;

typedef struct ASTIf {
	ASTNode node;
	struct ASTNode *expression;
	struct ASTStm *trueStatement;
	struct ASTStm *falseStatement;
} ASTIf;

void initASTIf(ASTIf *node, Token const *tok);
void freeASTIf(ASTIf *node);
int parseASTIf(ASTIf *node, struct Token const *tok, struct ASTScope *scope);
int printASTIf(ASTIf const *node);
ASTTravRes astIfTrav(
		ASTIf *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);

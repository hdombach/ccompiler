#pragma once

#include "node.h"
#include "../token.h"
#include "../sem/scope.h"

typedef struct ASTFor {
	ASTNode node;
	ASTNode *initClause;
	ASTNode *condExp;
	ASTNode *iterExp;
	ASTNode *loopStm;
} ASTFor;

void initASTFor(ASTFor *node, Token const *tok);
void freeASTFor(ASTFor *node);
int parseASTFor(
		ASTFor *node,
		Token const *tok,
		ASTScope const *scope);
int printASTFor(ASTFor const *node);
ASTTravRes astForTrav(
		ASTFor *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);

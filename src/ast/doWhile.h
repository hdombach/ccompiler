#pragma once

#include "node.h"

struct ASTStm;
struct Token;
struct ASTScope;

typedef struct ASTDoWhile {
	ASTNode node;
	struct ASTNode *expression;
	struct ASTStm *statement;
} ASTDoWhile;

void initASTDoWhile(ASTDoWhile *node, struct Token const *tok);
void freeASTDoWhile(ASTDoWhile *node);
int parseASTDoWhile(
		ASTDoWhile *node,
		struct Token const *tok,
		struct ASTScope const *scope);
int printASTDoWhile(ASTDoWhile const *node);
ASTTravRes astDoWhileTrav(
		ASTDoWhile *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);

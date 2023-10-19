#pragma once

#include "node.h"

struct ASTNode;
struct ASTStm;
struct Token;
struct ASTScope;

typedef struct ASTWhile {
	ASTNode node;
	struct ASTNode *expression;
	struct ASTStm *statement;
} ASTWhile;

void initASTWhile(ASTWhile *node, struct Token const *tok);
void freeASTWhile(ASTWhile *node);
int parseASTWhile(
		ASTWhile *node,
		struct Token const *tok,
		struct ASTScope const *scope);
int printASTWhile(ASTWhile const *node);

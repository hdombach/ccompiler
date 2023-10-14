#pragma once

#include "node.h"

struct ASTNode;
struct ASTStm;
struct Token;
struct ASTScope;

typedef struct ASTSwitch {
	ASTNode node;
	struct ASTNode *expression;
	struct ASTStm *statement;
} ASTSwitch;

void initASTSwitch(ASTSwitch *node);
void freeASTSwitch(ASTSwitch *node);
int parseASTSwitch(
		ASTSwitch *node,
		struct Token const *tok,
		struct ASTScope const *scope);
int printASTSwitch(ASTSwitch const *node);

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

void initASTSwitch(ASTSwitch *node, struct Token const *tok);
void freeASTSwitch(ASTSwitch *node);
int parseASTSwitch(
		ASTSwitch *node,
		struct Token const *tok,
		struct ASTScope const *scope);
int printASTSwitch(ASTSwitch const *node);
ASTTravRes astSwitchTrav(
		ASTSwitch *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);


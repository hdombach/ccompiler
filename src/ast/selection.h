#pragma once

#include "node.h"

typedef struct Token Token;
typedef struct ASTScope ASTScope;
typedef struct ASTNode ASTNode;

/************************************************************
 * If 
 ************************************************************/

typedef struct ASTIf {
	ASTNode node;
	struct ASTNode *expression;
	ASTNode *trueStatement;
	ASTNode *falseStatement;
} ASTIf;

void initASTIf(ASTIf *node, Token const *tok);
void freeASTIf(ASTIf *node);
int parseASTIf(ASTIf *node, struct Token const *tok, struct ASTScope *scope);
int printASTIf(ASTIf const *node);
int astIfChildCount(ASTIf const *node);
ASTNode *astIfGetChild(ASTIf *node, int index);

/************************************************************
 * Switch 
 ************************************************************/

typedef struct ASTSwitch {
	ASTNode node;
	struct ASTNode *expression;
	ASTNode *statement;
} ASTSwitch;

void initASTSwitch(ASTSwitch *node, struct Token const *tok);
void freeASTSwitch(ASTSwitch *node);
int parseASTSwitch(
		ASTSwitch *node,
		struct Token const *tok,
		struct ASTScope *scope);
int printASTSwitch(ASTSwitch const *node);
int astSwitchChildCount(ASTSwitch *node);
ASTNode *astSwitchGetChild(ASTSwitch *node, int index);

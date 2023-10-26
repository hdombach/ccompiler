#pragma once

#include "node.h"

struct Token;
struct ASTScope;

typedef struct ASTLblIdentifier {
	ASTNode node;
	char *name;
} ASTLblIdentifier;

void initASTLblIdentifier(ASTLblIdentifier *node, struct Token const *tok);
void freeASTLblIdentifier(ASTLblIdentifier *node);
int parseASTLblIdentifier(
		ASTLblIdentifier *node,
		struct Token const *tok,
		struct ASTScope *scope);
int printASTLblIdentifier(ASTLblIdentifier const *node);

typedef struct ASTLblCase {
	ASTNode node;
	struct ASTNode *expression;
} ASTLblCase;

void initASTLblCase(ASTLblCase *node, struct Token const *tok);
void freeASTLblCase(ASTLblCase *node);
int parseASTLblCase(
		ASTLblCase *node,
		struct Token const *tok,
		struct ASTScope *scope);
int printASTLblCase(ASTLblCase const *node);
ASTTravRes astLblCaseTrav(
		ASTLblCase *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);

int parseASTLblDefault(
		ASTNode *node,
		struct Token const *tok,
		struct ASTScope *scope);
int printASTLblDefault(ASTNode const *node);

int parseASTLabel(
		struct ASTNode *label,
		struct Token const *tok,
		struct ASTScope *scope);

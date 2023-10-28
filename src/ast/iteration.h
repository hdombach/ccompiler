#pragma once

#include "node.h"

struct ASTNode;
struct ASTStm;
struct Token;
struct ASTScope;

/************************************************************
 * While 
 ************************************************************/

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
		struct ASTScope *scope);
int printASTWhile(ASTWhile const *node);
ASTTravRes astWhileTrav(
		ASTWhile *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);

typedef struct ASTDoWhile {
	ASTNode node;
	struct ASTNode *expression;
	struct ASTStm *statement;
} ASTDoWhile;

/************************************************************
 * Do While 
 ************************************************************/

void initASTDoWhile(ASTDoWhile *node, struct Token const *tok);
void freeASTDoWhile(ASTDoWhile *node);
int parseASTDoWhile(
		ASTDoWhile *node,
		struct Token const *tok,
		struct ASTScope *scope);
int printASTDoWhile(ASTDoWhile const *node);
ASTTravRes astDoWhileTrav(
		ASTDoWhile *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);

typedef struct ASTFor {
	ASTNode node;
	ASTNode *initClause;
	ASTNode *condExp;
	ASTNode *iterExp;
	ASTNode *loopStm;
} ASTFor;

/************************************************************
 * For 
 ************************************************************/

void initASTFor(ASTFor *node, struct Token const *tok);
void freeASTFor(ASTFor *node);
int parseASTFor(
		ASTFor *node,
		struct Token const *tok,
		struct ASTScope *scope);
int printASTFor(ASTFor const *node);
ASTTravRes astForTrav(
		ASTFor *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);

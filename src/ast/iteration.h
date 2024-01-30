#pragma once

#include "node.h"

struct ASTNode;
struct Token;
struct ASTScope;

/************************************************************
 * While 
 ************************************************************/

typedef struct ASTWhile {
	ASTNode node;
	struct ASTNode *expression;
	ASTNode *statement;
} ASTWhile;

void initASTWhile(ASTWhile *node, struct Token const *tok);
void freeASTWhile(ASTWhile *node);
int parseASTWhile(
		ASTWhile *node,
		struct Token const *tok,
		struct ASTScope *scope);
int printASTWhile(ASTWhile const *node);
int astWhileChildCount(ASTWhile const *node);
ASTNode *astWhileGetChild(ASTWhile *node, int index);

typedef struct ASTDoWhile {
	ASTNode node;
	struct ASTNode *expression;
	ASTNode *statement;
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
int astDoWhileChildCount(ASTDoWhile const *node);
ASTNode *astDoWhileGetChild(ASTDoWhile *node, int index);

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
int astForChildCount(ASTFor const *node);
ASTNode *astForGetChild(ASTFor *node, int index);

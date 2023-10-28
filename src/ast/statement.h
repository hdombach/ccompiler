#pragma once

#include "iteration.h"
#include "expression.h"
#include "selection.h"
#include "label.h"
#include "node.h"

/***********************************************************************
 * Empty Statement
 ***********************************************************************/

typedef ASTNode ASTEmptyStm;

void initASTEmptyStm(ASTEmptyStm *node, Token const *tok);
int parseASTEmptyStm(ASTEmptyStm *node, Token const *tok, struct ASTScope *scope);
int printASTEmptyStm(ASTEmptyStm const *node);

/***********************************************************************
 * Break Statement
 ***********************************************************************/

typedef ASTNode ASTBreak;

void initASTBreak(ASTBreak *node, Token const *tok);
int parseASTBreak(ASTBreak *node,  Token const *tok, struct ASTScope *scope);
int printASTBreak(ASTBreak const *node);

/***********************************************************************
 * Continue Statement
 ***********************************************************************/

typedef ASTNode ASTContinue;

void initASTContinue(ASTContinue *node, Token const *tok);
int parseASTContinue(ASTContinue *node, Token const *tok, struct ASTScope *scope);
int printASTContinue(ASTContinue const *node);

struct ASTScope;
struct ASTCompStm;

typedef struct ASTStm {
	ASTNode node;
	ASTNode *content;
	ASTNode *label;
} ASTStm;

void initASTStm(ASTStm *node, Token const *tok);
void freeASTStm(ASTStm *node);

int parseASTStm(ASTStm *node, Token const *tok, struct ASTScope *scope);

int printASTStm(ASTStm const *node);
ASTTravRes astStmTrav(
		ASTStm *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);

#pragma once

#include "doWhile.h"
#include "expression.h"
#include "if.h"
#include "label.h"
#include "node.h"
#include "switch.h"
#include "while.h"

/***********************************************************************
 * Empty Statement
 ***********************************************************************/

typedef ASTNode ASTEmptyStm;

void initASTEmptyStm(ASTEmptyStm *node);
int parseASTEmptyStm(ASTEmptyStm *node, Token const *tok, struct ASTScope const *scope);
int printASTEmptyStm(ASTEmptyStm const *node);

/***********************************************************************
 * Break Statement
 ***********************************************************************/

typedef ASTNode ASTBreak;

void initASTBreak(ASTBreak *node);
int parseASTBreak(ASTBreak *node,  Token const *tok, struct ASTScope const *scope);
int printASTBreak(ASTBreak const *node);

/***********************************************************************
 * Continue Statement
 ***********************************************************************/

typedef ASTNode ASTContinue;

void initASTContinue(ASTContinue *node);
int parseASTContinue(ASTContinue *node, Token const *tok, struct ASTScope const *scope);
int printASTContinue(ASTContinue const *node);

struct ASTScope;
struct ASTCompStm;

typedef struct ASTStm {
	ASTNode node;
	ASTNode *content;
	ASTNode *label;
} ASTStm;

void initASTStm(ASTStm *node);
void freeASTStm(ASTStm *node);

int parseASTStm(ASTStm *node, Token const *tok, struct ASTScope const *scope);

int printASTStm(ASTStm const *node);

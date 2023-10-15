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

typedef enum {
	ASTS_UNKNOWN,
	ASTS_COMPOUND,
	ASTS_NODE,
	ASTS_IF,
	ASTS_EMPTY,
	ASTS_BREAK,
	ASTS_CONTINUE,
	ASTS_SWITCH,
	ASTS_WHILE,
	ASTS_DO_WHILE,
} ASTStmType;

struct ASTScope;
struct ASTCompStm;

typedef struct ASTStm {
	ASTNode node;
	ASTStmType type;	
	union {
		ASTNode *compStm;
		ASTNode *node;
		ASTNode *ifStm;
		ASTNode *switchStm;
		ASTNode *whileStm;
		ASTNode *doWhileStm;
	} c;
	ASTLabel *label;
} ASTStm;

void initASTStm(ASTStm *node);
void freeASTStm(ASTStm *node);

int parseASTStm(ASTStm *node, Token const *tok, struct ASTScope const *scope);

int printASTStm(ASTStm const *node);

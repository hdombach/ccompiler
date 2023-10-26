#pragma once

#include "../util/dlist.h"
#include "../token.h"
#include "node.h"
#include "statement.h"
#include "declaration.h"
#include "../sem/scope.h"

struct ASTScope;

typedef enum {
	AST_CIT_UNKNOWN,
	/* ASTCompItemType for statement */
	AST_CIT_STM,
	/* ASTCompItemType for declaration */
	AST_CIT_DECL,
} ASTCompItemType;

typedef struct ASTCompStm {
	ASTNode node;
	DList items;
	ASTScope scope;
} ASTCompStm;

int parseASTCompItem(
		ASTNode *node,
		Token const *tok,
		struct ASTScope *scope);

void initASTCompStm(ASTCompStm *node, Token const *tok);
void freeASTCompStm(ASTCompStm *node);
int parseASTCompStm(
		ASTCompStm *node,
		Token const *tok,
		struct ASTScope *scope);
int printASTCompStm(ASTCompStm const *node);
ASTTravRes astCompStmTrav(
		ASTCompStm *,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);

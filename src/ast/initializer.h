#pragma once

#include "expression.h"
#include "../token.h"
#include "../util/dlist.h"
#include "node.h"

typedef enum {
	AST_IT_UNKNOWN,
	AST_IT_NODE,
	AST_IT_LIST,
} ASTInitializerType;

struct ASTScope;

typedef struct ASTInitializerList {
	ASTNode node;
	DList list;
} ASTInitializerList;

void initASTInitializerList(ASTInitializerList *list, Token const *tok);
void freeASTInitializerList(ASTInitializerList *list);
int parseASTInitializerList(
		ASTInitializerList *list,
		Token const *tok,
		struct ASTScope *scope);
int printASTInitializerList(ASTInitializerList const *list);
ASTTravRes astInitializerListTrav(
		ASTInitializerList *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);
int astInitializerListChildCount(ASTInitializerList const *node);
ASTNode *astInitializerListGetChild(ASTInitializerList *node, int index);

int parseASTInitializer(
		ASTNode *node,
		Token const *tok,
		struct ASTScope *scope);

#pragma once

#include "declaration.h"
#include "funcDef.h"
#include "node.h"
#include "../sem/scope.h"

typedef struct {
	ASTNode node;
	DList items;
	struct ASTScope *scope;
} ASTFile;

/* parses declaration or func def */
int parseASTFileItem(ASTNode *item, Token const *tok, struct ASTScope *scope);

void initASTFile(ASTFile *file, Token const *tok);
void freeASTFile(ASTFile *file);
int parseASTFile(ASTFile *file, Token const *tok);
int printASTFile(ASTFile const *file);
int astFileChildCount(ASTFile const *node);
ASTNode *astFileGetChild(ASTFile *node, int index);

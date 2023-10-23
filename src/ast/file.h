#pragma once

#include "declaration.h"
#include "funcDef.h"
#include "node.h"
#include "scope.h"

typedef struct {
	ASTNode node;
	DList items;
	ASTScope scope;
} ASTFile;

/* parses declaration or func def */
int parseASTFileItem(ASTNode *item, Token const *tok, ASTScope const *scope);

DList astFileItemTypes(ASTNode const *item);

void initASTFile(ASTFile *file, Token const *tok);
void freeASTFile(ASTFile *file);
int parseASTFile(ASTFile *file, Token const *tok);
int printASTFile(ASTFile const *file);
ASTTravRes astFileTrav(
		ASTFile *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);

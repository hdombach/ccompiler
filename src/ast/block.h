#pragma once

#include "statement.h"
#include "../util/dlist.h"

typedef struct {
	DList statements;
} ASTBlock;

void initASTBlock(ASTBlock *node);
void freeASTBlock(ASTBlock *node);

int parseASTBlock(ASTBlock *node, Token const *tok);

int printASTBlock(ASTBlock const *node);

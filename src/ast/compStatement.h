#pragma once

#include "../util/dlist.h"
#include "../token.h"

struct ASTScope;

typedef struct {
	DList statements;
	struct ASTScope *scope;
} ASTCompStm;

void initASTCompStm(ASTCompStm *node);
void freeASTCompStm(ASTCompStm *node);

int parseASTCompStm(
		ASTCompStm *node,
		Token const *tok,
		struct ASTScope const *scope);
int printASTCompStm(ASTCompStm const *node);

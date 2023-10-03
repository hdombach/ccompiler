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

typedef struct {
	ASTInitializerType type;
	union {
		ASTNodeBuf nodeBuf;
		DList initializerList;
	} c;
} ASTInitializer;

void initASTInitializer(ASTInitializer *initializer);
void freeASTInitializer(ASTInitializer *initializer);
int parseASTInitializer(
		ASTInitializer *initializer,
		Token const *tok,
		struct ASTScope const *scope);
int printASTInitializer(ASTInitializer const *initializer);

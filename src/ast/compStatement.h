#pragma once

#include "../util/dlist.h"
#include "../token.h"
#include "statement.h"
#include "declaration.h"

struct ASTScope;

typedef enum {
	AST_CIT_UNKNOWN,
	/* ASTCompItemType for statement */
	AST_CIT_STM,
	/* ASTCompItemType for declaration */
	AST_CIT_DECL,
} ASTCompItemType;

typedef struct {
	ASTCompItemType type;
	union {
		ASTStm statement;
		ASTDeclaration declaration;
	} c;
} ASTCompItem;

typedef struct ASTCompStm {
	DList items;
	struct ASTScope *scope;
} ASTCompStm;

void initASTCompItem(ASTCompItem *item);
void freeASTCompItem(ASTCompItem *item);
int parseASTCompItem(
		ASTCompItem *item,
		Token const *tok,
		struct ASTScope const *scope);
int printASTCompItem(ASTCompItem const *node);

void initASTCompStm(ASTCompStm *node);
void freeASTCompStm(ASTCompStm *node);
int parseASTCompStm(
		ASTCompStm *node,
		Token const *tok,
		struct ASTScope const *scope);
int printASTCompStm(ASTCompStm const *node);

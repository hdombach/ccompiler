#pragma once

#include "../token.h"
#include "node.h"

struct ASTDeclarator;
struct ASTScope;

typedef struct ASTArrayDecl {
	struct ASTNode node;
	struct ASTNode *exp;
	struct ASTDeclarator *encl;
} ASTArrayDecl;

void initASTArrayDecl(ASTArrayDecl *decl);
void freeASTArrayDecl(ASTArrayDecl *decl);
/*
 * Takes ownership of encl
 */
int parseASTArrayDecl(
		ASTArrayDecl *decl,
		Token const *tok,
		struct ASTNode *encl,
		struct ASTScope const *scope);
int printASTArrayDecl(ASTArrayDecl const *decl);

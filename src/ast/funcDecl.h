#pragma once

#include "../token.h"
#include "../util/dlist.h"
#include "node.h"
#include "../sem/scope.h"

struct ASTDeclarator;
struct ASTScope;

typedef struct ASTFuncDecl {
	ASTNode node;
	DList params;
	struct ASTDeclarator *encl;
	int hasEllipses;
	/* used for params */
	ASTScope scope;
} ASTFuncDecl;

void initASTFuncDecl(ASTFuncDecl *decl, Token const *tok);
void freeASTFuncDecl(ASTFuncDecl *decl);
/*
 * Takes ownership of encl
 */
int parseASTFuncDecl(
		ASTFuncDecl *decl,
		Token const *tok,
		struct ASTDeclarator *encl,
		struct ASTScope *scope);
int printASTFuncDecl(ASTFuncDecl const *decl);
ASTTravRes astFuncDeclTrav(
		ASTFuncDecl *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);

#pragma once

#include "../token.h"
#include "../util/dlist.h"
#include "node.h"

struct ASTDeclarator;
struct ASTScope;

typedef struct ASTFuncDecl {
	ASTNode node;
	DList params;
	struct ASTDeclarator *encl;
	int hasEllipses;
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
		struct ASTScope const *scope);
int printASTFuncDecl(ASTFuncDecl const *decl);
ASTTravRes astFuncDeclTrav(
		ASTFuncDecl *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc);

#pragma once

#include "../util/dlist.h"
#include "../token.h"
#include "node.h"

typedef struct ASTDeclaration ASTDeclaration;
typedef struct ASTScope ASTScope;

typedef struct {
	ASTNode node;
	char *name;
	DList items;
	ASTScope *scope;
	int isUnion;
} ASTStructDecl;

void initASTStructDecl(ASTStructDecl *decl, Token const *tok);
void freeASTStructDecl(ASTStructDecl *decl);
int parseASTStructDecl(
		ASTStructDecl *decl,
		Token const *tok,
		ASTScope const *scope);
int printASTStructDecl(ASTStructDecl const *decl);
ASTTravRes astStructDeclTrav(
		ASTStructDecl *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc);

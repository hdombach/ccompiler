#pragma once

#include "../util/dlist.h"
#include "../token.h"
#include "node.h"

struct ASTScope;

typedef struct ASTEnumeratorDecl {
	ASTNode node;
	char *name;
	struct ASTNode *exp;
} ASTEnumeratorDecl;

typedef struct ASTEnumDecl {
	ASTNode node;
	char *name;
	DList enumerators;
} ASTEnumDecl;

void initASTEnumeratorDecl(ASTEnumeratorDecl *decl, Token const *tok);
void freeASTEnumeratorDecl(ASTEnumeratorDecl *decl);
int parseASTEnumeratorDecl(
		ASTEnumeratorDecl *decl,
		Token const *tok,
		struct ASTScope const *scope);
int printASTEnumeratorDecl(ASTEnumeratorDecl const *decl);
ASTTravRes astEnumeratorDeclTrav(
		ASTEnumeratorDecl *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc);

void initASTEnumDecl(ASTEnumDecl *decl, Token const *tok);
void freeASTEnumDecl(ASTEnumDecl *decl);
int parseASTEnumDecl(
		ASTEnumDecl *decl,
		Token const *tok,
		struct ASTScope const *scope);
int printASTEnumDecl(ASTEnumDecl const *decl);
ASTTravRes astEnumDeclTrav(
		ASTEnumDecl *decl,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc);

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

void initASTEnumeratorDecl(ASTEnumeratorDecl *decl);
void freeASTEnumeratorDecl(ASTEnumeratorDecl *decl);
int parseASTEnumeratorDecl(
		ASTEnumeratorDecl *decl,
		Token const *tok,
		struct ASTScope const *scope);
int printASTEnumeratorDecl(ASTEnumeratorDecl const *decl);

void initASTEnumDecl(ASTEnumDecl *decl);
void freeASTEnumDecl(ASTEnumDecl *decl);
int parseASTEnumDecl(
		ASTEnumDecl *decl,
		Token const *tok,
		struct ASTScope const *scope);
int printASTEnumDecl(ASTEnumDecl const *decl);

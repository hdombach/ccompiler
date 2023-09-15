#pragma once

#include "../token.h"
#include "../util/dlist.h"

struct ASTDeclarator;
struct ASTScope;

typedef struct ASTFuncDecl {
	DList params;
	struct ASTDeclarator *encl;
	int hasEllipses;
} ASTFuncDecl;

void initASTFuncDecl(ASTFuncDecl *decl);
void freeASTFuncDecl(ASTFuncDecl *decl);
int parseASTFuncDecl(
		ASTFuncDecl *decl,
		Token const *tok,
		struct ASTDeclarator *encl,
		struct ASTScope const *scope);
int printASTFuncDecl(ASTFuncDecl const *decl);

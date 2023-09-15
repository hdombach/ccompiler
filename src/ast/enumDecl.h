#pragma once

#include "expression.h"
#include "../util/dlist.h"

typedef struct ASTEnumeratorDecl {
	char *name;
	ASTExp *exp;
} ASTEnumeratorDecl;

typedef struct ASTEnumDecl {
	char *name;
	DList enumerators;
} ASTEnumDecl;

void initASTEnumeratorDecl(ASTEnumeratorDecl *decl);
void freeASTEnumeratorDecl(ASTEnumeratorDecl *decl);
int parseASTEnumeratorDecl(ASTEnumeratorDecl *decl, Token const *tok);
int printASTEnumeratorDecl(ASTEnumeratorDecl const *decl);

void initASTEnumDecl(ASTEnumDecl *decl);
void freeASTEnumDecl(ASTEnumDecl *decl);
int parseASTEnumDecl(ASTEnumDecl *decl, Token const *tok);
int printASTEnumDecl(ASTEnumDecl const *decl);

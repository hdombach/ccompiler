#pragma once

#include "declaration.h"
#include "../util/dlist.h"
#include "../token.h"
#include "scope.h"

typedef enum {
	AST_SDT_UNKNOWN,
	/* A variable declaration */
	AST_SDT_VAR,
	/* A bit field declaration */
	AST_SDT_BIT,
} ASTStructDeclItemType;

typedef struct ASTStructDeclItem {
	ASTStructDeclItemType type;
	union {
		ASTDeclaration declaration;
	} c;
} ASTStructDeclItem;

void initASTStructDeclItem(ASTStructDeclItem *item);
void freeASTStructDeclItem(ASTStructDeclItem *item);
int parseASTStructDeclItem(
		ASTStructDeclItem *item,
		Token const *tok,
		ASTScope *scope);
int printASTStructDeclItem(ASTStructDeclItem const *item);

typedef struct {
	char *name;
	DList items;
	ASTScope scope;
} ASTStructDecl;

void initASTStructDecl(ASTStructDecl *decl);
void freeASTStructDecl(ASTStructDecl *decl);
int parseASTStructDecl(ASTStructDecl *decl, Token const *tok, ASTScope *scope);
int printASTStructDecl(ASTStructDecl const *decl);

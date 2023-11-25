#pragma once

#include "../token.h"
#include "node.h"
#include "../util/dlist.h"
#include "../sem/scope.h"

struct ASTDeclarator;
struct ASTScope;

/************************************************************
 * Array Decl
 ************************************************************/

typedef struct ASTArrayDecl {
	struct ASTNode node;
	struct ASTNode *exp;
	struct ASTNode *encl;
} ASTArrayDecl;

void initASTArrayDecl(ASTArrayDecl *decl, struct Token const *tok);
void freeASTArrayDecl(ASTArrayDecl *decl);
/*
 * Takes ownership of encl
 */
int parseASTArrayDecl(
		ASTArrayDecl *decl,
		Token const *tok,
		struct ASTNode *encl,
		struct ASTScope *scope);
int printASTArrayDecl(ASTArrayDecl const *decl);
int astArrayDeclChildCount(ASTArrayDecl const *node);
ASTNode *astArrayDeclGetChild(ASTArrayDecl *node, int index);

/************************************************************
 * Enum Decl
 ************************************************************/

typedef struct ASTEnumConst {
	ASTNode node;
	char *name;
	struct ASTNode *exp;
} ASTEnumConst;

typedef struct ASTEnumDecl {
	ASTNode node;
	char *name;
	DList enumerators;
} ASTEnumDecl;

void initASTEnumConst(ASTEnumConst *decl, Token const *tok);
void freeASTEnumConst(ASTEnumConst *decl);
int parseASTEnumConst(
		ASTEnumConst *decl,
		Token const *tok,
		struct ASTScope *scope);
int printASTEnumConst(ASTEnumConst const *decl);
int astEnumConstChildCount(ASTEnumConst const *node);
ASTNode *astEnumConstGetChild(ASTEnumConst *node, int index);

void initASTEnumDecl(ASTEnumDecl *decl, Token const *tok);
void freeASTEnumDecl(ASTEnumDecl *decl);
int parseASTEnumDecl(
		ASTEnumDecl *decl,
		Token const *tok,
		struct ASTScope *scope);
int printASTEnumDecl(ASTEnumDecl const *decl);
int astEnumDeclChildCount(ASTEnumDecl const *node);
ASTNode *astEnumDeclGetChild(ASTEnumDecl *node, int index);

/************************************************************
 * Struct Decl
 ************************************************************/

typedef struct {
	ASTNode node;
	char *name;
	/**
	 * @brief Is a list of ASTDeclarations
	 */
	DList items;
	struct ASTScope *scope;
	int isUnion;
} ASTStructDecl;

void initASTStructDecl(ASTStructDecl *decl, Token const *tok);
void freeASTStructDecl(ASTStructDecl *decl);
int parseASTStructDecl(
		ASTStructDecl *decl,
		Token const *tok,
		struct ASTScope *scope);
int printASTStructDecl(ASTStructDecl const *decl);
int astStructDeclChildCount(ASTStructDecl const *node);
ASTNode *astStructDeclGetChild(ASTStructDecl *node, int index);

/************************************************************
 * Func Decl
 ************************************************************/

typedef struct ASTFuncDecl {
	ASTNode node;
	DList params;
	ASTNode *encl;
	int hasEllipses;
	/* used for params */
	ASTScope *scope;
} ASTFuncDecl;

void initASTFuncDecl(ASTFuncDecl *decl, Token const *tok);
void freeASTFuncDecl(ASTFuncDecl *decl);
/*
 * Takes ownership of encl
 */
int parseASTFuncDecl(
		ASTFuncDecl *decl,
		Token const *tok,
		ASTNode *encl,
		struct ASTScope *scope);
int printASTFuncDecl(ASTFuncDecl const *decl);
int astFuncDeclChildCount(ASTFuncDecl const *node);
ASTNode *astFuncDeclGetChild(ASTFuncDecl *node, int index);

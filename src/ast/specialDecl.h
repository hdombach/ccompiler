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
ASTTravRes astArrayDeclTrav(
		ASTArrayDecl *node,
		ASTTravFunc beforefunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);

/************************************************************
 * Enum Decl
 ************************************************************/

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
		struct ASTScope *scope);
int printASTEnumeratorDecl(ASTEnumeratorDecl const *decl);
ASTTravRes astEnumeratorDeclTrav(
		ASTEnumeratorDecl *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);

void initASTEnumDecl(ASTEnumDecl *decl, Token const *tok);
void freeASTEnumDecl(ASTEnumDecl *decl);
int parseASTEnumDecl(
		ASTEnumDecl *decl,
		Token const *tok,
		struct ASTScope *scope);
int printASTEnumDecl(ASTEnumDecl const *decl);
ASTTravRes astEnumDeclTrav(
		ASTEnumDecl *decl,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);

/************************************************************
 * Struct Decl
 ************************************************************/

typedef struct {
	ASTNode node;
	char *name;
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
ASTTravRes astStructDeclTrav(
		ASTStructDecl *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);

/************************************************************
 * Func Decl
 ************************************************************/

typedef struct ASTFuncDecl {
	ASTNode node;
	DList params;
	ASTNode *encl;
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
		ASTNode *encl,
		struct ASTScope *scope);
int printASTFuncDecl(ASTFuncDecl const *decl);
ASTTravRes astFuncDeclTrav(
		ASTFuncDecl *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);

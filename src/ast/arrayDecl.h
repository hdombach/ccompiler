#pragma once

#include "../token.h"

struct ASTDeclarator;
struct ASTScope;
struct ASTExp;
typedef struct ASTArrayDecl {
	struct ASTExp *exp;
	struct ASTDeclarator *encl;
} ASTArrayDecl;

void initASTArrayDecl(ASTArrayDecl *decl);
void freeASTArrayDecl(ASTArrayDecl *decl);
int parseASTArrayDecl(ASTArrayDecl *decl, Token const *tok, struct ASTDeclarator *encl);
int printASTArrayDecl(ASTArrayDecl const *decl);

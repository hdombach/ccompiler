#pragma once

#include "../token.h"
#include "specialDecl.h"
#include "node.h"

typedef struct ASTDeclarator ASTDeclarator;
typedef struct ASTTypeSpec ASTTypeSpec;

typedef struct ASTParam {
	ASTNode node;
	ASTTypeSpec *typeSpec;
	ASTDeclarator *declarator;
} ASTParam;

void initASTParam(ASTParam *param);
void freeASTParam(ASTParam *param);
int parseASTParam(ASTParam *param, Token const *tok, ASTScope *scope);
int printASTParam(ASTParam const *param);
int astParamChildCount(ASTParam const *node);
ASTNode *astParamGetChild(ASTParam *node, int index);

#pragma once

#include "declaration.h"
#include "../token.h"
#include "structDecl.h"

typedef struct ASTParam {
	ASTTypeSpec typeSpec;
	ASTDeclarator *declarator;
} ASTParam;

void initASTParam(ASTParam *param);
void freeASTParam(ASTParam *param);
int parseASTParam(ASTParam *param, Token const *tok, ASTScope const *scope);
int printASTParam(ASTParam const *param);

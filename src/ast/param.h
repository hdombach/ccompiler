#pragma once

#include "declaration.h"
#include "../token.h"
#include "structDecl.h"
#include "node.h"

typedef struct ASTParam {
	ASTNode node;
	ASTTypeSpec *typeSpec;
	ASTDeclarator *declarator;
} ASTParam;

void initASTParam(ASTParam *param);
void freeASTParam(ASTParam *param);
int parseASTParam(ASTParam *param, Token const *tok, ASTScope const *scope);
int printASTParam(ASTParam const *param);
ASTTravRes astParamTrav(
		ASTParam *param,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc);

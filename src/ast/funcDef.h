#pragma once

#include "../token.h"
#include "../util/dlist.h"
#include "compStatement.h"
#include "declaration.h"
#include "funcDecl.h"
#include "node.h"

struct ASTDeclarator;
struct ASTScope;

typedef struct ASTFuncDef {
	ASTNode node;
	ASTTypeSpec *typeSpec;
	ASTNode *funcDecl;
	ASTCompStm *compoundStm;
} ASTFuncDef;

void initASTFuncDef(ASTFuncDef *def, Token const *tok);
void freeASTFuncDef(ASTFuncDef *def);
int parseASTFuncDef(
		ASTFuncDef *def,
		Token const * tok,
		struct ASTScope const *scope);
int printASTFuncDef(ASTFuncDef const *def);
ASTTravRes astFuncDefTrav(
		ASTFuncDef *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc);

#pragma once

#include "intConstant.h"
#include "operation.h"
#include "identifier.h"
#include "node.h"

struct ASTScope;

typedef struct ASTExp {
	ASTNode node;
} ASTExp;

void initASTExp(ASTExp *node);
void freeASTExp(ASTExp *node);

int parseASTExp(ASTExp *node, Token const *tok, struct ASTScope const *scope);
int parseASTExpSing(ASTExp *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp15(ASTExp *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp14(ASTExp *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp13(ASTExp *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp12(ASTExp *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp11(ASTExp *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp10(ASTExp *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp9(ASTExp *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp8(ASTExp *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp7(ASTExp *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp6(ASTExp *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp5(ASTExp *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp4(ASTExp *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp3(ASTExp *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp2(ASTExp *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp1(ASTExp *node, Token const *tok, struct ASTScope const *scope);

int printASTExp(ASTExp const *node);

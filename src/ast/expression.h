#pragma once

#include "intConstant.h"
#include "operation.h"
#include "identifier.h"
#include "node.h"

struct ASTScope;

int parseASTExp(ASTNode *node, Token const *tok, struct ASTScope const *scope);
int parseASTExpSing(ASTNode *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp15(ASTNode *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp14(ASTNode *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp13(ASTNode *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp12(ASTNode *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp11(ASTNode *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp10(ASTNode *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp9(ASTNode *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp8(ASTNode *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp7(ASTNode *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp6(ASTNode *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp5(ASTNode *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp4(ASTNode *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp3(ASTNode *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp2(ASTNode *node, Token const *tok, struct ASTScope const *scope);
int parseASTExp1(ASTNode *node, Token const *tok, struct ASTScope const *scope);

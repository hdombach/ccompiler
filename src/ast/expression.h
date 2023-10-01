#pragma once

#include "intConstant.h"
#include "operation.h"
#include "identifier.h"
#include "node.h"

typedef enum {
	ASTE_UNKNOWN,
	ASTE_INT_CONSTANT,
	ASTE_OPERATION,
	ASTE_IDENTIFIER,
} ASTExpType;

struct ASTScope;

typedef struct ASTExp {
	ASTNode node;
	ASTExpType type;
	union {
		ASTIntConstant intConstant;
		ASTOperation operation;	
		ASTIdentifier identifier;
	} c;
	char buf[AST_NODE_S]; /* temp */
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

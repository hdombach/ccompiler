#pragma once

#include "intConstant.h"
#include "operation.h"

typedef enum {
	ASTE_UNKNOWN,
	ASTE_INT_CONSTANT,
	ASTE_OPERATION,
	ASTE_IDENTIFIER,
} ASTExpType;

struct ASTScope;

typedef struct ASTExp {
	ASTExpType type;
	union {
		ASTIntConstant intConstant;
		ASTOperation operation;	
		char *identifier;
	} c;
} ASTExp;

void initASTExp(ASTExp *node);
void freeASTExp(ASTExp *node);

int parseASTExp(ASTExp *node, Token const *tok, struct ASTScope *scope);
int parseASTExpSing(ASTExp *node, Token const *tok, struct ASTScope *scope);
int parseASTExp15(ASTExp *node, Token const *tok, struct ASTScope *scope);
int parseASTExp14(ASTExp *node, Token const *tok, struct ASTScope *scope);
int parseASTExp13(ASTExp *node, Token const *tok, struct ASTScope *scope);
int parseASTExp12(ASTExp *node, Token const *tok, struct ASTScope *scope);
int parseASTExp11(ASTExp *node, Token const *tok, struct ASTScope *scope);
int parseASTExp10(ASTExp *node, Token const *tok, struct ASTScope *scope);
int parseASTExp9(ASTExp *node, Token const *tok, struct ASTScope *scope);
int parseASTExp8(ASTExp *node, Token const *tok, struct ASTScope *scope);
int parseASTExp7(ASTExp *node, Token const *tok, struct ASTScope *scope);
int parseASTExp6(ASTExp *node, Token const *tok, struct ASTScope *scope);
int parseASTExp5(ASTExp *node, Token const *tok, struct ASTScope *scope);
int parseASTExp4(ASTExp *node, Token const *tok, struct ASTScope *scope);
int parseASTExp3(ASTExp *node, Token const *tok, struct ASTScope *scope);
int parseASTExp2(ASTExp *node, Token const *tok, struct ASTScope *scope);
int parseASTExp1(ASTExp *node, Token const *tok, struct ASTScope *scope);

int printASTExp(ASTExp const *node);

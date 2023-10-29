#pragma once

#include "../util/dlist.h"
#include "../token.h"
#include "node.h"

struct ASTDeclaration;
struct ASTParam;
struct ASTScope;


/*************************************************************
 * Func Operation
 *************************************************************/

typedef struct ASTFuncOperation {
	ASTNode node;
	/* expression */
	ASTNode *func;
	DList params;
} ASTFuncOperation;

void initASTFuncOperation(ASTFuncOperation *node, Token const *tok);
void freeASTFuncOperation(ASTFuncOperation *node);
/* If the parse fails, func is not freed */
int parseASTFuncOperation(
		ASTFuncOperation *node,
		Token const *tok,
		ASTNode *func,
		struct ASTScope *scope);
int printASTFuncOperation(ASTFuncOperation const *node);
ASTTravRes astFuncOperationTrav(
		ASTFuncOperation *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);
int astFuncOperationChildCount(ASTFuncOperation const *node);
ASTNode *astFuncOperationGetChild(ASTFuncOperation *node, int index);

/*************************************************************
 * Cond Operation
 *************************************************************/

/* or ternerary */
typedef struct ASTCondOperation {
	ASTNode node;
	ASTNode *condition;
	ASTNode *trueExp;
	ASTNode *falseExp;
} ASTCondOperation;

void initASTCondOperation(ASTCondOperation *node, Token const *tok);
void freeASTCondOperation(ASTCondOperation *node);
int parseASTCondOperation(
		ASTCondOperation *node,
		Token const *tok,
		struct ASTScope *scope);
int printASTCondOperation(ASTCondOperation const *node);
ASTTravRes astCondOperationTrav(
		ASTCondOperation *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);
int astCondOperationChildCount(ASTCondOperation const *node);
ASTNode *astCondOperationGetChild(ASTCondOperation *node, int index);

/*************************************************************
 * Operation
 *************************************************************/

/*
 * Is used for Subscript, cast, sizeof, binary, prefix, postfix
 */
typedef struct ASTOperation {
	ASTNode node;
	TokenType tokType;
	ASTNode *lhs;
	ASTNode *rhs;
} ASTOperation;

void initASTOperation(ASTOperation *node, Token const *tok);
void freeASTOperation(ASTOperation *node);
int parseASTOperation15(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
int parseASTOperation14(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
int parseASTOperation13(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
int parseASTOperation12(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
int parseASTOperation11(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
int parseASTOperation10(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
int parseASTOperation9(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
int parseASTOperation8(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
int parseASTOperation7(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
int parseASTOperation6(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
int parseASTOperation5(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
int parseASTOperation4(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
int parseASTOperation3(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
int parseASTOperation2(
		ASTOperation *node,
		Token const *tok,
		struct ASTScope *scope);
int parseASTOperation1(
		ASTOperation *node,
		const Token *tok,
		struct ASTScope *scope,
		struct ASTNode *lhs);
int printASTOperation(ASTOperation const *node);

ASTTravRes astOperationTrav(
		ASTOperation *node,
		ASTTravFunc beforeFunc,
		ASTTravFunc afterFunc,
		ASTTravCtx *ctx);
int astOperationChildCount(ASTOperation const *node);
ASTNode *astOperationGetChild(ASTOperation *node, int index);
